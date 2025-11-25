// SoundManager.h  
#pragma once  
#include "./Object2D.h"  
#include <map>  
#include <string>  
#include "DxLib.h"  

enum class SoundType {  
   Jump,  
   Coin,  
   Status,  
   Result,  
   Poison ,
   BackGround,
   GameOver
};  

class SoundManager : public Object2D {  
public:  
   SoundManager();  
   ~SoundManager();  

   // サウンドの読み込み  
   void LoadSound(SoundType type, const std::string& path);  

   // サウンド再生  
   void Play(SoundType type);  

   // BGM再生
   void PlayBGM(SoundType type);

   //サウンド停止
   void StopSound(SoundType type);

private:  
   std::map<SoundType, int> sounds;  
   int handle;
   int volume;
};

// グローバル変数宣言
extern SoundManager* g_soundManager;
