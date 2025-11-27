#include "Enemy.h"
#include <DxLib.h>
#include <cassert>

Enemy::Enemy(float x, float y)
    : Object2D(),
    hp(3),
    patrolSpeed(2.0f),
    chaseSpeed(3.0f),
    visionRange(200.0f),
    attackRange(40.0f),
    attackCooldown(1.0f),
    attackTimer(0.0f),
    animIndex_(0),
    animTimer_(0.0f)
{
    x = 400.0f;
    y = 300.0f;
    facingRight = true;

    // デフォルト画像（CSV使う場合は上書きされる）
    img_ = LoadGraph("enemy.png");

    state_ = ENEMY_PATROL;
    currentAnimState_ = state_;
}

// Destructor
Enemy::~Enemy()
{
    // スプライト削除
    if (img_ != -1) {
        DeleteGraph(img_);
    }
}

// CSVからアニメーション読み込み
void Enemy::LoadAnimationCSV(const std::string& csvFile, const std::string& spritePath)
{
    CsvReader csv(csvFile);

    img_ = LoadGraph(spritePath.c_str());
    assert(img_ != -1);

    for (int i = 1; i < csv.GetLines(); i++) // 1行目はヘッダー
    {
        std::string stateStr = csv.GetString(i, 0);
        int x = csv.GetInt(i, 1);
        int y = csv.GetInt(i, 2);
        int w = csv.GetInt(i, 3);
        int h = csv.GetInt(i, 4);
        float time = csv.GetFloat(i, 5);

        AnimFrame f = { x, y, w, h, time };

        // CSVのState文字列をState enumに変換
        State s = ENEMY_PATROL;
        if (stateStr == "PATROL") s = ENEMY_PATROL;
        else if (stateStr == "CHASE") s = ENEMY_CHASE;
        else if (stateStr == "ATTACK") s = ENEMY_ATTACK;
        else if (stateStr == "HURT") s = ENEMY_HURT;
        else if (stateStr == "DEAD") s = ENEMY_DEAD;

        anims_[s].push_back(f);
    }
}

// 状態を変更（アニメも同期）
void Enemy::SetState(State s)
{
    if (state_ != s) {
        state_ = s;
        currentAnimState_ = s;
        animIndex_ = 0;
        animTimer_ = 0.0f;
    }
}

// Update 内で呼び出す
void Enemy::Update(float dt, float playerX, float playerY)
{
    attackTimer -= dt;

    float dx = playerX - x;
    float dist = abs(dx);

    // 状態ごとの動作
    switch (state_)
    {
    case ENEMY_PATROL:
        vx = patrolSpeed * (facingRight ? 1 : -1);
        x += vx;

        if (x < 50 || x > 750) Flip();
        break;

    case ENEMY_CHASE:
        vx = chaseSpeed * (dx > 0 ? 1 : -1);
        x += vx;

        if (dx > 0 && !facingRight) Flip();
        if (dx < 0 && facingRight)  Flip();
        break;

    case ENEMY_ATTACK:
        if (attackTimer <= 0.0f) {
            attackTimer = attackCooldown;
            // TODO: プレイヤーにダメージ処理
        }
        break;

    case ENEMY_HURT:
    case ENEMY_DEAD:
        vx = 0;
        break;
    }

    // 状態遷移
    bool canSee = CanSeePlayer(playerX, playerY);

    if (state_ != ENEMY_DEAD && state_ != ENEMY_HURT)
    {
        if (canSee && dist > attackRange)
            SetState(ENEMY_CHASE);
        else if (canSee && dist <= attackRange)
            SetState(ENEMY_ATTACK);
        else
            SetState(ENEMY_PATROL);
    }

    // アニメ更新
    if (anims_.count(currentAnimState_) > 0)
    {
        animTimer_ += dt;
        const AnimFrame& frame = anims_[currentAnimState_][animIndex_];

        if (animTimer_ >= frame.time)
        {
            animTimer_ = 0.0f;
            animIndex_++;
            if (animIndex_ >= anims_[currentAnimState_].size())
                animIndex_ = 0;
        }
    }
}

// 描画
void Enemy::Draw()
{
    if (anims_.count(currentAnimState_) == 0) {
        // CSVアニメ無ければ通常描画
        if (facingRight)
            DrawGraph((int)x, (int)y, img_, TRUE);
        else
            DrawTurnGraph((int)x, (int)y, img_, TRUE);
        return;
    }

    const AnimFrame& frame = anims_[currentAnimState_][animIndex_];

    if (facingRight)
    {
        DrawRectGraph((int)x, (int)y,
            frame.x, frame.y, frame.w, frame.h,
            img_, TRUE);
    }
    else
    {
        DrawRectRotaGraph2((int)x, (int)y,
            frame.w / 2, frame.h / 2,
            1.0f, 1.0f, 0.0f,
            img_, TRUE,
            frame.x, frame.y, frame.w, frame.h,
            TRUE);
    }
}

// プレイヤーを視認できるか
bool Enemy::CanSeePlayer(float playerX, float playerY)
{
    float dx = playerX - x;
    if (abs(dx) > visionRange) return false;

    if (dx > 0 && !facingRight) return false;
    if (dx < 0 && facingRight)  return false;

    return true;
}

// 向き反転
void Enemy::Flip()
{
    facingRight = !facingRight;
}
