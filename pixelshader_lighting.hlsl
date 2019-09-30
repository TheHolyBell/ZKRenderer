struct Light
{
    float3 Pos;
    float pad1;
    float3 Diffuse;
    float pad2;
    float3 Ambient;
    float pad3;
};

cbuffer cbPerFrame
{
    Light light;
};

Texture2D g_texture : register(t0);
SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 16;

    AddressU = WRAP;
    AddressV = WRAP;
};

struct VS_OUTPUT
{
    float4 PositionH : SV_POSITION;
    float3 PositionW : POSITION;
    float3 Normal : NORMAL;
    float2 TexC : TEXCOORD;
};


float4 main(VS_OUTPUT input) : SV_Target
{
    input.Normal = normalize(input.Normal);

    float3 sampleColor = g_texture.Sample(samAnisotropic, input.TexC);
    //float3 sampleColor = input.normal = normalize(input.normal);

    float3 ambientLight = light.Ambient * 0.4f;

    float3 appliedLight = ambientLight;

    float3 vectorToLight = normalize(light.Pos - input.PositionW);

    float3 diffuseLightIntensity = max(dot(input.Normal, vectorToLight), 0);

    float distanceToLight = length(light.Pos - input.PositionW);
    float attenutaionFactor = 1 / (1 + distanceToLight * 0.01f);

    
    diffuseLightIntensity *= attenutaionFactor;

    appliedLight += diffuseLightIntensity * light.Diffuse;

    float3 finalColor = sampleColor * appliedLight;

    return float4(finalColor, 1.0f);

   // return float4(input.Normal, 1.0f);
}