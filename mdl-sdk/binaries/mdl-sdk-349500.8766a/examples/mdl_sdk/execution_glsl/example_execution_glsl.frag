/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

// examples/mdl_sdk/execution_glsl/example_execution_glsl.frag
//
// This file contains the implementations of the texture access functions
// and the fragment shader used to evaluate the material sub-expressions.

// Material pattern as chosen by the user.
uniform uint material_pattern;

// Current time in seconds since the start of the render loop.
uniform float animation_time;

// Mapping from material index to start index in material_texture_samplers_2d.
uniform uint material_texture_starts[MAX_MATERIALS];

// Array containg all 2D texture samplers of all used materials.
uniform sampler2D material_texture_samplers_2d[MAX_TEXTURES];


// Start offset of the current material inside material_texture_samplers_2d, set in main.
uint tex_start;


// The input variables coming from the vertex shader.
in vec3[1] texture_coordinate;    // used for state::texture_coordinate(tex_space) in "arg" mode
in vec3    vPosition;


// The color output variable of this fragment shader.
out vec4 FragColor;


// The MDL material state structure as configured via the GLSL backend options.
// Note: Must be in sync with the state struct in generate_glsl_switch_func and the code generated
//       by the MDL SDK (see dumped code when enabling DUMP_GLSL in example_execution_glsl.cpp).
struct State {
    vec3 normal;
    vec3 geometry_normal;
    float animation_time;
    vec3[1] texture_tangent_u;
    vec3[1] texture_tangent_v;
};


//
// The prototypes of the functions generated in our generate_glsl_switch_func() function.
//

// Return the number of available MDL material subexpressions.
uint get_mdl_num_mat_subexprs();

// Return the result of the MDL material subexpression given by the id.
vec3 mdl_mat_subexpr(uint id, State state);


#if __VERSION__ < 400
int bitCount(uint x)
{
    x = x - ((x >> 1u) & 0x55555555u);
    x = (x & 0x33333333u) + ((x >> 2u) & 0x33333333u);
    x = (x + (x >> 4u)) & 0x0F0F0F0Fu;
    x = x + (x >> 8u);
    x = x + (x >> 16u);
    return int(x & 0x0000003Fu);
}
#endif

// Implementation of tex::lookup_*() for a texture_2d texture.
vec4 tex_lookup_2d(uint tex, vec2 coord, int wrap_u, int wrap_v, vec2 crop_u, vec2 crop_v, float frame)
{
    if (tex == 0u) return vec4(0);
    return texture(material_texture_samplers_2d[tex_start + tex - 1u], coord);
}


// Implementation of tex::texel_*() for a texture_2d texture.
vec4 tex_texel_2d(uint tex, ivec2 coord, ivec2 uv_tile)
{
    if (tex == 0u) return vec4(0);
    return texelFetch(material_texture_samplers_2d[tex_start + tex - 1u], coord, 0);
}


// Implementation of state::position() in "func" mode.
vec3 position()
{
    // Mirror the position across the center in this example.
    return -vPosition;
}


// The fragment shader main function evaluating the MDL sub-expression.
void main() {
    // Set number of materials to use according to selected pattern
    uint num_materials = uint(bitCount(material_pattern));

    // Assign materials in a checkerboard pattern
    uint material_index =
        (uint(texture_coordinate[0].x * 4) ^ uint(texture_coordinate[0].y * 4)) % num_materials;

    // Change material index according to selected pattern
    switch (material_pattern)
    {
        case 2u: material_index = 1u; break;
        case 4u: material_index = 2u; break;
        case 5u: if (material_index == 1u) material_index = 2u; break;
        case 6u: material_index += 1u; break;
    }
    if (material_index > get_mdl_num_mat_subexprs())
        material_index = get_mdl_num_mat_subexprs();

    // Set up texture access for the chosen material
    tex_start = material_texture_starts[material_index];
    
    // Set MDL material state for state functions in "field" mode
    State state = State(
        /*normal=*/              vec3(0.0, 0.0, 1.0),
        /*geometry_normal=*/     vec3(0.0, 0.0, 1.0),
        /*animation_time=*/      animation_time,
        /*texture_tangent_u=*/   vec3[1](vec3(1.0, 0.0, 0.0)),
        /*texture_tangent_v=*/   vec3[1](vec3(0.0, 1.0, 0.0))
    );

    // Evaluate material sub-expression
    vec3 res = mdl_mat_subexpr(material_index, state);

    // Apply gamma correction and write to output variable
    FragColor = pow(vec4(res, 1.0), vec4(1.0 / 2.2));
}
