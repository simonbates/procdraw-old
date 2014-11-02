struct VS_INPUT
{
    float4 ObjectPosition : POSITION;
    float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.Position = IN.ObjectPosition;
    OUT.Color = IN.Color;

    return OUT;
}

float4 pixel_shader(VS_OUTPUT IN): SV_Target
{
    return IN.Color;
}
