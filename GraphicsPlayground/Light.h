#pragma once

#include "MathTypes.h"

struct DirectionalLight
{
	DirectionalLight(const Color4& arColor, const Float3& arDir) :
		Color(arColor), Dir(arDir) {};

	explicit DirectionalLight(const Color4& arColor) : Color(arColor) {};

	DirectionalLight() = default;

	Color4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Float3 Dir = { 0.0f, 0.0f, 1.0f};
};
