#pragma once

#include <DirectXMath.h>
using Float2 = DirectX::XMFLOAT2;
using Float3 = DirectX::XMFLOAT3;
using Float4 = DirectX::XMFLOAT4;
using Vector = DirectX::XMVECTOR;
using Color4 = DirectX::XMFLOAT4;
using Matrix = DirectX::XMMATRIX;

namespace MatrixS
{
	static const Matrix Identity = DirectX::XMMatrixIdentity();
};

namespace ColorS
{
	static constexpr Color4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	static constexpr Color4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
};

namespace Float3S
{
	static constexpr Float3 Forward = { 0.0f, 0.0f, 1.0f};
	static constexpr Float3 Zero = { 0.0f, 0.0f, 0.0f };
};

namespace Float2S
{
	static constexpr Float2 Zero = { 0.0f, 0.0f };
};