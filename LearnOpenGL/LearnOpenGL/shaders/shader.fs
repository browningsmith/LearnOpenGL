#version 330 core
out vec4 FragColor;

in vec3 currentColor;

void main() {
    
    FragColor = vec4(currentColor, 1.0);
}