#pragma once
struct Vertex
{
	Vertex() = default;
	Vertex(float ax, float ay, float az)
		: x(ax)
		, y(ay)
		, z(az)
	{};

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;
};
