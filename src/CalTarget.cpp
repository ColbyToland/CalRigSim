#include "CalTarget.hpp"

#include <opencv2/core.hpp>

#include "config/TargetConfigData.hpp"

#include "config/CalData.hpp"

#include "Debugger.hpp"

namespace epilog
{

CalTarget::CalTarget(int configInd) : m_textureID(0),
                                      m_configInd(configInd)
{
}

CalTarget::~CalTarget(void)
{
    if (m_textureID)
        glDeleteTextures(1, &m_textureID);
}

void CalTarget::setupData(void)
{
    CalData* config = CalData::getInstance();
    
    // Setup the vertex buffer, vertex array buffer, and element buffer for
        // offscreen rendering
    glGenVertexArrays(1, &m_VAO);
    
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    TargetConfigData* pData = &config->m_targetSettings.at(m_configInd);
    
    // Fill buffers
    size_t vertexBufferSz = 
        pData->m_vertexCount * TargetConfigData::VERTEX_SIZE * sizeof(float);
    glBufferData(   GL_ARRAY_BUFFER, 
                    vertexBufferSz, 
                    pData->m_vertexData.get(), 
                    GL_STATIC_DRAW );
                    
    size_t indexBufferSz = 
        pData->m_triCount * TargetConfigData::TRI_SIZE * sizeof(unsigned int);
    glBufferData(   GL_ELEMENT_ARRAY_BUFFER, 
                    indexBufferSz, 
                    pData->m_indices.get(), 
                    GL_STATIC_DRAW );

    // Describe the vertex array contents
    // position attribute
    glVertexAttribPointer(  0, 3, GL_FLOAT, GL_FALSE, 
                            TargetConfigData::VERTEX_SIZE * sizeof(float), 
                            (void*)0 );
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(  1, 2, GL_FLOAT, GL_FALSE, 
                            TargetConfigData::VERTEX_SIZE * sizeof(float), 
                            (void*)(TargetConfigData::POS_SIZE * sizeof(float)));
    glEnableVertexAttribArray(1);   

    setupTexture();
}

void CalTarget::draw(void) const
{
    CalData* config = CalData::getInstance();
    TargetConfigData* pData = &config->m_targetSettings.at(m_configInd);
    
    glBindVertexArray(m_VAO);
    if (m_textureID)
    { 
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }
        
    glDrawElements(GL_TRIANGLES, 
                    TargetConfigData::TRI_SIZE*pData->m_triCount, 
                    GL_UNSIGNED_INT, 
                    0);
}

void CalTarget::setupTexture(void)
{
    CalData* config = CalData::getInstance();
    TargetConfigData* pData = &config->m_targetSettings.at(m_configInd);
    cv::Mat texImg = config->m_textures.at(pData->m_texID).getTexture();
    
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
	if (texImg.channels() == 1)
	{
		colorFormat = GL_LUMINANCE;
	}
 
	// Create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                    texImg.cols, texImg.rows, 
                    0, colorFormat, GL_UNSIGNED_BYTE,
                    texImg.ptr());
}

} /// namespace epilog
