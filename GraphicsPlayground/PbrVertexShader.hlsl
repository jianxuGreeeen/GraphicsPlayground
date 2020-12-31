struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 Uv : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 Uv : TEXCOORD;
    float3 Normal : NORMAL;
};

cbuffer PerObject : register(b0)
{
    matrix WVP;
    matrix World;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.Pos = mul(float4(input.Pos, 1.0f), WVP);
    output.WorldPos = mul(float4(input.Pos, 1.0f), World);
    output.Uv = input.Uv;
    output.Color = input.Color;
    output.Normal = mul(float4(input.Normal, 1.0f), (float3x3)World).xyz;

    return output;
}