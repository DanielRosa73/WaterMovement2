#version 450

layout (local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r32f) uniform image2D prevWaterSurface;  // Previous state (last frame)
layout(binding = 1, r32f) uniform image2D currentWaterSurface; // Current state (this frame)

uniform float deltaTime;       // Time elapsed since the last update
uniform int WATERSIZE;         // Size of the water surface grid
uniform float waveSpeed = 1.0; // Speed of the wave propagation, adjust as necessary

void main() {
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    // Ensure we're not on the edge of the texture
    if (pos.x > 0 && pos.x < WATERSIZE - 1 && pos.y > 0 && pos.y < WATERSIZE - 1) {
        float previousHeight = imageLoad(prevWaterSurface, pos).r;
        float currentHeight = imageLoad(currentWaterSurface, pos).r;
        float left = imageLoad(currentWaterSurface, pos + ivec2(-1, 0)).r;
        float right = imageLoad(currentWaterSurface, pos + ivec2(1, 0)).r;
        float up = imageLoad(currentWaterSurface, pos + ivec2(0, -1)).r;
        float down = imageLoad(currentWaterSurface, pos + ivec2(0, 1)).r;

        // Wave equation
        float newHeight = 2.0 * currentHeight - previousHeight + waveSpeed * waveSpeed * deltaTime * deltaTime * (left + right + up + down - 4.0 * currentHeight);

        // Write the new height to the previous water surface, which becomes the next state
        imageStore(prevWaterSurface, pos, vec4(newHeight, 0.0, 0.0, 0.0));
    }
}