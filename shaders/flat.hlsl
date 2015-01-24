cbuffer CBufferPerObject
{
    row_major float4x4 WorldViewProjection;
    // direction to light in model space
    float4 LightDirection;
    float4 LightColor;
    float4 AmbientColor;
}

struct VertexShaderInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct VertexShaderOutput
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VertexShaderOutput vertex_shader(VertexShaderInput input)
{
    VertexShaderOutput output;

    output.pos = mul(input.pos, WorldViewProjection);
    
    float diffFactor = max(dot(input.normal, LightDirection.xyz), 0);
    float4 diffComponent = (input.color * diffFactor * LightColor);
    float4 ambientComponent = (input.color * AmbientColor);
    output.color.rgb = diffComponent.rgb + ambientComponent.rgb;
    output.color.a = input.color.a;

    return output;
}

float4 pixel_shader(VertexShaderOutput input) : SV_Target
{
    return input.color;
}
