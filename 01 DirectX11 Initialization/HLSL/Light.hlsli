
cbuffer MVP : register(b0)
{
    matrix g_World;
    matrix g_View;
    matrix g_Proj;
};

struct VSIn
{
    float3 posL : POSITIONT;
};

struct VSOut
{
    float4 posH : SV_Position;
};
