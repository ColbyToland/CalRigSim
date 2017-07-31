#pragma once

#include <GL/glut.h>

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

    void init(int argc, char** argv);
    void renderScene(void);

private:
    CalTarget const *   m_pTarget;
    CameraModel         m_camModel;

};

} // namespace epilog
