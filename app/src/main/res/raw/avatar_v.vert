#version 300  es
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texCoord;
// 变换矩阵
uniform mat4 u_MVPMatrix;
out vec2 v_texCoord;

void main() {
    gl_Position = u_MVPMatrix * a_position;
    // 传递给片元着色器
    v_texCoord = a_texCoord;
}