#include "Enemy.h"
#include "Stage.h"
#include <DxLib.h>
#include <assert.h>

Enemy::Enemy(const VECTOR2& pos)
{
    position = pos;

    State = ENEMY_PATROL;

    Speed = 1.5f;
    Gravity = 0.5f;
    vx = Speed;
    vy = 0.0f;
	anim = 0;
	animY = 0;
	frameX = 0;
	animTimer = 0.0f;
	animInterval = 0.1f;


    //敵画像サイズ
    imageSize = VECTOR2(64, 64);

    hImage = LoadGraph("data/image/Enemy.png");
    assert(hImage > 0);
}

Enemy::~Enemy()
{
    if (hImage > 0)
        DeleteGraph(hImage);
}

void Enemy::Update()
{
    Stage* st = FindGameObject<Stage>();
    if (!st) return;

    //左右移動
    position.x += vx;

    //壁に当たったら反転
    VECTOR2 right = position + VECTOR2(imageSize.x / 2, 0);
    VECTOR2 left = position - VECTOR2(imageSize.x / 2, 0);

    if (vx > 0 && st->IsWall(right))
        vx = -Speed;
    else if (vx < 0 && st->IsWall(left))
        vx = Speed;

    //重力
    vy += Gravity;
    position.y += vy;

    //床判定
    VECTOR2 foot = position + VECTOR2(0, imageSize.y / 2);
    int down = st->CheckDown(foot);
    if (down > 0)
    {
        position.y -= down;
        vy = 0;
    }
}


void Enemy::Draw()
{
    Stage* st = FindGameObject<Stage>();
    if (!st || hImage <= 0) return;

    int drawX = (int)(position.x - imageSize.x / 2 - st->ScrollX());
    int drawY = (int)(position.y - imageSize.y / 2);

    DrawRectGraph(drawX, drawY, imageSize.x, imageSize.y, 0, 0, imageSize.x, imageSize.y, hImage, FALSE);
    DrawGraph(drawX, drawY, hImage, TRUE);
}
