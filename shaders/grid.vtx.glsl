#version 450

layout (location = 0) in vec3 aPos;

layout (location = 0) out vec3 vPos;
layout (location = 1) out vec3 vNor;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vPos = aPos;
    vNor = vec3(0.0, 1.0, 0.0);
    gl_Position = vec4(aPos,1);
}