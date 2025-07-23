#pragma once
#include "Object2D.h"

extern int g_score;

class Item : public Object2D
{
public:
	Item();
	~Item();
	void Update();
	void Draw();
	
private:
	
};