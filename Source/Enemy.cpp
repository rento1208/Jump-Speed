#include "Enemy.h"
#include "Stage.h"
#include "Player.h"
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
    position.x -= vx;

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

    if (State == ENEMY_DEAD)
        return;
	
    //プレイヤーとの当たり判定
    Player* pl = FindGameObject<Player>();
    if (!pl || State == ENEMY_DEAD)
        return;

    Rect er = GetRect();
    Rect pr = pl->GetRect();

    if (!Intersect(er, pr))
        return;

    VECTOR2 ppos = pl->GetPosition();

    //プレイヤーの足元Y
    float playerFootY = pr.bottom;

    //敵の頭Y
    float enemyHeadY = er.top;

    //少し余裕を持たせる
    const float stompMargin = 6.0f;

    //上から踏んだか
    bool stomp =
        playerFootY <= enemyHeadY + stompMargin &&
        pl->GetvelocityY() > 0;   // 落下中のみ

    if (stomp)
    {
        // 踏み
        if (pl->GetAttackPower() >= 1)
        {
            State = ENEMY_DEAD;
			pl->AttackPower -= 1;
        }
        else
        {
            // 攻撃力0で敵と接触→ 死
			SceneManager::ChangeScene("GAMEOVER");
        }
    }
    else
    {
        //横・下・斜め
        if (pl->GetAttackPower() >= 1)
        {
            State = ENEMY_DEAD;
            pl->AttackPower -= 1;
        }
        else
        {
			SceneManager::ChangeScene("GAMEOVER");
        }
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

    if (State == ENEMY_DEAD) return;
}

Rect Enemy::GetRect() const
{
   return MakeRect(position, imageSize);
}

bool Enemy::IsDead() const
{
    return State == ENEMY_DEAD;
}
