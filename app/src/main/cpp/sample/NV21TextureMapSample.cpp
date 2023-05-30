//
// Created by dev on 2023/5/29.
//

#include "NV21TextureMapSample.h"
#include "GLUtils.h"


void NV21TextureMapSample::LoadImage(NativeImage *pImage) {
    if (pImage == nullptr) {
        return;
    }
    LOGCATI("NV21TextureMapSample LoadImage image format : %d", pImage->format);
    m_RenderImage.width = pImage->width;
    m_RenderImage.height = pImage->height;
    m_RenderImage.format = pImage->format;
    NativeImageUtil::copyNativeImage(pImage, &m_RenderImage);

}


void NV21TextureMapSample::Init() {
    char vShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 a_position;   \n"
            "layout(location = 1) in vec2 a_texCoord;   \n"
            "out vec2 v_texCoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = a_position;               \n"
            "   v_texCoord = a_texCoord;                \n"
            "}                                          \n";

    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 v_texCoord;                                 \n"
            "layout(location = 0) out vec4 outColor;             \n"
            "uniform sampler2D y_texture;                        \n"
            "uniform sampler2D uv_texture;                        \n"
            "void main()                                         \n"
            "{                                                   \n"
            "	vec3 yuv;										\n"
            "   yuv.x = texture(y_texture, v_texCoord).r;  	\n"
            "   yuv.y = texture(uv_texture, v_texCoord).a-0.5;	\n"
            "   yuv.z = texture(uv_texture, v_texCoord).r-0.5;	\n"
            "	highp vec3 rgb = mat3( 1,       1,       	1,					\n"
            "               0, 		-0.344, 	1.770,					\n"
            "               1.403,  -0.714,       0) * yuv; 			\n"
            "	outColor = vec4(rgb, 1);						\n"
            "}                                                   \n";


    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr,
                                          m_VertexShader, m_FragmentShader);
    // NV21 Y Plane 纹理的采样器
    m_ySampleLocation = glGetUniformLocation(m_ProgramObj, "y_texture");
    //NV21  UV Plane 纹理的采样器
    m_uvSampleLocation = glGetUniformLocation(m_ProgramObj, "uv_texture");
    // create textures
    GLuint textureIds[2] = {0};
    glGenTextures(2, textureIds);
    m_yTextureId = textureIds[0];
    m_uvTextureId = textureIds[1];

}

void NV21TextureMapSample::Draw(int screenW, int screenH) {
    LOGCATE("NV21TextureMapSample::Draw()");
    if (m_ProgramObj == GL_NONE ||
        m_yTextureId == GL_NONE || m_uvTextureId == GL_NONE) {
        return;
    }

    // upload Y plane data
    glBindTexture(GL_TEXTURE_2D, m_yTextureId);

    /**
     * - target 变量设置成  GL_TEXTURE_2D;
        - level 指明纹理的层次，可以设置成0，
        - internalforamt 指的是纹理数据在OpenGL中是如何表示的，如GL_RGB就表示纹理的像素在OpenGL里面以红绿蓝三个分量表示；
        - width 和 height 指明了载入纹理时候，数据所包含的纹理的宽度和高度；
        - border 指的是纹理的边界，可以设置成0；
        - format 指的是载入纹理的格式，它告诉OpenGL外部数据是如何表示单个像素的，
           - 如GL_RED表示数据每个像素只有红色分量，GL_RGBA表示是红绿蓝alpha四个分量组成一个像素；
        - type：  表示一个像素中每个分量所占用的空间大小，如GL_FLOAT就表示每个分量是一个浮点数；GL_UNSIGNED_BYTE表示每个分量用一个无符号byte表示；
        - data ：外部纹理数据的起始地址。
     */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE,
                 m_RenderImage.width, m_RenderImage.height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 m_RenderImage.ppPlane[0]);
    // s,t 轴
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);




    // update uv plane data
    glBindTexture(GL_TEXTURE_2D, m_uvTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, m_RenderImage.width >> 1,
                 m_RenderImage.height >> 1, 0,
                 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);


    // 顶底坐标
    GLfloat verticesCoords[] = {
            -1.0f, 0.78f, 0.0f,  // Position 0
            -1.0f, -0.78f, 0.0f,  // Position 1
            1.0f, -0.78f, 0.0f,  // Position 2
            1.0f, 0.78f, 0.0f,  // Position 3
    };

    // 纹理坐标
    GLfloat textureCoords[] = {
            0.0f, 0.0f,        // TexCoord 0
            0.0f, 1.0f,        // TexCoord 1
            1.0f, 1.0f,        // TexCoord 2
            1.0f, 0.0f         // TexCoord 3
    };
    // 渲染路径？
    GLushort indices[] = {0, 1, 2, 0, 2, 3};
    // use the program object
    glUseProgram(m_ProgramObj);
    // load the vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), verticesCoords);
    // load texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textureCoords);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // 激活纹理 0；
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_yTextureId);
    // set the Y plane sampler to texture unit to 0;
    glUniform1i(m_ySampleLocation,0);

    // 激活纹理 1;
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_uvTextureId);

    // set the uv plane sampler to texture unit to 1;
    glUniform1i(m_uvSampleLocation,1);

    // 6: 使用count个成序列的元素来创建一系列的几何图元
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);


}

void NV21TextureMapSample::Destroy() {
    if (m_ProgramObj)
    {
        glDeleteProgram(m_ProgramObj);
        glDeleteTextures(1, &m_yTextureId);
        glDeleteTextures(1, &m_uvTextureId);
        m_ProgramObj = GL_NONE;
    }

}
