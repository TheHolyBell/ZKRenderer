cbuffer mycBuffer : register(b0)
{
    matrix mat;
}

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(float4(input.inPos, 1.0f), mat);
    output.texCoord = input.inTexCoord;
    return output;
}