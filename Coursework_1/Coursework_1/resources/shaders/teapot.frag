#version 440

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightColor;
uniform vec3 objectColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int controlBit;

const vec3 coolColor = vec3(159.0f / 255, 148.0f / 255, 255.0f / 255);
const vec3 warmColor = vec3(255.0f / 255, 75.0f / 255, 75.0f / 255);
const float alpa = 0.25;
const float beta = 0.5;
const float ambientStrength = 0.1;
const float specularStrength = 0.5;
const float shininess = 32.0;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    float intensity, diff, spec;
    vec3 ambient, diffuse, specular;

    vec3 result;
    if (controlBit == 1)
    {
        // This is cel/toon shding
        intensity = dot(lightDir, norm);

        if (intensity > 0.95)
            result = vec3(1.0, 0.5, 0.31);
        else if (intensity > 0.5)
            result = vec3(0.6, 0.3, 0.21);
        else if (intensity > 0.25)
            result = vec3(0.4, 0.2, 0.11);
        else
            result = vec3(0.2, 0.1, 0.11);
    }
    if (controlBit == 2)
    {
        // This is gooch shading
        intensity = (1.0 + dot(lightDir, norm)) / 2;
        vec3 coolColorMod = coolColor + objectColor * 0.25;
        vec3 warmColorMod = warmColor + objectColor * 0.5;

        result = mix(coolColorMod, warmColorMod, intensity);
    }
    if (controlBit == 3)
    {
        // This is phong shading
        ambient = lightColor * ambientStrength;

        diff = max(dot(norm, lightDir), 0);
        diffuse = diff * lightColor;

        vec3 reflectDir = reflect(-lightDir, norm);

        spec = pow(max(dot(viewDir, reflectDir), 0), shininess);
        specular = specularStrength * spec * lightColor;

        result = (ambient + diffuse + specular) * objectColor;
    }
    if (controlBit == 4)
    {
        // This is blinn-phong shading
        ambient = lightColor * ambientStrength;

        diff = max(dot(norm, lightDir), 0);
        diffuse = diff * lightColor;

        vec3 halfDir = normalize(lightDir + viewDir);

        spec = pow(max(dot(halfDir, norm), 0), shininess);
        specular = specularStrength * spec * lightColor;

        result = (ambient + diffuse + specular) * objectColor;
    }

    FragColor = vec4(result, 1.0);
}