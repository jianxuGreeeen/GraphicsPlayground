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

struct PointLight
{
	PointLight(const Color4& arColor, const Float4& arPos) :
		Color(arColor), Pos(arPos) {};

	explicit PointLight(const Color4& arColor) : Color(arColor) {};

	PointLight() = default;

	Color4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Float4 Pos = { 0.0f, 0.0f, 1.0f, 1.0f }; // Range is the w component
	Float3 Attenuation = { 1.0f, 1.0f, 1.0f };
};

struct LightBufferData
{
	DirectionalLight DirLight = {
		Color4(.2f, .2f, .2f, 1.0f),
		Float3(1.0f, -1.0f, 0.0f)
	};
	float pad1 = 0.0f;

	PointLight PtLight1;
	float pad2 = 0.0f;
};
