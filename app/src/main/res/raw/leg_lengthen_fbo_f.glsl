#version 300 es
precision mediump float;
in vec2 v_texCoord;
layout (location = 0) out vec4 outColor;
uniform sampler2D s_TextureMap;
void main() {
    vec4 tempColor = texture(s_TextureMap, v_texCoord);
    float luminance = tempColor.r * 0.299 + tempColor.g * 0.587 + tempColor.b * 0.114;
    // outColor = vec4(vec3(luminance),tempColor.a);
    outColor = tempColor;
}