Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Uv : TEXCOORD;
	float3 Normal : NORMAL;
};

cbuffer LightBuffer : register(b0)
{
	float4 Diffuse;
	float3 Dir;
	float pad;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	textureColor = ObjTexture.Sample(ObjSamplerState, input.Uv);
	lightDir = Dir;
	lightDir.xy = -lightDir.xy;
	lightIntensity = saturate(dot(input.Normal, lightDir));

	color = Diffuse * 0.15f;
	if (lightIntensity > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		color += (Diffuse * lightIntensity * 2.0f);
	}

	//color = input.Color * textureColor
	color = saturate(color);
	color = color * textureColor;
	return color;
}