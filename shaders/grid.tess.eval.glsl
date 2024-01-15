#version 450

layout(triangles, equal_spacing, cw) in;

uniform mat4 view;
uniform mat4 projection;
uniform sampler2D waterHeightMap;
uniform int WATERSIZE;
uniform float deltaTime; // Uniform for time to add motion

out vec3 normal;

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
    vec2 normalizedPos = (worldPos.xz + vec2(0.5 * WATERSIZE)) / WATERSIZE;
    float height = getHeight(normalizedPos);
    worldPos.y += height;

    // Calculate normals (no dfdy or dfdy)
    float heightLeft = getHeight(normalizedPos + vec2(-1.0 / WATERSIZE, 0.0));
    float heightRight = getHeight(normalizedPos + vec2(1.0 / WATERSIZE, 0.0));
    float heightDown = getHeight(normalizedPos + vec2(0.0, -1.0 / WATERSIZE));
    float heightUp = getHeight(normalizedPos + vec2(0.0, 1.0 / WATERSIZE));

    vec3 tangent = vec3(2.0 / WATERSIZE, heightRight - heightLeft, 0.0);
    vec3 bitangent = vec3(0.0, heightUp - heightDown, 2.0 / WATERSIZE);

    normal = normalize(cross(tangent, bitangent));

    gl_Position = projection * view * vec4(worldPos, 1.0);
}