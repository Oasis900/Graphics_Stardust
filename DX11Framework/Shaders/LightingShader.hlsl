cbuffer ConsantBuffer : register(b0)
{
    float4x4 projection;    // 64  | 16 
    float4x4 view;          // 128 | 16
    float4x4 world;         // 160 | 16
    
    float4 diffuse_light;   // 176 | 4
    float4 diffuse_mat;     // 192 | 4
    
    float4 ambient_light;   // 208 | 4
    float4 ambient_mat;     // 224 | 4

    float4 specular_light;  // 240 | 4
    float4 specular_mat;    // 256 | 4
    float3 camera_position; // 268 | 3
    float specular_power;   // 272 | 1
    
    float3 point_pos;       // 284 | 3
    float point_range;      // 288 | 1
    float4 point_color;     // 304 | 4
    
    float3 light_direction; // 316 | 3
    float simple_count;     // 320 | 1

    float4 has_texture;     // 336 | 4
}

Texture2D crate_texture : register(t0);
Texture2D asphalt_texture : register(t1);
SamplerState bilinear_sampler : register(s0);

struct LightComponents
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
};

struct VSOut
{
    float4 vs_pos : SV_POSITION0;
    float3 vs_normal : NORMAL0;
    float4 vs_color : COLOR0;
    
    float3 ps_pos : POSITION0;
    float3 ps_normal : NORMAL1;
    float3 ps_tex_coord : TEXCOORD0;
    float3 ps_point_pos : POSITION1;
    float4 ps_color : COLOR1;
};

VSOut VS_main(float3 Position : POSITION, float3 Normal : NORMAL, float2 TexCoord : TEXCOORD)
{   
    VSOut output = (VSOut) 0;
    //--------------------------------------------------//
    float4 PosW = float4(Position, 1.0f);
    float4 NormalW = float4(Normal, 0.0f);
    float3 TexCoordW = float3(TexCoord, 0.0f);
    float4 PointPosW = float4(Position, 0.0f);
    //--------------------------------------------------//
    output.vs_pos = mul(PosW, world);
    output.vs_normal = mul(NormalW, world);
    output.vs_pos = mul(output.vs_pos, view); 
    output.vs_pos = mul(output.vs_pos, projection);
    //--------------------------------------------------//
    //--------------------------------------------------//
    output.ps_pos = mul(PosW, world);
    output.ps_normal = normalize(output.vs_normal);
    output.ps_tex_coord = TexCoordW;
    //--------------------------------------------------//
    output.ps_point_pos = normalize(mul(PointPosW, world));
    //--------------------------------------------------//
    return output;
}

float4 CalculateDiffuseLight(VSOut input, uint type, LightComponents light, float4 tex_color)
{
    float3 light_to_normal;
    
    [call]
    switch (type)
    {
        case 0: // --- Directional Light Diffuse --- //
            
            light_to_normal = normalize(light_direction);
                
            [flatten]
            if (has_texture.x == 1)
            {
                light.diffuse = point_color * diffuse_light * tex_color * saturate(dot(-light_to_normal, input.ps_normal));
            }
            else
            {
                light.diffuse = point_color * diffuse_light * diffuse_mat * saturate(dot(-light_to_normal, input.ps_normal));
            }
            
        break;
        //--------------------------------------------------//
        case 1: // --- Point Light Diffuse --- //
            
            float p_constant = 1.0f;
            float p_linear = 0.8f;
            float p_quadratic = 0.0f;
            
            light_to_normal = input.ps_point_pos - input.ps_pos;

            float distance = length(light_to_normal);

            [flatten]
            if (has_texture.x == 1)
            {
                light.diffuse = point_color * diffuse_light * tex_color * saturate(1.0f / (p_constant + p_linear * distance + p_quadratic * pow(distance, 2)));
            }
            else
            {
                light.diffuse = point_color * diffuse_light * diffuse_mat * saturate(1.0f / (p_constant + p_linear * distance + p_quadratic * pow(distance, 2)));
            }
            
        break;
        //--------------------------------------------------//
        case 2: // --- Spot Light Diffuse --- //
            // TODO : If there is time implement a Spot Light
        break;

        default:
        break;
    }

    return light.diffuse;
}

