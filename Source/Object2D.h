#pragma once
#include "../Library/GameObject.h"

struct VECTOR2 {
	float x, y;
	VECTOR2() { x = 0; y = 0; }
	VECTOR2(float _x, float _y) { x = _x; y = _y; }
};
inline VECTOR2 operator -(VECTOR2 a, VECTOR2 b) {
	VECTOR2 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	return r;
}

inline VECTOR2 operator +(VECTOR2 a, VECTOR2 b) {
	VECTOR2 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	return r;
}

inline float VSize(VECTOR2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

struct Rect
{
	float left;
	float top;
	float right;
	float bottom;
};

inline Rect MakeRect(const VECTOR2& pos, const VECTOR2& size)
{
	return {
		pos.x - size.x / 2,
		pos.y - size.y / 2,
		pos.x + size.x / 2,
		pos.y + size.y / 2
	};
}

inline bool Intersect(const Rect& a, const Rect& b)
{
	return !(a.right < b.left ||
		a.left > b.right ||
		a.bottom < b.top ||
		a.top > b.bottom);
}

class Object2D : public GameObject {
public:
	Object2D();
	virtual ~Object2D();
	virtual void Update() override;
	virtual void Draw() override;
	const VECTOR2& GetPosition()const { return position; }
protected:
	int hImage; // ‰æ‘œ‚ğ“Ç‚Ş‚½‚ß
	int hSound;
	int anim; // ŠG‚ÌêŠ
	int animY; // ŠG‚ÌêŠ
	VECTOR2 position; // À•W
	VECTOR2 imageSize; // ‚P‚Â‚ÌŠG‚Ì‘å‚«‚³
	VECTOR2 itemImageSize;
};