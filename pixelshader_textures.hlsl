struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 16;

    AddressU = WRAP;
    AddressV = WRAP;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    return g_texture.Sample(samAnisotropic, input.tex);
}