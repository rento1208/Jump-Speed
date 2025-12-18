#include "Stage.h"
#include <assert.h>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "CsvReader.h"



Stage::Stage()
{
   // ステージデータの読み込み
	CsvReader* csv = new CsvReader("data/map/stage0.csv");
	for (int line = 0; line < csv->GetLines(); line++) {
		std::vector<int> mapLine;
		for (int column = 0; column < csv->GetColumns(line); column++) {
			int c = csv->GetInt(line, column);
			mapLine.push_back(c);
		}
		map.push_back(mapLine);
	}
	delete csv;

	mapWidth = map[0].size();
	mapHeight = map.size();

	hImage = LoadGraph("data/image/parts1.png");
	assert(hImage > 0);
	imageSize = VECTOR2(64, 64);
	anim = 3;
	animY = 1;

	hItemImage = LoadGraph("data/image/item.png");
	assert(hItemImage > 0);
	itemImageSize = VECTOR2(64, 64);
	anim = 10;
	animY = 16;
	

	scrollX = 0;
	
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[y].size(); x++) {
			int c = map[y][x];
			if (c == 9) {
				int px = x * imageSize.x + imageSize.x / 2.0f;
				int py = y * imageSize.y + imageSize.y / 2.0f;
				new Player(VECTOR2(px, py));
			}
		}
	}
		
	for (int y = 0; y < map.size(); y++){
		for (int x = 0; x < map[y].size(); x++){
			int d = map[y][x];
			if (d == 8) {
				int px = x * imageSize.x + imageSize.x / 2;
				int py = y * imageSize.y + imageSize.y / 2;

				Enemy* e = new Enemy(VECTOR2(px, py));
				enemies.push_back(e);

				map[y][x] = 0; // 出現後は床
			}
		}
	}
}


Stage::~Stage()
{
}

void Stage::Update()
{
	for (auto it = enemies.begin(); it != enemies.end(); )
	{
		if ((*it)->IsDead())
		{
			delete* it;
			*it = nullptr;
			it = enemies.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Stage::Draw()
{
	int w = imageSize.x;
	int h = imageSize.y;

	
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[y].size(); x++) {
			int c = map[y][x];
			if (c == 1) {
				DrawRectGraph(x * w - scrollX, y * h, 3 * w, 1 * h, w, h, hImage, TRUE);
			}
			else if (c == 2) {
				DrawRectGraph(x * w - scrollX, y * h, 0 * w, 1 * h, w, h, hImage, TRUE);
			}
			else if (c >= 10 && c <= 21) {
				int chipIndex = c - 10;
				int col = chipIndex % 4;
				int row = chipIndex / 4;
				DrawRectGraph(x * w - scrollX, y * h, col * w, row * h, w, h,hItemImage, TRUE);
			}
			
		}
	}

	for (Enemy* e : enemies) {
		e->Draw();
	}
}

int Stage::CheckRight(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int x = pos.x / imageSize.x;
	int dx = pos.x - x * imageSize.x; // チップの中の座標
	return dx+1;
}

int Stage::CheckLeft(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int x = pos.x / imageSize.x;
	int dx = pos.x - x * imageSize.x; // チップの中の座標
	return imageSize.x - dx;
}

int Stage::CheckDown(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int y = pos.y / imageSize.y;
	int dy = pos.y - y * imageSize.y; // チップの中の座標
	return dy + 1;
}

int Stage::CheckUp(VECTOR2 pos)
{
	if (IsWall(pos) == false) {
		return 0;
	}
	// チップにどれぐらいめり込んでいるかを返す
	int y = pos.y / imageSize.y;
	int dy = pos.y - y * imageSize.y; // チップの中の座標
	return imageSize.y - dy;
}

int Stage::GetChip(const VECTOR2& worldPos)
{
	const int chipSize = 64;

	int x = (int)(worldPos.x) / chipSize;
	int y = (int)(worldPos.y) / chipSize;

	if (y >= 0 && y < (int)map.size() && x >= 0 && x < (int)map[y].size()) {
		return map[y][x];
	}

	return -1; // 範囲外
}

void Stage::RemoveChip(const VECTOR2& worldPos)
{
	const int chipSize = 64;

	int x = (int)(worldPos.x) / chipSize;
	int y = (int)(worldPos.y) / chipSize;

	if (y >= 0 && y < (int)map.size() && x >= 0 && x < (int)map[y].size()) {
		map[y][x] = 0; // チップ番号を0にして消去扱い
	}
}



bool Stage::IsWall(const VECTOR2& pos) const
{
	// チップの場所を特定する
	int x = pos.x / imageSize.x;
	int y = pos.y / imageSize.y;
	if (y < 0 || y >= map.size()) {
		return false;
	}
	if (x < 0 || x >= map[y].size()) {
		return false;
	}

	int c = map[y][x];
	// チップの番号を見て、壁かどうか確定する
	if (c == 0) return false;        // 空
	if (c == 8) return false;        // 敵
	if (c == 9) return false;        // プレイヤー
	if (c >= 10 && c <= 21) return false; //アイテム・コイン

	return true;
	
}
