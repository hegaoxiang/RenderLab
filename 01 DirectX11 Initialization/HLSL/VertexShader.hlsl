#include"Light.hlsli"

VSOut VS(float3 posL : POSITIONT)
{
    VSOut vOut;
    vOut.posH = mul(mul(mul(float4(posL, 1.0), g_World), g_View), g_Proj);
    return vOut;
}
