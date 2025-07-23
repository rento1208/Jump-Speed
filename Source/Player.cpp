#include "Player.h"
#include <assert.h>
#include "Stage.h"
#include "../ImGui/imgui.h"
#include "CsvReader.h"
#include <time.h>
#include <windows.h>

int g_score = 0;
//static const float Gravity = 0.05f;
//static const float JumpHeight = 64.0f * 2.0f;
//static const float JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);

Player::Player() : Player(VECTOR2(100,200))
{
}

Player::Player(VECTOR2 pos)

{
	// パラメーターを読む
	CsvReader* csv = new CsvReader("data/playerParam.csv");
	for (int i = 0; i < csv->GetLines(); i++) {
		std::string tag = csv->GetString(i, 0);
		if (tag == "Gravity") {
			Gravity = csv->GetFloat(i, 1);
		}
		else if (tag == "JumpHeight") {
			JumpHeight = csv->GetFloat(i, 1);

		}
		else if (tag == "MoveSpeed") {
			moveSpeed = csv->GetFloat(i, 1);
		}
	}
	JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);

	hImage = LoadGraph("data/image/tamadot.png");
	assert(hImage > 0);

	imageSize = VECTOR2(64, 64);
	anim = 0;
	animY = 3;
	animTimer = 0.0f;
	animInterval = 0.1f;

	position = pos;
	velocityY = 0.0f;
	hSound = LoadSoundMem("data/sound/jump.mp3");
	hSound2 = LoadSoundMem("data/sound/coin.mp3");
	hSound3 = LoadSoundMem("data/sound/status.mp3");
	hSound4 = LoadSoundMem("data/sound/result.mp3");
	hSound5 = LoadSoundMem("data/sound/poison.mp3");
}
	

Player::~Player()
{
}

