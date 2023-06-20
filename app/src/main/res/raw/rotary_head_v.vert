#version 300 es
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texCoord;
uniform mat4 u_MVPMatrix;
out vec2 v_texCoord;
void main() {
    gl_Position = u_MVPMatrix*a_position;
    v_texCoord = a_texCoord;
    // 当前顶点对应的点的大小为8个像素
    gl_PointSize = 8.0;
}