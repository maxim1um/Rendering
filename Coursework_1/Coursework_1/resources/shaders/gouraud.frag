#version 440

out vec4 FragColor;

in vec3 lightingColor;

uniform vec3 objectColor;

void main()
{
    FragColor = vec4(lightingColor * objectColor, 1.0);
}