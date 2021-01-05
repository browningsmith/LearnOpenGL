#version 330 core
layout (location = 0) in vec3 APos;
layout (location = 1) in vec3 AColor;

out vec3 CurrentColor;

void main()
{
    gl_Position = vec4(APos, 1.0);
    CurrentColor = AColor;
}