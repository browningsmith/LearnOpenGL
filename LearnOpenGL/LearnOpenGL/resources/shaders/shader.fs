#version 330 core
out vec4 FragColor;

in vec3 CurrentColor;

void main()
{
    FragColor = vec4(CurrentColor, 1.0);
}