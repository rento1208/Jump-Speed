#include "PlayScene.h"
#include <DxLib.h>
#include "Stage.h"
#include "Object2D.h"
#include "Player.h"
#include "Enemy.h"
#include <assert.h>
#include "SoundManager.h"
#include "../GameData.h"



PlayScene::PlayScene()
	:SceneBase(), hImage_(0)
{
	new Stage();
	hImage_ = LoadGraph("data/image/Stage.jpeg");
	
	g_soundManager = new SoundManager();
    g_soundManager->Play(SoundType::BackGround);
    g_time = 0.0f;
    prevTime_ = GetNowCount();

}

PlayScene::~PlayScene()
{

}

void PlayScene::Update()
{
	// 経過時間計測
    int now = GetNowCount();
    float delta = (now - prevTime_) / 1000.0f; // 秒
    prevTime_ = now;

    g_time += delta;


    // BGM再生(ループ)
    g_soundManager->PlayBGM(SoundType::BackGround);

    Player* player = FindGameObject<Player>();
    Enemy* enemy = FindGameObject<Enemy>();


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
	float fSize = SetFontSize(32);    
    DrawFormatString(980, 10, GetColor(0, 0, 0), "経過時間: %.2f秒", g_time  , fSize);
	//DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	//DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
    
}



//コードメモリー　 Code Memory
//スタックメモリー Stack Memory
//ヒープメモリー   Heap Memory
