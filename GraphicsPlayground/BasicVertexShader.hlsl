struct VS_OUTPUT
{
    float4 Color : COLOR;
    float4 Pos : SV_POSITION;
};

VS_OUTPUT main(float4 inPos : POSITION, float4 inColor : COLOR)
{
    VS_OUTPUT output;

    output.Pos = inPos;
    output.Color = inColor;

    return output;
}