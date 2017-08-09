#pragma once

#include "glad.h"

#include "CalTex.hpp"

#include "config/TargetConfigData.hpp"

namespace epilog
{

class CalTarget
{
public:
    CalTarget(int configInd);

    ~CalTarget(void);

    void setupData(void);
    void draw(void) const;

private:
    CalTarget() : m_textureID(0) { }
    
    void setupTexture(void);
    
    GLuint m_VAO;

    GLuint  m_textureID;
    
    int m_configInd;
};

} // namespace epilog
