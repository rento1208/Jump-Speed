#include "ResultScene.h"
#include "Stage.h"

ResultScene::ResultScene()
{
	hImage = LoadGraph("data/image/Stage.jpeg");
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void ResultScene::Draw()
{
	DrawExtendGraph(0, 0, 1280, 720, hImage, TRUE);
	int rsize = SetFontSize(50);
	DrawString(450, 200, "リザルトシーン", rsize, GetColor(0, 0, 0));
	int time = GetNowCount();
	if ((time / 700) % 2 == 0) {
		DrawString(390, 450, "Push [T]Key To Title", rsize, GetColor(0, 0, 0));
	}
	
}
