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
	moveRange = 100.0f;      // 左右に動く範囲
	originX = pos.x;       // 初期X座標（基準点）
	moveDir = 1;              // 移動方向（1:右, -1:左）


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
    if (State == ENEMY_DEAD)
        return;

    Stage* st = FindGameObject<Stage>();
    Player* pl = FindGameObject<Player>();
    if (!st || !pl) return;
  

    VECTOR2 right = position + VECTOR2(imageSize.x / 2, 0);
    VECTOR2 left = position - VECTOR2(imageSize.x / 2, 0);

    if (vx > 0 && st->IsWall(right))
        vx = -Speed;
    else if (vx < 0 && st->IsWall(left))
        vx = Speed;

    position.x += Speed * moveDir;
    
    VECTOR2 frontFoot;
    frontFoot.x = position.x + (imageSize.x / 2 + 2) * moveDir;
    frontFoot.y = position.y + imageSize.y / 2 + 2;
  
    int frontDown = st->CheckDown(frontFoot);
    if (frontDown <= 0)
    {
        moveDir *= -1;
    }
 

    //重力
    vy += Gravity;
    position.y += vy;

    VECTOR2 foot = position + VECTOR2(0, imageSize.y / 2);
    int down = st->CheckDown(foot);
    if (down > 0)
    {
        position.y -= down;
        vy = 0;
    }

    //当たり判定
    Rect er = GetRect();
    Rect pr = pl->GetRect();

    if (!Intersect(er, pr))
        return;

    float playerFootY = pr.bottom;
    float enemyHeadY = er.top;

    const float stompMargin = 6.0f;

    bool stomp =
        playerFootY <= enemyHeadY + stompMargin &&
        pl->GetvelocityY() > 0;

    if (stomp)
    {
        //踏みつけ
        pl->Bounce();   //踏み反発

        if (pl->GetAttackCount() >= 1)
        {
            State = ENEMY_DEAD;
            pl->ConsumeAttackCount(1);
        }
        else
        {
            pl->Damage(30);
        }
    }
	//踏みつけ以外の当たり方
    else
    {
        if (pl->GetAttackCount() >= 1)
        {
            State = ENEMY_DEAD;
            pl->ConsumeAttackCount(1);
        }
        else
        {
            pl->Damage(30);
        }
    }
}

void Enemy::Draw()
{

    if (State == ENEMY_DEAD) return;

    Stage* st = FindGameObject<Stage>();
    if (!st || hImage <= 0) return;

    int drawX = (int)(position.x - imageSize.x / 2 - st->ScrollX());
    int drawY = (int)(position.y - imageSize.y / 2);

    DrawGraph(drawX, drawY, hImage, TRUE);
}

Rect Enemy::GetRect() const
{
   return MakeRect(position, imageSize);
}

bool Enemy::IsDead() const
{
    return State == ENEMY_DEAD;
}
