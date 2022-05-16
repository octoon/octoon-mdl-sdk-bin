/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/
#version 330 core

// varying inputs
in vec3 world_position;

// uniforms
uniform samplerCube env_tex;

// outputs
out vec4 FragColor;

void main() {

    vec3 color = texture(env_tex, world_position).rgb;
    FragColor = vec4(pow(color, vec3(1.0/2.2)), 1);
}
