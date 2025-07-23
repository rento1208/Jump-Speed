#pragma once
#include "Object2D.h"

class Goal : public Object2D {
public:
    Goal(VECTOR2 pos);
    void Update() override;
    void Draw() override;

private:
    int hImage;
    VECTOR2 position;
    VECTOR2 size;
};
