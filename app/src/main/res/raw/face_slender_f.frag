#version 300 es
precision highp float;

layout (location = 0) out vec4 outColor;
in vec2 v_texCoord;
uniform sampler2D s_TextureMap;
//图像分辨率
uniform vec2 u_texSize;
//pre控制点
uniform vec4 u_preCtrlPoints;
//cur控制点
uniform vec4 u_curCtrlPoints;
//影响半径
uniform float u_reshapeRadius;
// 强度
uniform float u_reshapeRatio;

vec2 face_slender_1(vec2 prePoint, vec2 curPoint, vec2 texCoord, float radius, vec2 texSize) {
    // 纹理位置
    vec2 pos = texCoord;
    vec2 newSrc = prePoint * texSize;
    vec2 newDst = curPoint * texSize;
    vec2 newTex = texCoord * texSize;
    float newRadius = radius;
    float r = distance(newSrc, newTex);
    if (r < newRadius) {
        float alpha = 1.0 - r / newRadius;
        vec2 displacementVec = (newDst - newSrc) * pow(alpha, 2.0) * 0.5 * u_reshapeRatio;
        pos = (newTex - displacementVec) / texSize;
    }
    return pos;
}
// 瘦脸模式2：
vec2 face_slender_2(vec2 prePoint, vec2 curPoint, vec2 texCoord, float radius, vec2 texSize) {
    vec2 pos = texCoord;
    vec2 pos = texCoord;
    vec2 newSrc = prePoint * texSize;
    vec2 newDst = curPoint * texSize;
    vec2 newTex = texCoord * texSize;
    float newRadius = radius;
    float r = distance(newSrc, newTex);
    if (r < newRadius) {
        float powRadius = pow(newRadius, 2.0);
        float powDistance = pow(r, 2.0);
        float powNewDistance = pow(distance(newDst, newSrc), 2.0);

        float gamma = (powRadius - powDistance / powRadius - powDistance + powNewDistance);
        float sigma = pow(gamma, 2.0);
        vec2 displacementVec = (newDst - newSrc) * sigma * u_reshapeRatio;
        pos = (newTex - displacementVec) / texSize;
    }

    return pos;
}

void main() {
    vec2 leftPreCtrl = u_preCtrlPoints.xy;
    vec2 rightPreCtrl = u_preCtrlPoints.zw;

    vec2 leftCurCtrl = u_curCtrlPoints.xy;
    vec2 rightCurCtrl = u_curCtrlPoints.zw;

    vec2 newTexCoord = face_slender_1(leftPreCtrl, leftCurCtrl, v_texCoord, u_reshapeRatio, u_texSize);
    newTexCoord = face_slender_1(rightPreCtrl, rightCurCtrl, newTexCoord, u_reshapeRatio, u_texSize);
    outColor = texture(s_TextureMap, newTexCoord);
}