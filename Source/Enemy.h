#pragma once
#include "Object2D.h"

enum EnemyState
{
    ENEMY_PATROL,
    ENEMY_DEAD
};

class Enemy : public Object2D
{
public:
    Enemy(const VECTOR2& pos);
    ~Enemy();

    void Update() override;
    void Draw() override;
    Rect GetRect() const;
    bool IsDead() const;

private:
    EnemyState State;

    float vx;
    float vy;
    float Speed;
    float Gravity;
    int anim;
    int animY;
    int frameX;
    float animTimer;
    float animInterval;
 
};
