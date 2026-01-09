#include "TitleScene.h"
#include <DxLib.h>
#include "TitleStage.h"
#include "../GameData.h"

TitleScene::TitleScene()
{
	//hImage = LoadGraph("data/image/Stage.jpeg");
	hImage = LoadGraph("data/image/Title.png");
	
	new TitleStage();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		g_time = 0;
		g_clearTime = 0;
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	DrawExtendGraph(0, 0, 1280, 720, hImage, TRUE);
	

	int time = GetNowCount();

	if ((time / 700) % 2 == 0) {
		int fsize = SetFontSize(50);

		DrawString(300, 550, "Push [SPACE]Key To Play", fsize, GetColor(0, 0, 0));
	}
	
}
