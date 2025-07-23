#include "GameOverScene.h"
#include "Stage.h"
#include "Player.h"

GameOverScene::GameOverScene()
{
	hImage = LoadGraph("data/image/GameOver.jpg");
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
		g_score = 0;
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void GameOverScene::Draw()
{
	DrawExtendGraph(0, 0, 1280, 720, hImage, TRUE);
	int rsize = SetFontSize(50);
	//DrawString(450, 200, "ゲームオーバー", rsize, GetColor(255, 255, 0));
	int time = GetNowCount();
	if ((time / 700) % 2 == 0) {
		DrawString(390, 450, "Push [T]Key To Title", GetColor(255, 255, 255));
	}
	
}
