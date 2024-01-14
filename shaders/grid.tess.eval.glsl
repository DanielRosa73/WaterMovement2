#version 450

layout(triangles, equal_spacing, cw) in;

uniform mat4 view;
uniform mat4 projection;
uniform sampler2D waterHeightMap;
uniform int WATERSIZE;
uniform float deltaTime; // Uniform for time to add motion

vec3 getWorldPos(vec3 barycentricCoords) {
    return barycentricCoords.x * gl_in[0].gl_Position.xyz +
           barycentricCoords.y * gl_in[1].gl_Position.xyz +
           barycentricCoords.z * gl_in[2].gl_Position.xyz;
}

float getHeight(vec2 normalizedPos) {
    // Sample the height from the heightmap
    return texture(waterHeightMap, normalizedPos).r;
}

vec2 getHorizontalDisplacement(vec2 normalizedPos) {
    float displacement = sin(deltaTime + length(normalizedPos) * 10.0) * 0.01;
    return normalizedPos + vec2(displacement);
}

void main() {
    vec3 worldPos = getWorldPos(gl_TessCoord);

    // Normalize position to [0, 1] range
    vec2 normalizedPos = (worldPos.xz + vec2(0.5 * WATERSIZE)) / WATERSIZE;

    float height = getHeight(normalizedPos);

    worldPos.y += height;

    // Add horizontal displacement to create more dynamic movement
    vec2 displacement = getHorizontalDisplacement(normalizedPos);
    worldPos.x += displacement.x;
    worldPos.z += displacement.y;

    gl_Position = projection * view * vec4(worldPos, 1.0);
}