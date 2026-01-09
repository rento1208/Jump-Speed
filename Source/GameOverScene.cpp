#include "GameOverScene.h"
#include "Stage.h"
#include "Player.h"

GameOverScene::GameOverScene()
	:SceneBase(), hImage(0), changeTimer(0.0f)
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
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void GameOverScene::Draw()
{
	
	DrawExtendGraph(0, 0, 1280, 720, hImage, TRUE);
	int rsize = SetFontSize(50);
	int time = GetNowCount();
	if ((time / 700) % 2 == 0) {
		DrawString(390, 450, "Push [T]Key To Title", GetColor(255, 255, 255));
	}
	
}
