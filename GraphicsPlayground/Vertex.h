#pragma once
#include "MathTypes.h"

struct Vertex
{
	Vertex() 
		: Pos(0.0f, 0.0f, 0.0f)
		, Color(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	Vertex(const Float3& arPos, const Color4& arColor)
		: Pos(arPos)
		, Color(arColor)
	{};

	explicit Vertex(const Float3& arPos)
		: Pos(arPos)
		, Color(0.0f, 0.0f, 0.0f, 1.0f)
	{};

	Float3 Pos;
	Color4	Color;

	static const int ByteWidth = 7*sizeof(float);
};
