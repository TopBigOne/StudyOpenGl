#version 300 es
layout (location = 0) in vec4 a_positon;
layout (location = 1) in vec2 a_texCoord;
uniform mat4 u_MVPMatrix;
out vec2 v_texCoord;
void main() {
    gl_Position = u_MVPMatrix * a_positon;
    v_texCoord = a_texCoord;
}