#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTex;

layout (location = 0) out vec3 vPos;
layout (location = 1) out vec3 vNor;
layout (location = 2) out vec2 vTex;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vPos = aPos;
    vNor = aNor;
    vTex = aTex;
    gl_Position = projection * view * vec4(aPos,1);
}