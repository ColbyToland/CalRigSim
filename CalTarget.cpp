#include "CalTarget.hpp"

namespace epilog
{

CalTarget::~CalTarget(void)
{
    //if (m_textureID)
    //    glDeleteTextures(1, &m_textureID);
}

void CalTarget::draw(void) const
{
/*
    glBindTexture(GL_TEXTURE_2D, m_textureID);

	glBegin(GL_TRIANGLES);
        glTexCoord2f(0, 0);
		glVertex3f(0.0,     0.0,    0.0);
        glTexCoord2f(1, 0);
		glVertex3f(0.5,     0.0,    0.0);
        glTexCoord2f(0, 1);
		glVertex3f(0.0,     0.5,    0.0);
	glEnd();

    glDisable(GL_TEXTURE_2D);
*/
}

void CalTarget::setTexture(cv::Mat& texImg)
{
    texImg.copyTo(m_texImg);
    imwrite("test.png", m_texImg);
/*
    // Setup OpenGL texture
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float borderColor[] = {0.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLenum colorFormat = GL_BGR;
	if (m_texImg.channels() == 1)
	{
		colorFormat = GL_LUMINANCE;
	}
 
	// Create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                    m_texImg.cols, m_texImg.rows, 
                    0, colorFormat, GL_UNSIGNED_BYTE,
                    m_texImg.ptr());
*/
}

void CalTarget::setTexture(CalTex& texture)
{
    setTexture(texture.getTexture());
}

} // namespace epilog
