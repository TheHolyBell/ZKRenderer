cbuffer mycBuffer : register(b0)
{
    matrix mat;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos, 1.0f), mat);
    output.tex = input.tex;
    return output;
}