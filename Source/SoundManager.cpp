// SoundManager.cpp
#include "SoundManager.h"

// グローバル変数の定義
SoundManager* g_soundManager = nullptr;


SoundManager::SoundManager()
{
    sounds[SoundType::Jump] = LoadSoundMem("data/sound/jump.mp3");
    sounds[SoundType::Coin] = LoadSoundMem("data/sound/coin.mp3");
    sounds[SoundType::Status] = LoadSoundMem("data/sound/status.mp3");
    sounds[SoundType::Result] = LoadSoundMem("data/sound/result.mp3");
    sounds[SoundType::Poison] = LoadSoundMem("data/sound/poison.mp3");
}

SoundManager::~SoundManager() {
    for (auto& pair : sounds) {
        DeleteSoundMem(pair.second);
    }
}

void SoundManager::LoadSound(SoundType type, const std::string& path) {
    int h = LoadSoundMem(path.c_str());
    if (h <= 0) {
        // ロード失敗時のエラーハンドリング
    }
    sounds[type] = h;
}

void SoundManager::Play(SoundType type) {
    if (sounds.find(type) != sounds.end()) {
        PlaySoundMem(sounds[type], DX_PLAYTYPE_BACK);
    }
}
