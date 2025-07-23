#include "TitleStage.h"
#include <assert.h>
#include <vector>
#include "Player.h"
#include "CsvReader.h"

TitleStage::TitleStage()
{
	// ステージデータの読み込み
	CsvReader* csv = new CsvReader("data/map/stage1.csv");
	for (int line = 0; line < csv->GetLines(); line++) {
		std::vector<int> mapLine;
		for (int column = 0; column < csv->GetColumns(line); column++) {
			int c = csv->GetInt(line, column);
			mapLine.push_back(c);
		}
		map.push_back(mapLine);
	}
	delete csv;

	hImage = LoadGraph("data/image/parts1.png");
	assert(hImage > 0);
	imageSize = VECTOR2(64, 64);
	anim = 3;
	animY = 1;

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
}

TitleStage::~TitleStage()
{
}

void TitleStage::Draw()
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
		}
	}
}

