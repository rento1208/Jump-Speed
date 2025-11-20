#include "PlayScene.h"
#include <DxLib.h>
#include "Stage.h"
#include "Object2D.h"
#include "Player.h"
#include <assert.h>
#include "SoundManager.h"



PlayScene::PlayScene()
{
	g_soundManager = new SoundManager();
	hBgm = LoadSoundMem("data/sound/background.mp3");
	hBgm2 = LoadSoundMem("data/sound/gameover.mp3");
	

	hImage = LoadGraph("data/image/Stage.jpeg");
	
    new Stage();
	hBgm = LoadSoundMem("data/sound/background.mp3");
	assert(hBgm != -1);
}

PlayScene::~PlayScene()
{
	StopSoundMem(hBgm);
	DeleteSoundMem(hBgm);
}

void PlayScene::Update()
{
	if (CheckSoundMem(hBgm) == 0) {
		PlaySoundMem(hBgm, DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(150, hBgm);
	}
	
	Player* player = FindGameObject<Player>();
	
	if (player && player->GetPosition().y > 1200) {
	
		SceneManager::ChangeScene("GAMEOVER");
		PlaySoundMem(hBgm2, DX_PLAYTYPE_BACK);

	}
}

void PlayScene::Draw()
{
	DrawExtendGraph(0, 0, 1280, 720, hImage, TRUE);
	//DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	//DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}



//コードメモリー　 Code Memory
//スタックメモリー Stack Memory
//ヒープメモリー   Heap Memory
