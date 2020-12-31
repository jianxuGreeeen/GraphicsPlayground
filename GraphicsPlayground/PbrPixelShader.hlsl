Texture2D brdfLUT	: register(t0);
Texture2D albedoTex	: register(t1);
Texture2D roughnessTex	: register(t2);
Texture2D metalTex	: register(t3);
Texture2D normalTex	: register(t4);
SamplerState samplerState : register(s0);

struct ObjData
{
	float3 albedo;
	float3 normal;
	float roughness;
	float metallic;
};

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

cbuffer Data : register(b0)
{
	DirectionalLight DirLight;
	PointLight PtLight1;
	float3 CamPos;
};


static const float PI = 3.14159265359;

//Trowbridge-Reitz/GGX
float NormalDistributionGGX(float3 normalVec, float3 halfwayVec, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = dot(normalVec, halfwayVec);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = NdotH2 * (a2 - 1.0f) + 1.0f;
	denom = PI * denom * denom;

	return num / denom;
}

//Schlick's approximation
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

//Smith's approximation
float GeometrySmith(float3 normalVec, float3 viewDir, float3 lightDir, float roughness)
{
	float NdotV = max(dot(normalVec, viewDir), 0.0);
	float NdotL = max(dot(normalVec, lightDir), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

//Freshenl Schlick approximation
float3 FreshnelSchlick(float3 h, float3 v, float3 F0)
{
	float hDotv = max(dot(h, v), 0.0f);
	return F0 + (1.0f - F0) * pow(1.0f - hDotv, 5.0f);
}

float3 FreshnelSchlickRoughness(float3 h, float3 v, float3 F0, float roughness)
{
	float hDotv = max(dot(h, v), 0.0f);
	float3 Fr = max(float3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), F0);
	return F0 + ((Fr - F0) * pow(1.0f - hDotv, 5.0f));
}

//Cook Torrence
// f = D * F * G / (4 * (N.L) * (N.V))
// D = Normal distribution (Trowbridge-Reitz/GGX)
// F = Fresnel (Schlick's approximation)
// G = Geo shadowing term (Smith's approximation)
float3 CalcDirectLight(float3 worldPos, float3 viewDir, float3 normalVec, PointLight light, float3 F0, ObjData obj)
{
	//calculate light radiance
	float3 n = obj.normal;
	float3 v = viewDir;
	float3 l = normalize(light.Pos - worldPos);
	float3 h = normalize(v + l);

	float distance = length(light.Pos - worldPos);
	if (distance > light.Range)
	{
		return float3(0.0f, 0.0f, 0.0f);
	}

	float attenuation = 1.0f / dot(light.Att, float3(1.0f, distance, distance * distance));
	float3 radiance = light.Diffuse.rgb * attenuation;

	float D = NormalDistributionGGX(n, h, obj.roughness);
	float G = GeometrySmith(n, v, l, obj.roughness);
	float3 F = FreshnelSchlick(h, v, F0);

	float3 kS = F;
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= (1.0 - obj.metallic);

	float3 num = D * G * F;
	float denom = 4 * max(dot(n, v), 0.0f) * max(dot(n, l), 0.0); 
	float3 specular = num / max(denom, 0.001); // can't divide by 0

	//Add to outgoing radiance Lo
	float NdotL = max(dot(n, l), 0.0f);
	return (((kD * obj.albedo / PI) + specular) * radiance * NdotL);
}

float3 CalcIndirectLight(float3 viewDir, float3 normalVec, float3 F0, ObjData obj)
{
	float ao = 1.0f;

	float3 kS = FreshnelSchlickRoughness(normalVec, viewDir, F0, obj.roughness);
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= (1.0 - obj.metallic);

	float3 irradiance = float3(1.0f, 1.0f, 1.0f);
	float3 diffuse = obj.albedo * irradiance;

	const float MAX_REF_LOD = 4.0f;
	float3 prefilteredColor = float3(0.5f, 0.8f, 0.93f);
	float2 brdf = brdfLUT.Sample(samplerState, float2(max(dot(normalVec, viewDir), 0.0f), obj.roughness)).rg;
	float3 specular = prefilteredColor * (kS * brdf.x + brdf.y);

	float3 ambient = (kD * diffuse + specular) * ao;
	return ambient;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	ObjData obj;
	obj.albedo = albedoTex.Sample(samplerState, input.Uv).rgb;
	obj.normal = normalize(input.Normal*normalTex.Sample(samplerState, input.Uv).rgb);
	obj.metallic = metalTex.Sample(samplerState, input.Uv).r;
	obj.roughness = roughnessTex.Sample(samplerState, input.Uv).r;

	float3 v = normalize(CamPos.xyz - input.WorldPos.xyz);
	float3 n = obj.normal;
	float3 r = reflect(-v, n);

	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	F0 = lerp(F0, obj.albedo, obj.metallic);
	float3 totalDirectLight = float3(0.0f, 0.0f, 0.0f);
	// direct
	float3 light = CalcDirectLight(input.WorldPos.xyz, v, n, PtLight1, F0, obj);
	totalDirectLight += light;

	// indirect
	float3 ambient = CalcIndirectLight(v, n, F0, obj);
	float3 color = ambient + totalDirectLight;

	color = color / (color + float3(1.0f, 1.0f, 1.0f));
	color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

	return float4(color, 1.0f);
}