#version 440 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;

void main()
{
    vec3 toon;
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);

    float intensity = dot(lightDir, norm);

    if (intensity > 0.95)
        toon = vec3(1.0, 0.5, 0.31);
    else if (intensity > 0.5)
        toon = vec3(0.6, 0.3, 0.21);
    else if (intensity > 0.25)
        toon = vec3(0.4, 0.2, 0.11);
    else
        toon = vec3(0.2, 0.1, 0.11);

    FragColor = vec4(toon, 1.0f);
}