#pragma once
#include "MathTypes.h"
struct Vertex
{
	Vertex() 
		: Pos(0.0f, 0.0f, 0.0f)
		, Color(0.0f, 0.0f, 0.0f, 0.0f)
	{}

	Vertex(Vector3 aPos, Color4 aColor)
		: Pos(std::move(aPos))
		, Color(std::move(aColor))
	{};

	Vector3 Pos;
	Color4	Color;

	static const int ByteWidth = 7;
};
