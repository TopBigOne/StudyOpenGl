#version 300 es

precision highp float;
in vec2 v_texCoord;
layout (location = 0) out vec4 outColor;
uniform sampler2D s_TexureMap;
void main() {
    outColor = texture(s_TexureMap,v_texCoord);
}