#pragma once
#include "Object2D.h"
#include <vector>
#include <map>

enum EnemyState
{
    ENEMY_PATROL,
    ENEMY_CHASE,
    ENEMY_HURT,
    ENEMY_DEAD
};

class Enemy : public Object2D
{
public:
    Enemy();                          // デフォルト
    Enemy(float px, float py);        // 位置だけ指定
    Enemy(VECTOR2 pos, float px, float py); // CSV 用 (pos を使用)

    virtual ~Enemy();

    virtual void Update() override;
    virtual void Draw() override;

    void SetPlayerPosition(float px, float py);

private:
    // 共通初期化（画像ロード・アニメ設定）
    void InitializeCommon();

    // ------ AI 関連 ------
    EnemyState state;     // 敵の状態
    float patrolSpeed;    // 巡回速度
    float chaseSpeed;     // 追跡速度
    float visionRange;    // 視界範囲
    bool facingRight;     // 向き

    // ------ 物理パラメータ ------
    float Gravity;        // 重力加速度
    float JumpHeight;     // ジャンプ高さ
    float JumpV0;         // 初速度
    float moveSpeed;      // 移動速度

    // ------ プレイヤー情報 ------
    float playerX;
    float playerY;

    // ------ アニメーション ------
    float animTimer;      // アニメ用タイマー
    float animInterval;   // フレーム切り替え間隔
    int frameXCount;      // 横に何枚あるか

    // ------ AI 動作 ------
    bool CanSeePlayer();  // プレイヤーが視界に入っているか
    void Flip();          // 向きを反転
};
