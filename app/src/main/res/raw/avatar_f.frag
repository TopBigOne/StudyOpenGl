#version 300 es

 precision highp float;
in vec2 v_texCoord;
layout (location = 0) out vec4 outColor;
// 2D 纹理采样器
uniform sampler2D s_TextureMap;
uniform vec2 u_texSize;
uniform float u_needRotate;
uniform float u_rotateAngle;
vec2 rotate(float radius, float angle, vec2 texSize, vec2 texCoord) {
    vec2 newTexCoord = texCoord;
    vec2 center = vec2(texSize.x / 2.0, texSize.y / 2.0);
    vec2 tc = texCoord * texSize;
    tc -= center;
    // length(x): 向量x的长度。
    float dist = length(tc);
    if (dist < radius) {
        float percent = (radius - dist) / radius;
        float theta = percent * percent * angle * 8.0;
        float s = sin(theta);
        float c = cos(theta);
        // float dot(T x, T y)返回x y的点积
        tc = vec2(dot(tc, vec2(c, s)), dot(tc, vec2(s, c)));
        tc += center;
        newTexCoord = tc / texSize;
    }

    return newTexCoord;
}

void main() {
    vec2 texCoord = v_texCoord;
    if (u_needRotate > 0.0) {
        texCoord = rotate(0.5 * u_texSize.x, u_rotateAngle, u_texSize, v_texCoord);
    }
    // 输出的颜色
    outColor = texture(s_TextureMap, texCoord);
    //丢弃透明度比较低的片元
    if (outColor.a < 0.6) {
        discard;
    }
}