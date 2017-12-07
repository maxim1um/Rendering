#version 440

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;

uniform vec3 coolColor;
uniform vec3 warmColor;

uniform vec3 lightPos;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);

    float diff = dot(norm, lightDir);
    float instensity = (1.0 + diff) / 2;

    vec3 coolColorMod = coolColor + objectColor * 0.25;
    vec3 warmColorMod = warmColor + objectColor * 0.5;

    vec3 result = mix(coolColorMod, warmColorMod, instensity);

    FragColor = vec4(result, 1.0);
}