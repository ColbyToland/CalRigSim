#include "CalTarget.hpp"

namespace epilog
{

CalTarget::~CalTarget(void)
{
    if (m_textureID)
        glDeleteTextures(1, &m_textureID);
}

void CalTarget::setupData(void)
{
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 
                    GL_STATIC_DRAW);

    // Describe the vertex array contents
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    setupTexture();
}

void CalTarget::draw(void) const
{
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void CalTarget::bindTexture(void) const
{
    if (m_textureID) glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void CalTarget::setTexture(cv::Mat& texImg)
{
    texImg.copyTo(m_texImg);
}

void CalTarget::setTexture(CalTex& texture)
{
    setTexture(texture.getTexture());
}

void CalTarget::setupTexture(void)
{
    if (m_textureID)
        glDeleteTextures(1, &m_textureID);

    // setup texture
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
}

} // namespace epilog
