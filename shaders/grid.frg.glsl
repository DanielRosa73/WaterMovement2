#version 450

in vec3 normal; // Received normal from tessellation evaluation shader
out vec4 FragColor;

void main() {
    // Map the normal vector components to color
    vec3 color = normal * 0.5 + 0.5; // Normalizing from [-1, 1] to [0, 1]
    FragColor = vec4(color, 1.0);
}