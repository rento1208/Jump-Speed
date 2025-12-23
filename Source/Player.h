#pragma once
#include "Object2D.h"

extern int g_score;

class Player : public Object2D {
public:
	Player();
	Player(VECTOR2 pos);
	~Player();
	void Update() override;
	void Draw() override;
	VECTOR2 GetPosition() const;
	void DrawScore();        
	Rect GetRect() const;
	int GetAttackCount() const;
	float GetvelocityY() const;
	bool IsInvincible() const;
	void Damage(int dmg);
	void ConsumeAttackCount(int v);
	void Bounce();
	void DrawGauge();


	


	float velocityY;         // Y方向の速度
	bool onGround;           // 地面に接地しているか
	bool prevPushed;         // 前フレームでジャンプボタンが押されていたか
	float Gravity;           // 重力加速度
	float JumpHeight;        // ジャンプ高さ
	float JumpV0;            // 初速度
	float moveSpeed;         // 移動速度
	float animTimer;         // アニメーション時間用タイマー
	float animInterval;      // フレームの切り替え間隔（秒）
	float AttackCount;       //攻撃回数
	int prevTime = GetNowCount();  // 前回の時間取得値
	int playerHP; 		 // プレイヤーの体力
	float invincibleTime;     // 残り無敵時間（秒）
	float invincibleMax;      // 無敵時間（秒）
	int timer; // ゲーム時間計測用タイマー
	int maxAttackCount;
	int hp;
	
};