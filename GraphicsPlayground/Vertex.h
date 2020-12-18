#pragma once
#include "MathTypes.h"
struct Vertex
{
	Vertex() 
		: Pos(0.0f, 0.0f, 0.0f)
		, Color(0.0f, 0.0f, 0.0f, 0.0f)
	{}

	Vertex(Float3 aPos, Color4 aColor)
		: Pos(std::move(aPos))
		, Color(std::move(aColor))
	{};

	Float3 Pos;
	Color4	Color;

	static const int ByteWidth = 7*sizeof(float);
};
