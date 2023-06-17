#version 300 es
// 绘制外层图像
precision highp float;
layout (location = 0) out vec4 outColor;
in vec2 v_texCoord;
uniform sampler2D s_TextureMap;
void main() {
    vec4 sampler0;
    vec4 sampler1;
    vec4 sampler2;
    vec4 sampler3;
    float blueStep = 0.16;
    float step = blueStep / 100.0;
    sampler0 = texture(s_TextureMap, vec2(v_texCoord.x - step, v_texCoord.y - step));
    sampler1 = texture(s_TextureMap, vec2(v_texCoord.x + step, v_texCoord.y + step));
    sampler2 = texture(s_TextureMap, vec2(v_texCoord.x + step, v_texCoord.y - step));
    sampler2 = texture(s_TextureMap, vec2(v_texCoord.x - step, v_texCoord.y + step));
    outColor = (sampler0 + sampler1 + sampler2 + sampler3) / 4.0;

    // 放宽 alpha 值过滤范围，使白斑变的更大
    if (outColor.a > 0.05) {
        //叠加一些强度，使白斑变的更亮
        outColor += vec4(0.2, 0.2, 0.2, 0.0);
    } else {
        discard;
    }
}