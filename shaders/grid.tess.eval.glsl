#version 450

layout(triangles, equal_spacing, cw) in;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;
    vec3 position = p0 + p1 + p2;

    gl_Position = projection * view * vec4(position, 1.0);
}