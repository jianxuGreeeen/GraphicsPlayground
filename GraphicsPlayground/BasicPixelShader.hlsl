Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return ObjTexture.Sample(ObjSamplerState, input.Uv);
}