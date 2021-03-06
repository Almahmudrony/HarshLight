#include "Texture.h"
#include "SOIL/SOIL.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>

Texture2d::Texture2d(const char * path)
{
    m_TexObject = 0;
    m_RawPath = nullptr;

#ifdef _DEBUG
    assert(path != nullptr);
#endif
    int32_t width, height, channel;
    uint8_t* image = SOIL_load_image(path, &width, &height, &channel, SOIL_LOAD_RGB);
    if (!image)
    {
        fprintf(stderr, "fail to load texture: %s\n", path);
        return;
    }
    
    m_RawPath = path;
    glGenTextures(1, &m_TexObject);
#ifdef _DEBUG
    assert(m_TexObject);
#endif
    glBindTexture(GL_TEXTURE_2D, m_TexObject);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2d::Texture2d(uint32_t width, uint32_t height, const uint32_t color)
{
	m_RawPath = nullptr;
	glGenTextures(1, &m_TexObject);
#ifdef _DEBUG
	assert(m_TexObject);
#endif
	glBindTexture(GL_TEXTURE_2D, m_TexObject);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glClearTexImage(m_TexObject, 0, GL_RGB, GL_UNSIGNED_BYTE, &color);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2d::~Texture2d()
{
    if (m_TexObject)
    {
        glDeleteTextures(1, &m_TexObject);
        m_TexObject = 0;
    }
}

GLuint Texture2d::GetTexObj() const
{
    return m_TexObject;
}

Texture3dCompute::Texture3dCompute(uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, GLuint internal_format, GLuint format, GLuint type, uint32_t filter)
	:m_DimX(dim_x), m_DimY(dim_y), m_DimZ(dim_z), m_InternalFormat(internal_format), m_Format(format), m_Type(type), m_TexObject(0), m_Filter(filter)
{
#ifdef _DEBUG
	assert(m_DimX && m_DimY && m_DimZ);
#endif
	glGenTextures(1, &m_TexObject);
	glBindTexture(GL_TEXTURE_3D, m_TexObject);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	switch (m_Filter)
	{
	case TextureFilter::kTrilinear:
	{
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLsizei level = 1;
		while (dim_x >>= 1) ++level;
		glTexStorage3D(GL_TEXTURE_3D, level, internal_format, m_DimX, m_DimY, m_DimZ);
		glGenerateMipmap(GL_TEXTURE_3D);
		break;
	}
	case TextureFilter::kBilinear:
	{
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D, 0, internal_format, m_DimX, m_DimY, m_DimZ, 0, format, type, nullptr);
		break;
	}
	case TextureFilter::kPoint:default:
	{
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage3D(GL_TEXTURE_3D, 0, internal_format, m_DimX, m_DimY, m_DimZ, 0, format, type, nullptr);
		break;
	}
	}
	glBindTexture(GL_TEXTURE_3D, 0);

}

Texture3dCompute::~Texture3dCompute()
{
	if (m_TexObject)
	{
		glDeleteTextures(1, &m_TexObject);
		m_TexObject = 0;
	}
}

void Texture3dCompute::CleanContent(GLuint clean_color)
{
//	GLuint clear_color[] = { 0,0,0,0 }; // black and transparent
	glClearTexImage(m_TexObject, 0, m_Format, m_Type, &clean_color);

	//glBindFramebuffer(GL_FRAMEBUFFER, m_UtilFBO);
	//
	//glClearBufferuiv(GL_COLOR, 0, clear_color);

	////glClearColor(1.0f, 0.0f, 0.635f, 1.0f);  //pink
	////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Texture3dCompute::GetTexObj() const
{
	return m_TexObject;
}

GLuint Texture3dCompute::GetInternalFormat() const
{
	return m_InternalFormat;
}
