#pragma once
#include "Object2D.h"
#include "csvReader.h"
#include <string>
#include <vector>
#include <map>

enum State
{
    ENEMY_PATROL,
    ENEMY_CHASE,
    ENEMY_ATTACK,
    ENEMY_HURT,
    ENEMY_DEAD
};

struct AnimFrame
{
    int x, y, w, h;  // スプライトシート上の矩形
    float time;      // 1フレームの表示時間
};

class Enemy : public Object2D
{
public:
    Enemy(float x = 0.0f, float y = 0.0f);
    ~Enemy();

    void Update(float dt, float playerX, float playerY);
    void Draw() override;
    bool CanSeePlayer(float playerX, float playerY);
    void Flip();

    // Enemyの物理・ステータス
    float x, y;
    float vx;
    bool facingRight;
    float hp;

    float patrolSpeed;
    float chaseSpeed;
    float visionRange;   // プレイヤー発見範囲
    float attackRange;   // 攻撃距離
    float attackCooldown;
    float attackTimer;

    // CSV管理のアニメーション
    void LoadAnimationCSV(const std::string& csvFile, const std::string& spritePath);
    void SetState(State s);

private:
    State state_;
    int img_; // スプライトシート画像

    // アニメーション
    std::map<State, std::vector<AnimFrame>> anims_;
    State currentAnimState_;
    int animIndex_;      // 現在のフレームインデックス
    float animTimer_;    // フレーム時間管理
};
