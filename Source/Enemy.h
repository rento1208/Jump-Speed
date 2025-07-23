#pragma once
#include "Object2D.h"

class Enemy : public Object2D {
public:
    Enemy(VECTOR2 pos);

    void Update() override;
    void Draw() override;

    void TakeDamage(int damage);
    VECTOR2 GetPosition() const;
    VECTOR2 GetColliderSize() const;

private:
    int hImage;
    VECTOR2 imageSize;
    VECTOR2 position;
    VECTOR2 colliderSize;
    int hp;
    int maxHp;
    int direction;
    float moveSpeed;
    float velocityY;
    float grabity = 1.0f;
    int knockbackTimer;
    VECTOR2 knockbackVelocity;
    bool isDead;
    bool onGround=false;
};
