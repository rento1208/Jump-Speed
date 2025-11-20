#pragma once
#include <math.h>

struct VECTOR2
{
	float x;
	float y;
	
};

inline VECTOR2 operator +(VECTOR2 a, VECTOR2 b)
{
	VECTOR2 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

inline VECTOR2 operator -(VECTOR2 a, VECTOR2 b)
{
	VECTOR2 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}

inline VECTOR2 operator /(VECTOR2 a, float f)
{
	VECTOR2 c;
	c.x = a.x / f;
	c.y = a.y / f;
	return c;
}

inline VECTOR2 operator *(VECTOR2 a, float f)
{
	VECTOR2 c;
	c.x = a.x * f;
	c.y = a.y * f;
	return c;
}

inline float VSize(VECTOR2 v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}
