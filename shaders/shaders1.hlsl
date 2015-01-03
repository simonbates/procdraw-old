cbuffer CBufferPerObject
{
    row_major float4x4 WorldViewProjection;
}

struct VertexShaderInput
{
    float4 pos : POSITION;
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
    output.color = input.color;

    return output;
}

float4 pixel_shader(VertexShaderOutput input) : SV_Target
{
    return input.color;
}
