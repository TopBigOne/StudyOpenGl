#version 300 es

precision highp float;
// 从顶点着色器中输入
in vec2 v_texCoord;
layout (location = 0) out vec4 outColor;
// 采样器
uniform sampler2D s_TextureMap;
//点击的位置（归一化）
uniform vec2 u_TouchXY;
//纹理尺寸
uniform vec2 u_TexSize;
//归一化的时间
uniform float u_Time;
//边界 0.1
uniform float u_Boundary;
void main() {
    // 比例
    float ratio = u_TexSize.x / u_TexSize.y;
    // 根据纹理尺寸，对采用坐标进行转换
    vec2 texCoord = v_texCoord * vec2(1.0, ratio);
    //根据纹理尺寸，对中心点坐标进行转换
    vec2 touchXY = u_TouchXY * vec2(1.0, ratio);
    // 向量 texCoord 点 和 向量 touchXY 点的距离
    // 采样点坐标与中心点的距离
    float distance = distance(texCoord, touchXY);
    if (u_Time - u_Boundary > 0.0 &&
    distance <= u_Time + u_Boundary &&
    distance >= u_Time - u_Boundary) {
        float diff = distance - u_Time;
        // 采样坐标的距离
        float moveDistance = 20.0 * diff * (diff - u_Boundary) * (diff + u_Boundary);
        // 单位方向 向量
        vec2 unitDirectionVec = normalize(texCoord - touchXY);
        texCoord = texCoord + (unitDirectionVec * moveDistance);
    }
    texCoord = texCoord / vec2(1.0, ratio);
    outColor = texture(s_TextureMap, texCoord);

}