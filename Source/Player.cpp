#include "Player.h"
#include <assert.h>
#include "Stage.h"
#include "../ImGui/imgui.h"
#include "CsvReader.h"
#include <time.h>
#include "Item.h"
#include "SoundManager.h"

int g_score = 0;
int g_time = 0;

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
	onGround = false;
	prevPushed = false;
	AttackPower = 0.0f;
	playerHP = 100;
	invincibleMax = 1.0f;  
	invincibleTime = 0.0f;
	
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
			g_soundManager->Play(SoundType::Jump);
		}
		else {
			prevPushed = false;
			
		}
	}
	{
		// 重力処理
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
	
	//アイテムを取得した際の処理
	for (const auto& offset : offsets) {
		VECTOR2 checkPos = position + offset;
		int chip = st->GetChip(checkPos);
		if (chip == 0) continue;

		ItemType type;
		switch (chip) {
		case 10: type = ItemType::GameClear; break;
		case 11: type = ItemType::Score; break;
		case 14: type = ItemType::SpeedUp; break;
		case 15: type = ItemType::Ricaver; break;
		case 16: type = ItemType::GravityUp; break;
		case 17: type = ItemType::JumpUp; break;
		case 12: type = ItemType::Sword; break;
		case 13: type = ItemType::Shield; break;
		default: continue;
		}

		Item item(type);
		item.Apply(this, st, checkPos);

		// スコアやゲームクリアの場合は処理をスキップ
		if (type == ItemType::Score || type == ItemType::GameClear) return;
	}

	// 無敵時間の更新
	const float dt = 1.0f / 60.0f;

	if (invincibleTime > 0.0f)
	{
		invincibleTime -= dt;
		if (invincibleTime < 0.0f)
			invincibleTime = 0.0f;
	}

	// ゲームオーバー判定
	if (playerHP <= 0)
	{
		SceneManager::ChangeScene("GAMEOVER");
	}
	
     /*ImGui::Begin("Player");
		ImGui::Checkbox("onGround", &onGround);
		ImGui::InputFloat("positionY", &position.y);
		ImGui::End();*/
}

	

void Player::DrawScore()
{
// スコアを表示
 float fSize = SetFontSize(30);
DrawFormatString(10, 10, GetColor(0, 0, 0), "Score: %d", g_score,fSize);
DrawFormatString(10, 50, GetColor(0, 0, 0), "攻撃可能回数: %.0f", AttackPower,fSize);
DrawFormatString(10, 90, GetColor(0, 0, 0), "体力: %d", playerHP, fSize);

}

Rect Player::GetRect() const
{
	return MakeRect(position, imageSize);
}



int Player::GetAttackPower() const
{
	return AttackPower;
}


float Player::GetvelocityY() const
{
	return velocityY;
}

bool Player::IsInvincible() const
{
	return invincibleTime > 0.0f;
}

void Player::ConsumeAttackPower(int v)
{
	AttackPower -= v;
	if (AttackPower < 0) AttackPower = 0;
}

void Player::Bounce()
{
	velocityY = JumpV0 * 0.7f; // 通常ジャンプより少し弱め
	onGround = false;
}

void Player::Damage(int dmg)
{
	if (invincibleTime > 0.0f) return;

	playerHP -= dmg;
	invincibleTime = invincibleMax;
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

	// 無敵時間中は点滅表示
	if (invincibleTime > 0.0f && ((int)(invincibleTime * 10) % 2 == 0))
		return;

	DrawScore();
	
}

VECTOR2 Player::GetPosition() const
{
	return position;
}