void Player::Update()
{
	Stage* st = FindGameObject<Stage>();

	
	
	//プレイヤー移動（右）
	if (CheckHitKey(KEY_INPUT_D)) {
			position.x += moveSpeed;
			int push = st->CheckRight(position + VECTOR2(24, -31)); // 右上
			position.x -= push;
			push = st->CheckRight(position + VECTOR2(24, 31)); // 右下
			position.x -= push;
			animTimer += 1.0f / 60.0f;
			if (onGround) {
				if (animTimer >= animInterval) {
					animTimer = 0.0f;
					anim = (anim + 5) % 7;
					animY = 3;
				}
			}
			else {
				anim = 0;
				animY = 3;
			}
	}
	
	//プレイヤー移動（左）
	if (CheckHitKey(KEY_INPUT_A)) {
		position.x -= moveSpeed;
		int push = st->CheckLeft(position + VECTOR2(-24, -31)); // 左上
		position.x += push;
		push = st->CheckLeft(position + VECTOR2(-24, 31)); // 左下
		position.x += push;
	}
	
	//プレイヤー移動（ジャンプ）
	if (onGround) {
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			if (prevPushed == false) {
				velocityY = JumpV0;
				}
			prevPushed = true;
			PlaySoundMem(hSound, DX_PLAYTYPE_BACK);
		}
		else {
			prevPushed = false;
			
		}
	}
	{
		position.y += velocityY;
		velocityY += Gravity;
		onGround = false;
		if (velocityY < 0.0f) {
			int push = st->CheckUp(position + VECTOR2(-24, -31)); // 左下
			if (push > 0) {
				velocityY = 0.0f;
				position.y += push;
			}
			push = st->CheckUp(position + VECTOR2(24, -31)); // 右下
			if (push > 0) {
				velocityY = 0.0f;
				position.y += push;
			}
		}
		else {
			int push = st->CheckDown(position + VECTOR2(-24, 31+1)); // 左下
			if (push > 0) {
				velocityY = 0.0f;
				onGround = true;
				position.y -= push-1;
			}
			push = st->CheckDown(position + VECTOR2(24, 31+1)); // 右下
			if (push > 0) {
				velocityY = 0.0f;
				onGround = true;
				position.y -= push-1;
			}
		}
	}
	// プレイヤーの表示位置が、400よりも右だったら、スクロールする
	if (st != nullptr) {
		float drawX = position.x - st->ScrollX(); // これが表示座標
		static const int RightLimit = 400;
		static const int LeftLimit = 400;
		if (drawX > RightLimit) {
			st->SetScrollX(position.x - RightLimit);
		}
		else if (drawX < LeftLimit) {
			st->SetScrollX(position.x - LeftLimit);
		}
	}
	VECTOR2 offsets[] = {
	VECTOR2(-24, -32), // 左上
	VECTOR2(0, -32), // 真上
	VECTOR2(24, -32), // 右上
	VECTOR2(-32,   0), // 左中央
	VECTOR2(32,   0), // 右中央
	VECTOR2(-24,  32), // 左下
	VECTOR2(0,  32), // 真下
	VECTOR2(24,  32), // 右下
	};

	//チップ17：ジャンプ力アップ
	for (const auto& offset : offsets) {
		VECTOR2 checkPos = position + offset;
		int chip = st->GetChip(checkPos);
		if (chip == 17) {
			PlaySoundMem(hSound3, DX_PLAYTYPE_BACK); 
			JumpHeight += 80.0f;
			JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);
			st->RemoveChip(checkPos);
			break;
		}
	}

	//チップ14：移動速度アップ
	for (const auto& offset : offsets) {
		VECTOR2 checkPos = position + offset;
		int chip = st->GetChip(checkPos);
		if (chip == 14) {
			PlaySoundMem(hSound3, DX_PLAYTYPE_BACK);  //SE再生
			moveSpeed += 0.5f;
			st->RemoveChip(checkPos);
			break;
		}
	}
	//チップ16 :詰み（重力+）
	for (const auto& offset : offsets) {
		VECTOR2 checkPos = position + offset;
		int chip = st->GetChip(checkPos);
		if (chip == 16) {
			PlaySoundMem(hSound5, DX_PLAYTYPE_BACK); //SE再生
			JumpHeight = JumpHeight;
			JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);
			moveSpeed = moveSpeed;
			Gravity += 3.0f;
			st->RemoveChip(checkPos);
			break;
		}
	}
	//チップ15 : 全能力回復
	for (const auto& offset : offsets) {
		VECTOR2 checkPos = position + offset;
		int chip = st->GetChip(checkPos);
		if (chip == 15) {
			//JumpHeight = 100.0f;
			//JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);
			//moveSpeed = 1.5;
			//Gravity -= 3.0f;
			st->RemoveChip(checkPos);
			break;
		}
	}

		//チップ11：スコアプラス
		for (const auto& offset : offsets) {
			VECTOR2 checkPos = position + offset;
			int chip = st->GetChip(checkPos);

			if (chip == 11) {
				PlaySoundMem(hSound2, DX_PLAYTYPE_BACK);  //SE再生
				st->RemoveChip(checkPos);  // チップを消す
				g_score += 1;

				return; // 以降の更新処理をスキップ
			}
		}
		//チップ10：ゲームクリア条件
		for (const auto& offset : offsets) {
			VECTOR2 checkPos = position + offset;
			int chip = st->GetChip(checkPos);
			if (chip == 10) {
				PlaySoundMem(hSound4, DX_PLAYTYPE_BACK); //SE再生
				st->RemoveChip(checkPos);  // チップを消す
				SceneManager::ChangeScene("GAMECLEAR"); // シーン遷移
				return; // 以降の更新処理をスキップ
			}
		}
		



		/*ImGui::Begin("Player");
		ImGui::Checkbox("onGround", &onGround);
		ImGui::InputFloat("positionY", &position.y);
		ImGui::End();*/
	}

void Player::DrawScore()
{
	//スコアを表示
	DrawFormatString(10, 10, GetColor(0, 0, 0), "Score: %d", g_score);
}

void Player::Draw()
{
	Object2D::Draw();
	Stage* st = FindGameObject<Stage>();
	float x = position.x - st->ScrollX();
	/*DrawBox(x - 24, position.y - 32, x + 24, position.y + 32,
		GetColor(255, 0, 0), FALSE);*/

	int srcX = anim * (int)imageSize.x;
	int srcY = animY * (int)imageSize.y;

	DrawRectGraph(
		(int)(x - imageSize.x / 2),                 // 描画X
		(int)(position.y - imageSize.y / 2),        // 描画Y
		srcX, srcY,                                 // 元画像の切り取り位置
		(int)imageSize.x, (int)imageSize.y,         // 切り取るサイズ
		hImage, TRUE);     
	DrawScore();
	
}

VECTOR2 Player::GetPosition() const
{
	return position;
}


