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

    void setupData(void);
    void bindTexture(void) const;
    void draw(void) const;

    void setTexture(cv::Mat& texImg);
    void setTexture(CalTex& texture);

private:
    void setupTexture(void);

    cv::Mat m_texImg;
    GLuint  m_textureID;
};

} // namespace epilog
