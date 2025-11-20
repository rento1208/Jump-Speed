#include "Item.h"
#include "DxLib.h"  // PlaySoundMem など
#include "SoundManager.h"



void Item::Apply(Player* player, Stage* stage, const VECTOR2& pos) {
	// アイテム効果を種類ごとに適用
   switch (type) {
	// ジャンプ力アップ
   case ItemType::JumpUp:
      g_soundManager->Play(SoundType::Poison);
      g_soundManager->Play(SoundType::Status);
       player->JumpHeight += 80.0f;
       player->JumpV0 = -sqrtf(2.0f * player->Gravity * player->JumpHeight);
       stage->RemoveChip(pos);
       break;
	   // スピードアップ
   case ItemType::SpeedUp:
      g_soundManager->Play(SoundType::Poison);
      g_soundManager->Play(SoundType::Status);
       player->moveSpeed += 0.5f;
       stage->RemoveChip(pos);
       break;
	   // 体力全回復
   case ItemType::FullRecover:
      g_soundManager->Play(SoundType::Poison);
       // ここに回復処理をまとめる
       stage->RemoveChip(pos);
       break;
	   // 重力アップ
   case ItemType::GravityUp:
      g_soundManager->Play(SoundType::Poison);
      g_soundManager->Play(SoundType::Status);
       player->Gravity += 3.0f;
       player->JumpV0 = -sqrtf(2.0f * player->Gravity * player->JumpHeight);
       stage->RemoveChip(pos);
       break;
	   // スコア獲得
   case ItemType::Score:
      g_soundManager->Play(SoundType::Coin);
       stage->RemoveChip(pos);
       g_score += 1;
       break;
	   // ゲームクリア
   case ItemType::GameClear:
      g_soundManager->Play(SoundType::Result);
       stage->RemoveChip(pos);
       break;
   }
}
