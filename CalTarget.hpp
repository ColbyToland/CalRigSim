#pragma once

#include "glad.h"

#include "CalTex.hpp"

namespace epilog
{

class CalTarget
{
public:
    CalTarget() : m_textureID(0) { }
    CalTarget(CalTex& texture) : m_textureID(0) 
    { 
        setTexture(texture); 
    }

    ~CalTarget(void);

    void setTexture(cv::Mat& texImg);
    void setTexture(CalTex& texture);

    void draw(void) const;

private:
    cv::Mat m_texImg;
    GLuint  m_textureID;
};

} // namespace epilog
