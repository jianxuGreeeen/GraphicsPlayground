#pragma once
#include "MathTypes.h"

struct Camera
{
	static constexpr float DefaultFov = 45.0f;
	static constexpr float DefaultNear = 0.1f;
	static constexpr float DefaultFar = 1000.0f;

	Vector EyePos;
	Vector FocusPos;
	float Fov = DefaultFov;
	float Near = DefaultNear;
	float Far = DefaultFar;
};

