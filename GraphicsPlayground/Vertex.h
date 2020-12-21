#pragma once
#include "MathTypes.h"

struct Vertex
{
	Vertex(const Float3& arPos, const Color4& arColor, const Float2& arUV)
		: Pos(arPos)
		, Color(arColor)
		, UV(arUV)
	{};

	Vertex() 
		: Vertex(Float3(0.0f, 0.0f, 0.0f), Color4(0.0f, 0.0f, 0.0f, 1.0f), Float2(0.0f, 0.0f))
	{}

	Vertex(const Float3& arPos, const Color4& arColor)
		: Vertex(arPos, arColor, Float2(0.0f, 0.0f))
	{};

	explicit Vertex(const Float3& arPos)
		: Vertex(arPos, Color4(0.0f, 0.0f, 0.0f, 1.0f), Float2(0.0f, 0.0f))
	{};

	Float3 Pos;
	Color4 Color;
	Float2 UV;

	static const int ByteWidth = 9*sizeof(float);
};
