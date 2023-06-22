#version 300 es

layout (location = 0) in float a_tData;
uniform vec4 u_StartEndPoints;
uniform vec4 u_ControlPoints;
uniform vec4 u_MVPMatrix;
uniform float u_offset;

// 有2个算法： 使用 de Casteljau【德卡斯特里奥】算法或 Bézier clipping 算法。
// https://liolok.com/zhs/bezier-curve-and-de-casteljau-algorithm/
/**
使用了三次幂函数的形式来计算贝塞尔曲线上的点，
其中 P0、P1、P2、P3 分别是曲线的起点、第一个控制点、第二个控制点和终点，t 是曲线上的参数，范围在 0 到 1 之间。
这种方式计算简单，但是需要进行多次幂函数运算，可能会导致性能不佳。
*/
vec2 bezier3(in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p3, in float t) {
    float tt = 1.0 - t;
    return pow(tt, 3.0) * p0 + 3 * t * pow(tt, 2.0) * p1 + 3 * pow(t, 2.0) * tt * p2 + pow(tt, 3.0) * p3;
}

/**
使用了 mix 函数来计算贝塞尔曲线上的点，即先计算出曲线上的两个控制点 q0 和 q1，
然后再计算出曲线上的点。这种方式计算相对简单，但是需要进行多次向量插值运算，也可能会导致性能不佳。
*/
vec2 bezier3_(in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p3, float t) {
    // mix(p0, p1, t) 返回的是一个新的二维向量 q0，表示起点和终点之间插值后的点。具体来说，
    // 如果 t=0，则 q0=p0；如果 t=1，则 q0=p1；如果 t=0.5，则 q0 是 p0 和 p1 的平均值。
    // 在这个代码片段中，q0 表示曲线上的第一个控制点。
    vec2 q0 = mix(p0, p1, t);
    vec2 q1 = mix(p1, p2, t);
    vec2 q2 = mix(p2, p3, t);
    vec2 r0 = mix(q0, q1, t);
    vec2 r1 = mix(q1, q2, t);
    return mix(r0, r1, t);
}

void main() {
/**
在 3D 图形编程中，向量通常是由三个分量 x、y、z 组成的。
为了方便进行矩阵变换，有时候会使用四维向量来表示点或向量，
即使用一个四元组 (x, y, z, w) 来表示。其中 w 分量通常被称为齐次坐标（homogeneous coordinate）。
齐次坐标有一个重要的性质，就是通过除以 w 分量可以将其转换为三维坐标。
具体来说，如果一个四元组 (x, y, z, w) 的 w 分量不为零，那么可以将其转换为三元组 (x/w, y/w, z/w)，
即通过将 x、y、z 分量分别除以 w 分量来获得三维坐标。
这个过程称为齐次除法（homogenization）。
在这段代码中，pos.w = 1.0 是将 pos 向量的第四个分量 w 设置为 1.0。
这个操作是为了将 pos 向量转换为一个点，
即将其视为一个四元组 (x, y, z, w) 中的点，其中 w 分量设置为 1.0。
这样一来，如果需要对这个点进行矩阵变换，就可以使用齐次坐标来表示，方便进行变换计算。
*/
    vec4 pos;
    pos.w = 1.0;
    vec2 p0 = u_startendpoints.xy;
    vec2 p3 = u_startendpoints.zw;
    vec2 p1 = u_controlpoints.xy;
    vec2 p2 = u_controlpoints.zw;

    p0.y *= u_offset;
    p1.y *= u_offset;
    p2.y *= u_offset;
    p3.y *= u_offset;


    float t = a_tData;
    vec2 point = bezier3_(p0, p1, p2, p3, t);
    float f_zero = 0.0f;
    if (t < f_zero) {
        pos.xy = vec2(f_zero, f_zero);
    } else {
        pos.xy = point;
    }
    gl_PointSize = 4.0;
    gl_Position = u_MVPMatrix * pos;


}