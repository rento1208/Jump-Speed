#include "Goal.h"
#include "Player.h"
#include "Stage.h"
#include <DxLib.h>

Goal::Goal(VECTOR2 pos) {
    position = pos;
    size = VECTOR2(128, 160);
    hImage = LoadGraph("data/image/goal_door.png"); // ゴール画像
}

void Goal::Update() {
    Player* p = FindGameObject<Player>();
    if (!p) return;

    float dx = std::abs(p->GetPosition().x - position.x);
    float dy = std::abs(p->GetPosition().y - position.y);

    if (dx < size.x / 2 && dy < size.y / 2) {
        // ゴール到達処理
        DrawString(300, 200, "クリア！！", GetColor(255, 255, 0));
        // ゲーム状態を管理してもOK（例：isGameClear = true）
    }
}

void Goal::Draw() {
    Stage* st = FindGameObject<Stage>();
    float drawX = position.x - st->ScrollX();
    float drawY = position.y - size.y / 2;  // ← 中央表示に修正

    DrawGraph(
        (int)(drawX - size.x / 2),
        (int)(drawY),
        hImage, TRUE
    );
}
