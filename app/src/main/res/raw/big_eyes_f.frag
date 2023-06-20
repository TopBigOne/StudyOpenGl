#version 100
precision highp float;
layout (location = 0) out vec4 outColor;
uniform sampler2D s_TextureMap;
in vec2 v_texCoord;
// 左眼中心点
uniform highp vec2 u_LeftEyeCenterPos;
// 右眼中心点
uniform highp vec2 u_RightEyeCenterPos;
// 放大系数
uniform highp float u_ScaleRatio;

// 影响半径
uniform highp float u_Radius;

// 图片分辨率
uniform vec2 u_ImageSize;
vec2 warpEyes(vec2 centerPos, vec2 curPos, float radius, float scaleRatio) {
    vec2 result = curPos;
    vec2 imaCurPos = curPos * u_ImageSize;
    // 两个向量之间的距离。
    float d = distance(imaCurPos, centerPos);

    if (d < radius) {
        // smoothstep
        float gamma = 1.0 - scaleRatio * pow(smoothstep(0.0, 1.0, d / radius) - 1.0, 2.0);
        result = centerPos + gamma * (imaCurPos - centerPos);
        result = result / u_ImageSize;
    }
    return result;

}


void main() {
    vec2 newTexCoord = warpEyes(u_LeftEyeCenterPos, v_texCoord, u_Radius, u_ScaleRatio);
    newTexCoord = warpEyes(u_RightEyeCenterPos, newTexCoord, u_Radius, u_ScaleRatio);
    outColor = texture(s_TextureMap, newTexCoord);

}