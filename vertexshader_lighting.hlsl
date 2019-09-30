cbuffer cbPerObject
{
    matrix wvp;
    matrix worldMatrix;
};

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexC : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 PositionH : SV_POSITION;
    float3 PositionW : POSITION;
    float3 Normal : NORMAL;
    float2 TexC : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.PositionH = mul(float4(input.Position, 1.0f), wvp);
    output.PositionW = mul(float4(input.Position, 1.0f), worldMatrix).xyz;
    output.Normal = mul(float4(input.Normal, 0.0f), worldMatrix).xyz;
    output.TexC = input.TexC;

    return output;
}