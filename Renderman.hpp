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
    static CalTarget m_defaultTarget;

    Renderman(void);
    Renderman(const CalTarget* target);
    Renderman(const CalTarget* target, const CameraModel& cam_model);

    bool init();
    void renderScene(void);
    void mainLoop(void);

private:
    GLFWwindow *        m_pwindow;
    CalTarget const *   m_pTarget;
    CameraModel         m_camModel;

};

} // namespace epilog
