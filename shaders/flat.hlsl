cbuffer CBufferPerObject
{
    row_major float4x4 WorldViewProjection;
    // Direction to light in model space
    float4 LightDirection;
    float4 LightColor;
    float4 AmbientLightColor;
    float4 MaterialColor;
}

struct VertexShaderInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
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
    float4 diffComponent = (LightColor * MaterialColor * diffFactor);
    float4 ambientComponent = (AmbientLightColor * MaterialColor);
    output.color.rgb = diffComponent.rgb + ambientComponent.rgb;
    output.color.a = MaterialColor.a;

    return output;
}

float4 pixel_shader(VertexShaderOutput input) : SV_Target
{
    return input.color;
}
