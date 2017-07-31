#include "Renderman.hpp"

namespace epilog
{

CalTarget Renderman::m_defaultTarget = CalTarget();

Renderman::Renderman(void) :    m_pTarget(&m_defaultTarget), 
                                m_camModel()
{
}

Renderman::Renderman(const CalTarget* target) : m_pTarget(target), 
                                                m_camModel()
{
}

Renderman::Renderman(const CalTarget* target, const CameraModel& camModel)
    :   m_pTarget(target),
        m_camModel(camModel)
{
}

void Renderman::init(int argc, char** argv)
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(m_camModel.width,m_camModel.height);
	glutCreateWindow("Calbration Target");
}

void Renderman::renderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pTarget->draw();

    glutSwapBuffers();
}

} // namespace epilog
