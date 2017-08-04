#pragma once

#include "glad.h"

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

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
    
    glm::mat4 convertCVtoGLCamera(cv::Mat& ocvCam, float near, float far);
    glm::mat4 projectiveCam(float near, float far);

};

} // namespace epilog
