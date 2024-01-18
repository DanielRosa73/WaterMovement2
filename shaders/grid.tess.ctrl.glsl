#version 450

layout(vertices = 3) out;

layout (location = 0) in vec3 vPos[];
layout (location = 1) in vec3 vNor[];

uniform mat4 view; // Camera view matrix
uniform float tessFactor = 3.0; 
uniform float distanceFactor = 0.1; 

layout (location = 0) out vec3 tcPos[];
layout (location = 1) out vec3 tcNor[];

void main() {
    if (gl_InvocationID == 0) {
        // Calculate the average position of the triangle's vertices
        vec4 p = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;

        vec4 viewPos = view * p;

        // Calculate tessellation level based on distance
        float level = 4; //tessFactor + distanceFactor / abs(viewPos.z);

        // Set tessellation levels
        gl_TessLevelInner[0] = level;
        gl_TessLevelInner[1] = level;

        gl_TessLevelOuter[0] = level;
        gl_TessLevelOuter[1] = level;
        gl_TessLevelOuter[2] = level;
        gl_TessLevelOuter[3] = level;

    }

    tcPos[gl_InvocationID] = vPos[gl_InvocationID];
    tcNor[gl_InvocationID] = vNor[gl_InvocationID];
    // Pass the vertex position to the tessellation evaluation shader
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}