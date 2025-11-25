#include "PlayScene.h"
#include <DxLib.h>
#include "Stage.h"
#include "Object2D.h"
#include "Player.h"
#include <assert.h>
#include "SoundManager.h"



PlayScene::PlayScene()
	:SceneBase(), hImage_(0)
{
	new Stage();
	hImage_ = LoadGraph("data/image/Stage.jpeg");
	
	g_soundManager = new SoundManager();
    g_soundManager->Play(SoundType::BackGround);

}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
    // BGM再生(ループ)
    g_soundManager->PlayBGM(SoundType::BackGround);

    Player* player = FindGameObject<Player>();

    if (player && player->GetPosition().y > 1200)
    {
        // ゲームオーバー音を再生
        g_soundManager->Play(SoundType::GameOver);

        // シーン切替
        SceneManager::ChangeScene("GAMEOVER");
    }
}

void PlayScene::Draw()
{
	DrawExtendGraph(0, 0, 1280, 720, hImage_, TRUE);
	//DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	//DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}



//コードメモリー　 Code Memory
//スタックメモリー Stack Memory
//ヒープメモリー   Heap Memory
