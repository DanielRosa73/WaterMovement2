#version 450

layout (local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r32f) uniform image2D prevWaterSurface;
layout(binding = 1, r32f) uniform image2D currentWaterSurface;

uniform float deltaTime;
uniform int WATERSIZE;
uniform float waveSpeed = 2.0;
uniform float dampingFactor = 1.0;
uniform float edgeDampingFactor = 2.0; // Higher damping factor for edges

void main() {
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    if (pos.x > 0 && pos.x < WATERSIZE && pos.y > 0 && pos.y < WATERSIZE) {
        // Non-edge processing
        float previousHeight = imageLoad(prevWaterSurface, pos).r;
        float currentHeight = imageLoad(currentWaterSurface, pos).r;
        float left = imageLoad(currentWaterSurface, pos + ivec2(-1, 0)).r;
        float right = imageLoad(currentWaterSurface, pos + ivec2(1, 0)).r;
        float up = imageLoad(currentWaterSurface, pos + ivec2(0, -1)).r;
        float down = imageLoad(currentWaterSurface, pos + ivec2(0, 1)).r;

        float heightDifference = currentHeight - previousHeight;
        float dampedDifference = heightDifference * (1.0 - dampingFactor * deltaTime);

        float newHeight = currentHeight + dampedDifference + waveSpeed * waveSpeed * deltaTime * deltaTime * (left + right + up + down - 4.0 * currentHeight);

        imageStore(prevWaterSurface, pos, vec4(newHeight, 0.0, 0.0, 0.0));
    } else {
        // Edge processing
        float previousHeight = imageLoad(prevWaterSurface, pos).r;
        float currentHeight = imageLoad(currentWaterSurface, pos).r;

        float heightDifference = currentHeight - previousHeight;
        // Invert the wave direction and apply higher damping at edges
        float dampedDifference = -heightDifference * (1.0 - edgeDampingFactor * deltaTime);

        float newHeight = currentHeight + dampedDifference;

        imageStore(prevWaterSurface, pos, vec4(newHeight, 0.0, 0.0, 0.0));
    }
}