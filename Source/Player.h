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
	
protected:
	
	float velocityY;
	bool onGround;
	bool prevPushed;
	

	float Gravity;
	float JumpHeight;
	float JumpV0;
	float moveSpeed;
	float animTimer;         // アニメーション時間用タイマー
	float animInterval;      // フレームの切り替え間隔（秒）
	int prevTime = GetNowCount();
	

	int hSound2;
	int hSound3;
	int hSound4;
	int hSound5;
	
   
};