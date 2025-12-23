#include "GameClearScene.h"
#include "Stage.h"
#include "Player.h"
#include "../GameData.h"
GameClearScene::GameClearScene()
	:SceneBase(), hImage(0), changeTimer(0.0f)
{
	hImage = LoadGraph("data/image/Stage.jpeg");
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
		g_score = 0;
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void GameClearScene::Draw()
{
	DrawExtendGraph(0, 0, 1280, 720, hImage, TRUE);
	
	int rsize = SetFontSize(50);
	DrawString(450, 200, "ゲームクリア！", rsize, GetColor(0, 0, 0));
	
	// スコア表示
	DrawFormatString(450, 350, GetColor(255, 255, 255), "Your Score: %d", g_score);

	//クリアタイム表示
	DrawFormatString(300, 400, GetColor(255, 255, 255), "Clear Time: %.2f seconds", g_clearTime / 60.0f);
	
	int time = GetNowCount();
	if ((time / 700) % 2 == 0) {
		DrawString(390, 450, "Push [T]Key To Title", rsize, GetColor(0, 0, 0));
	}

}
