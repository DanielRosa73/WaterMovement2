#version 450 core

in vec3 position;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform bool noColor;
uniform bool normalColor;

out vec4 FragColor;

void main() {
    if (noColor) {
        FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        return;
    }
    if (normalColor) {
        FragColor = vec4(normal * 0.5 + 0.5, 1.0);
        return;
    }
    vec3 lightDir = normalize(lightPos - position);
    
    float diff = max(dot(normal, lightDir), 0.0);

    float specularStrength = 0.75;
    vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 waterColor = vec3(0.0, 0.3, 0.5);
    float ambiant = 0.6;
    vec3 result = (diff * lightColor + specularStrength * spec * lightColor + ambiant) * waterColor;

    FragColor = vec4(result, 1.0);
}