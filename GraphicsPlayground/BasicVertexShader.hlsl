struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Color : COLOR;
    float4 Pos : SV_POSITION;
};

cbuffer PerApplication : register(b0)
{
    matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
    matrix viewMatrix;
}

cbuffer PerObject : register(b2)
{
    matrix worldMatrix;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    output.Pos = mul(mvp, float4(input.Pos, 1.0f));
    output.Color = input.Color;

    return output;
}