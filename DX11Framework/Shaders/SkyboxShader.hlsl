cbuffer ConsantBuffer : register(b0)
{
    float4x4 projection;    // 64  | 16 
    float4x4 view;          // 128 | 16
    float4x4 world;         // 160 | 16
}

TextureCube skybox_texture : register(t2);
SamplerState skybox_sampler : register(s0);

struct VSOut
{
    float4 position : SV_POSITION;
    float3 tex_coord : TEXCOORD;
};

VSOut VS_main(float3 Position : SV_POSITION)
{
    VSOut output;

    float4 pos_trans = float4(Position, 1.0f);

    output.position = mul(pos_trans, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.position = output.position.xyww;

    output.tex_coord = Position;
    
    return output;
}

float4 PS_main(VSOut input) : SV_TARGET
{
    return skybox_texture.Sample(skybox_sampler, input.tex_coord);
}

