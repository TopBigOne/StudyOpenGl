#version 300 es
precision mediump float;

layout (location = 0) out vec4 outColor;
uniform vec4 u_Color;
void main() {
    outColor = u_Color;


}