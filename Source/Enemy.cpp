#include "Enemy.h"
#include "Stage.h"
#include <DxLib.h>
#include <cmath>
#include "CsvReader.h"
#include <assert.h>

// ------------------------------
// 共通初期化（画像読み込み・アニメ設定）
// ------------------------------
void Enemy::InitializeCommon()
{
    state = ENEMY_PATROL;
    patrolSpeed = 2.0f;
    chaseSpeed = 3.0f;
    visionRange = 200.0f;
    facingRight = true;

    Gravity = 0.5f;
    JumpHeight = 20.0f;
    JumpV0 = 8.0f;
    moveSpeed = 2.0f;

    playerX = 0;
    playerY = 0;

    imageSize = { 32,32 };

    anim = 0;
    animY = 0;
    animTimer = 0.0f;
    animInterval = 0.15f;
    frameXCount = 4;

    // ★ ここが重要：どのコンストラクタでも必ず画像をロードする
    hImage = LoadGraph("data/image/Parts1.png");
    assert(hImage > 0);
}

// ------------------------------
// (1) デフォルト
// ------------------------------
Enemy::Enemy()
{
    position = { 0,0 };
    InitializeCommon();
}

// ------------------------------
// (2) float px, py 版（位置だけセット）
// ------------------------------
Enemy::Enemy(float px, float py)
{
    position.x = px;
    position.y = py;
    InitializeCommon();
}

// ------------------------------
// (3) CSV 用コンストラクタ
// pos = 初期位置
// px, py = 未使用なので無視 (あなたのコード保持)
// ------------------------------
Enemy::Enemy(VECTOR2 pos, float px, float py)
{
    position = pos;   // ★ ← 必ず位置をセット

    InitializeCommon();

    // パラメータ上書き
    CsvReader csv("data/EnemyParam.csv");
    for (int i = 0; i < csv.GetLines(); i++) {
        std::string tag = csv.GetString(i, 0);
        if (tag == "Gravity") Gravity = csv.GetFloat(i, 1);
        else if (tag == "JumpHeight") JumpHeight = csv.GetFloat(i, 1);
        else if (tag == "MoveSpeed") moveSpeed = csv.GetFloat(i, 1);
    }

    JumpV0 = -sqrtf(2.0f * Gravity * JumpHeight);
}

Enemy::~Enemy()
{
    if (hImage > 0)
        DeleteGraph(hImage);
}

void Enemy::SetPlayerPosition(float px, float py)
{
    playerX = px;
    playerY = py;
}

bool Enemy::CanSeePlayer()
{
    float dx = playerX - position.x;

    if (fabs(dx) > visionRange) return false;

    if (dx > 0 && !facingRight) return false;
    if (dx < 0 && facingRight)  return false;

    return true;
}

void Enemy::Flip()
{
    facingRight = !facingRight;
}

void Enemy::Update()
{
    Stage* st = FindGameObject<Stage>();
    const float dt = 1.0f / 60.0f;

    float dx = playerX - position.x;
    float dist = fabs(dx);

    switch (state)
    {
    case ENEMY_PATROL:
        position.x += patrolSpeed * (facingRight ? 1 : -1);

        if (position.x < 50 || position.x > 750)
            Flip();

        if (CanSeePlayer())
            state = ENEMY_CHASE;
        break;

    case ENEMY_CHASE:
        position.x += chaseSpeed * (dx > 0 ? 1 : -1);

        if (dx > 0 && !facingRight) Flip();
        if (dx < 0 && facingRight)  Flip();

        if (!CanSeePlayer())
            state = ENEMY_PATROL;
        break;

    case ENEMY_HURT:
    case ENEMY_DEAD:
        break;
    }

    animTimer += dt;
    if (animTimer >= animInterval)
    {
        animTimer = 0.0f;
        anim++;
        if (anim >= frameXCount)
            anim = 0;
    }
}

void Enemy::Draw()
{
    if (hImage <= 0) return;

    int sx = anim * imageSize.x;
    int sy = animY * imageSize.y;

    int drawX = position.x - imageSize.x / 2;
    int drawY = position.y - imageSize.y / 2;

    Stage* st = FindGameObject<Stage>();
    if (st != nullptr)
        drawX -= st->ScrollX();

    if (facingRight)
    {
        DrawRectGraph(drawX, drawY,
            sx, sy, imageSize.x, imageSize.y,
            hImage, TRUE);
    }
    else
    {
        DrawRectGraph(drawX + imageSize.x, drawY,
            sx, sy, imageSize.x, imageSize.y,
            hImage, TRUE, TRUE);
    }
}
