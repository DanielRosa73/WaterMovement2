#version 450 core

in vec3 position;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main() {
    vec3 lightDir = normalize(lightPos - position);
    
    float diff = max(dot(normal, lightDir), 0.0);

    float specularStrength = 0.75;
    vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 waterColor = vec3(0.0, 0.3, 0.5);
    float ambiant = 1.0f;
    vec3 result = (diff * lightColor + specularStrength * spec * lightColor + ambiant) * waterColor;

    FragColor = vec4(result, 1.0);
}