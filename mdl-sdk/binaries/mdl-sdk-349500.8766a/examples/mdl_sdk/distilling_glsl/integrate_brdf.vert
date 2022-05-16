/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/
#version 330 core

layout (location = 0) in vec3 Position;

out vec2 texture_coordinate;

void main()
{
    texture_coordinate = (vec2(Position.x, Position.y) + vec2(1.0)) * 0.5;
    gl_Position = vec4(Position, 1.0);
}
