#pragma once

#include "glad.h"

#include "CalTex.hpp"

#include "config/TargetConfigData.hpp"

namespace epilog
{

class CalTarget
{
public:
    CalTarget(TargetConfigData* pTargetConfig);

    ~CalTarget(void);

    void setupData(void);
    void draw(void) const;

private:
    CalTarget() : m_textureID(0) { }
    
    void setupTexture(void);
    
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

    cv::Mat m_texImg;
    GLuint  m_textureID;
    
    TargetConfigData* m_pData;
};

} // namespace epilog
