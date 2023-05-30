#version 300 es

void main() {
    precision mediump float;
    in vec2 v_texCoord;
    layout (location = 0) out vec4 outColor;
    uniform sampler2D y_texture;
    uniform sampler2D uv_texture;
    vec3 yuv;
    yuv.x = texture(y_texture, v_texcoord).r;
    yuv.y = texture(uv_texture, v_texcoord).a - 0.5;
    yuv.z = texture(uv_texture, v_texcoord).r - 0.5;
    highp vec3 rgb = mat3(1, 1, 1, // 第一列
                          0, -0.344, 1.770, // 第二列
                          1.403, -0.714, 0  // 第三列
    ) * yuv;

    outColor = vec4(rgb, 1);
}