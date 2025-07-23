#include "Enemy.h"
#include "Stage.h"
#include <DxLib.h>
#include "../ImGui/imgui.h"
#include "CsvReader.h"
#include <assert.h>

Enemy::Enemy(VECTOR2 pos) {
    hImage = LoadGraph("data/image/enemy.png");
    assert(hImage > 0);

    position = pos;
    imageSize = VECTOR2(128, 160);        
    colliderSize = VECTOR2(64, 160);    

    hp = maxHp = 50;
    direction = -1;
    moveSpeed = 2.0f;
    knockbackTimer = 0;
    knockbackVelocity = VECTOR2(2, 0);
    velocityY = 0.0f;

}

void Enemy::Update() {
    if (hp <= 0) {
        DestroyMe();
        return;
    }

    Stage* st = FindGameObject<Stage>();
    float halfW = colliderSize.x / 2.0f;
    float halfH = colliderSize.y / 2.0f;

    if (knockbackTimer > 0) {
        position.x += knockbackVelocity.x;
        position.y += knockbackVelocity.y;
        knockbackTimer--;
    }
    else {
        // 水平方向の移動（右なら +、左なら -）
        position.x += direction * moveSpeed;

        // 右向きの時、右端の上下で壁に当たっていたら反転
        if (direction > 0) {
            int pushTop = st->CheckRight(VECTOR2(position.x + halfW, position.y - halfH));
            int pushBottom = st->CheckRight(VECTOR2(position.x + halfW, position.y + halfH));
            if (pushTop > 0 || pushBottom > 0) {
                position.x -= moveSpeed; // 移動を取り消す
                direction *= -1;         // 向きを反転
            }
        }

        // 左向きの時、左端の上下で壁に当たっていたら反転
        else {
            int pushTop = st->CheckLeft(VECTOR2(position.x - halfW, position.y - halfH));
            int pushBottom = st->CheckLeft(VECTOR2(position.x - halfW, position.y + halfH));
            if (pushTop > 0 || pushBottom > 0) {
                position.x += moveSpeed; // 移動を取り消す
                direction *= -1;         // 向きを反転
            }
        }

        // 地面に立っているか確認（Y方向の当たり判定）
        if (velocityY < 0.0f) {
            int push = st->CheckUp(VECTOR2(position.x - halfW, position.y - halfH));
            if (push > 0) {
                velocityY = 0.0f;
                position.y += push;
            }
            push = st->CheckUp(VECTOR2(position.x + halfW, position.y - halfH));
            if (push > 0) {
                velocityY = 0.0f;
                position.y += push;
            }
            push = st->CheckUp(VECTOR2(position.x, position.y - halfH));
            if (push > 0) {
                velocityY = 0.0f;
                position.y += push;
            }
        }
        else {
            int push = st->CheckDown(VECTOR2(position.x - halfW, position.y + halfH + 1));
            if (push > 0) {
                velocityY = 0.0f;
                onGround = true;
                position.y -= push - 1;
            }
            push = st->CheckDown(VECTOR2(position.x + halfW, position.y + halfH + 1));
            if (push > 0) {
                velocityY = 0.0f;
                onGround = true;
                position.y -= push - 1;
            }
            push = st->CheckDown(VECTOR2(position.x, position.y + halfH + 1));
            if (push > 0) {
                velocityY = 0.0f;
                onGround = true;
                position.y -= push - 1;
            }
        }
    }
}

void Enemy::Draw() {
    Stage* st = FindGameObject<Stage>();
    float x = position.x - st->ScrollX();
    float y = position.y;

    // 敵の画像を描画
    DrawGraph((int)(x - imageSize.x / 2), (int)(y - imageSize.y), hImage, TRUE);

    int barWidth = 48;
    int barHeight = 6;
    int left = (int)(x - barWidth / 2);
    int top = (int)(y - imageSize.y - 10);
    int right = (int)(x + barWidth / 2);
    int bottom = top + barHeight;

    // 背景（黒）
    DrawBox(left, top, right, bottom, GetColor(0, 0, 0), TRUE);

    // HP割合
    float hpRatio = (float)hp / maxHp;
    int hpRight = left + (int)(barWidth * hpRatio);

    // 色変更（緑 → 黄 → 赤）
    int color;
    if (hpRatio <= 0.1f) color = GetColor(255, 0, 0);
    else if (hpRatio <= 0.5f) color = GetColor(255, 255, 0);
    else color = GetColor(0, 255, 0);

    DrawBox(left, top, hpRight, bottom, color, TRUE);



    DrawBox((int)(x - colliderSize.x / 2), (int)(y - colliderSize.y), (int)(x + colliderSize.x / 2), (int)(y), GetColor(255, 0, 0), FALSE); // 赤い枠線
}
void Enemy::TakeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
    printf("Enemy took %d damage, hp = %d\n", damage, hp); 
}

VECTOR2 Enemy::GetPosition() const {
    return position;
}

VECTOR2 Enemy::GetColliderSize() const {
    return colliderSize;
}
