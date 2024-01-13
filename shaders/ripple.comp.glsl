#version 450

layout (local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r32f) uniform image2D currentWaterSurface;
layout(binding = 1, r32f) uniform image2D nextWaterSurface;

uniform float DAMP = 15.0; // Damping factor

uniform int WATERSIZE;

void main() {
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    // Ensure we're not on the edge of the texture
    if (pos.x > 0 && pos.x < WATERSIZE - 1 && pos.y > 0 && pos.y < WATERSIZE - 1) {
        float currentHeight = imageLoad(currentWaterSurface, pos).r;
        float left = imageLoad(currentWaterSurface, pos + ivec2(-1, 0)).r;
        float right = imageLoad(currentWaterSurface, pos + ivec2(1, 0)).r;
        float up = imageLoad(currentWaterSurface, pos + ivec2(0, -1)).r;
        float down = imageLoad(currentWaterSurface, pos + ivec2(0, 1)).r;

        // Ripple calculation
        float newHeight = (left + right + up + down) / 2.0 - currentHeight;
        newHeight -= newHeight / DAMP;

        // Write the new height to the next water surface
        imageStore(nextWaterSurface, pos, vec4(newHeight, 0.0, 0.0, 0.0));
    }
}