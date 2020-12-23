#pragma once
#include "MathTypes.h"

struct Vertex
{
	Vertex(const Float3& arPos = Float3S::Zero
		, const Color4& arColor = ColorS::Black
		, const Float2& arUV = Float2S::Zero
		, const Float3& arNormal = Float3S::Forward)
		: Pos(arPos)
		, Color(arColor)
		, UV(arUV)
		, Normal(arNormal)
	{};

	Float3 Pos;
	Color4 Color;
	Float2 UV;
	Float3 Normal;

	static const int ByteWidth = 12*sizeof(float);
};
