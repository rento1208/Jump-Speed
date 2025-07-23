#pragma once
#include "Object2D.h"
#include <vector>

class TitleStage : public Object2D {
public:
	TitleStage();
	~TitleStage();
	void Draw()override;
	float ScrollX() { return scrollX; }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sc">  </param>
	void SetScrollX(int sc) { scrollX = sc; }
	
private:
	float scrollX; // â°ÉXÉNÉçÅ[Éãó 
	bool IsWall(VECTOR2 pos);
    std::vector<std::vector<int>> map;
	int hImage;

};