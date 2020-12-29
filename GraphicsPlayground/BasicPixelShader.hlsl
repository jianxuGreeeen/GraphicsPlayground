Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct PointLight
{
	float4 Diffuse;
	float3 Pos;
	float Range;
	float3 Att;
	float pad;
};

struct DirectionalLight
{
	float4 Diffuse;
	float3 Dir;
	float pad;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 WorldPos : POSITION;
	float4 Color : COLOR;
	float2 Uv : TEXCOORD;
	float3 Normal : NORMAL;
};

cbuffer LightBuffer : register(b0)
{
	DirectionalLight DirLight;
	PointLight PtLight1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 textureColor = ObjTexture.Sample(ObjSamplerState, input.Uv);	

	float3 lightDir = DirLight.Dir;
	lightDir.xy = -lightDir.xy;
	float lightIntensity = saturate(dot(input.Normal, lightDir));

	float4 dirLightColor = DirLight.Diffuse * 0.15f;
	if (lightIntensity > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		dirLightColor += (DirLight.Diffuse * lightIntensity * 2.0f);
	}
	dirLightColor = saturate(dirLightColor);
	dirLightColor = dirLightColor * textureColor;

	float4 ptLightColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 lightToPixel = PtLight1.Pos - input.WorldPos;
	float d = length(lightToPixel);
	if (d > PtLight1.Range)
	{
		return dirLightColor;
	}
	lightToPixel /= d;
	float ptLightIntensity = dot(lightToPixel, input.Normal);
	if (ptLightIntensity > 0.0f)
	{
		ptLightColor += ptLightIntensity * textureColor * PtLight1.Diffuse;
		ptLightColor /= PtLight1.Att[0] + (PtLight1.Att[1] * d) + (PtLight1.Att[2] * (d * d));
	}

	float4 finalColor = saturate(ptLightColor + dirLightColor);
	return finalColor;
}