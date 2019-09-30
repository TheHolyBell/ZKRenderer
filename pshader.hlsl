cbuffer alphaBuffer : register(b0)
{
    float alpha;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 16;

    AddressU = WRAP;
    AddressV = WRAP;
};

float4 main(PS_INPUT input) : SV_Target
{
    return objTexture.Sample(samAnisotropic, input.texCoord);

}