#pragma once
#include "Object2D.h"
#include "Enemy.h"
#include <vector>

class Stage : public Object2D {
public:
	Stage();
	~Stage();
	void Draw() override;
	float ScrollX() { return scrollX; }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sc">  </param>
	void SetScrollX(int sc) { scrollX = sc; }

	/// <summary>
	/// 指定した点が当たっているか調べる
	/// </summary>
	/// <param name="pos">調べる点</param>
	/// <returns>当たっていれば、左に押し返す量</returns>
	int CheckRight(VECTOR2 pos);

	int CheckLeft(VECTOR2 pos);

	int CheckDown(VECTOR2 pos);

	int CheckUp(VECTOR2 pos);

	int GetChip(const VECTOR2& worldPos); // 指定位置のチップ番号を返す

	void RemoveChip(const VECTOR2& worldPos); // 指定位置のチップを削除（例：0にする）

	bool IsWall(const VECTOR2& pos) const;

private:
	float scrollX; // 横スクロール量

	
	std::vector<std::vector<int>> map;

	int hItemImage;

	int mapWidth;  // マップ横サイズ（列数）

	int mapHeight; // マップ縦サイズ（行数）
	
	std::vector<Enemy*> enemies;


};

