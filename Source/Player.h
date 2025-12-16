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

	float velocityY;         // Y方向の速度
	bool onGround;           // 地面に接地しているか
	bool prevPushed;         // 前フレームでジャンプボタンが押されていたか
	float Gravity;           // 重力加速度
	float JumpHeight;        // ジャンプ高さ
	float JumpV0;            // 初速度
	float moveSpeed;         // 移動速度
	float animTimer;         // アニメーション時間用タイマー
	float animInterval;      // フレームの切り替え間隔（秒）
	float AttackPower;       //攻撃回数
	float DiffencePower;    // 防御回数
	int prevTime = GetNowCount();  // 前回の時間取得値
};