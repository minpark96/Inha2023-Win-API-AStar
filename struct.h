#pragma once
#include "Types.h"

struct Vec2
{
	int32 x;
	int32 y;

public:
	bool operator==(Vec2& other)
	{
		return y == other.y && x == other.x;
	}

	bool operator!=(Vec2& other)
	{
		return !(*this == other);
	}

	bool operator<(const Vec2& other) const
	{
		if (y != other.y)
			return y < other.y;
		return x < other.x;
	}

	Vec2& operator = (POINT _pt)
	{
		Vec2 ret;
		x = _pt.x;
		y = _pt.y;
		return ret;
	}

	Vec2& operator + (Vec2& other)
	{
		Vec2 ret;
		ret.y = y + other.y;
		ret.x = x + other.x;
		return ret;
	}

	Vec2& operator+=(Vec2& other)
	{
		y += other.y;
		x += other.x;
		return *this;
	}

public:
	Vec2()
		: x(0)
		, y(0)
	{}

	Vec2(float _x, float _y)
		: x((int)_x)
		, y((int)_y)
	{}

	Vec2(int _x, int _y)
		: x(_x)
		, y(_y)
	{}

	Vec2(const POINT& _pt)
		: x(_pt.x)
		, y(_pt.y)
	{}
};

struct PQNode
{
	bool operator<(const PQNode& other) const { return f < other.f; }
	bool operator>(const PQNode& other) const { return f > other.f; }

	int32	f; // f = g + h
	int32	g;
	Vec2	pos;
};
