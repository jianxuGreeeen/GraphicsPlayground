struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 Uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Color : COLOR;
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
};

cbuffer PerObject : register(b0)
{
    matrix WVP;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.Pos = mul(float4(input.Pos, 1.0f), WVP);
    output.Uv = input.Uv;
    output.Color = input.Color;

    return output;
}