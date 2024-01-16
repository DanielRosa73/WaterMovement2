#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D uTexture;

vec3 lightPos = vec3(0.0, 10.0, 0.0);
vec3 lightColor = vec3(1.0);

void main() {
    vec3 lightDir = normalize(lightPos - position);
    float diff = max(dot(lightDir, normal), 0.0) + 0.5; // Assurez-vous que la valeur est positive
    vec3 diffuse = diff * lightColor;

    vec4 texColor = texture(uTexture, texCoords); // Échantillonne la couleur de la texture

    vec3 finalColor = 2.0 * diffuse * texColor.rgb; // Mélangez la texture avec l'éclairage diffus
    fragColor = vec4(finalColor, 1.0);
}