float4 CalculateAmbientLight(LightComponents light, float4 tex_color)
{
    if (has_texture.x == 1)
    {
        light.ambient = point_color * ambient_light * tex_color;
    }
    else
    {
        light.ambient = point_color * ambient_light * ambient_mat;
    }
    
    return light.ambient;
}

float4 CalculateSpecularLight(VSOut input, uint type, LightComponents light, float4 tex_color)
{
    float3 light_to_normal, view_source, halfway_vector, reflection_source;
    
    [call]
    switch (type)
    {
        case 0: // --- Directional Light Specular --- //
                
            light_to_normal = normalize(light_direction);
        
            view_source = normalize(camera_position - input.ps_pos); // Destination minus Start

            halfway_vector = normalize(light_to_normal + view_source);
            
            reflection_source = normalize(reflect(input.ps_normal, halfway_vector));
                    
            [flatten]
            if (has_texture.x == 1) // TODO : Not the correct Implementation for Specular Lighting with material
            {
                light.specular = point_color * specular_light * tex_color * pow(saturate(dot(view_source, reflection_source)), specular_power);
            }
            else
            {
                light.specular = point_color * specular_light * specular_mat * pow(saturate(dot(view_source, reflection_source)), specular_power);
            }
        
        break;
        //--------------------------------------------------//
        case 1: // --- Point Light Specular --- //

            [flatten]
            if (has_texture.x == 1)
            {
                
            }
            else
            {
                
            }
                
        break;
        //--------------------------------------------------//
        case 2: // --- Spot Light Specular --- //
            // TODO : If there is time implement a Spot Light
        break;

        default:
        break;
    }

    return light.specular;
}

float4 DirectionalLight(VSOut input)
{
    LightComponents light_comps;
    //--------------------------------------------------//
    float4 crate_color = crate_texture.Sample(bilinear_sampler, input.ps_tex_coord);
    float4 asphalt_color = asphalt_texture.Sample(bilinear_sampler, input.ps_tex_coord);
    //--------------------------------------------------//
    light_comps.diffuse = CalculateDiffuseLight(input, 0, light_comps, crate_color) + CalculateDiffuseLight(input, 0, light_comps, asphalt_color);
    light_comps.ambient = CalculateAmbientLight(light_comps, crate_color) + CalculateAmbientLight(light_comps, asphalt_color);
    light_comps.specular = CalculateSpecularLight(input, 0, light_comps, crate_color) + CalculateSpecularLight(input, 0, light_comps, asphalt_color) * 0.5f;
    //--------------------------------------------------//
    input.ps_color = light_comps.specular + light_comps.diffuse + light_comps.ambient;
    //--------------------------------------------------//
    return input.ps_color;
}

float4 PointLight(VSOut input) // -- ! Functional but not optimised for current scene ! -- //
{
    LightComponents light_comps;
    //--------------------------------------------------//
    float4 crate_color = crate_texture.Sample(bilinear_sampler, input.ps_tex_coord);
    float4 asphalt_color = asphalt_texture.Sample(bilinear_sampler, input.ps_tex_coord);
    //--------------------------------------------------//
    light_comps.diffuse = CalculateDiffuseLight(input, 1, light_comps, crate_color) + CalculateDiffuseLight(input, 1, light_comps, asphalt_color);
    light_comps.ambient = CalculateAmbientLight(light_comps, crate_color) + CalculateAmbientLight(light_comps, asphalt_color);
    
    input.ps_color = light_comps.diffuse * 1.0f + light_comps.ambient * 1.0f;

    return input.ps_color;

}

float4 PS_main(VSOut input) : SV_TARGET
{
    return DirectionalLight(input);
}



