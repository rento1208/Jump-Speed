#pragma once
#include "Player.h"
#include "Stage.h"

enum class ItemType {
    JumpUp,       // チップ17
    SpeedUp,      // チップ14
    Ricaver,  // チップ15
    GravityUp,    // チップ16
    Score,        // チップ11
    GameClear,     // チップ10
    Sword       //チップ12

};

class Item {
public:
    Item(ItemType type) : type(type) {}
	~Item() {}
    void Apply(Player* player, Stage* stage, const VECTOR2& pos);

private:
    ItemType type;
};