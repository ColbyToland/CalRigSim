#pragma once

#include "glad.h"

#include <GLFW/glfw3.h>

#include "CalTarget.hpp"
#include "CameraModel.hpp"

namespace epilog
{

class Renderman
{
public:
    static CalTarget s_defaultTarget;

    Renderman(CalTarget* target);
    Renderman(void);

    bool init();
    void mainLoop(void);

private:
    GLFWwindow *    m_pwindow;
    CalTarget *     m_pTarget;

    GLuint  m_shaderProgram;
    GLuint  m_previewShaderProgram;

    bool initGLFW(void);
    bool initOffscreenShaders(void);
    bool initPreviewShaders(void);

};

} // namespace epilog
