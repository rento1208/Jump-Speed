#pragma once
#include "Object2D.h"

class Enemy : public Object2D
{
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
};
