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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}

bool Renderman::init()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_pwindow = glfwCreateWindow(m_camModel.width, m_camModel.height, 
                                "Calibration Target", 
                                NULL, NULL);
    if (m_pwindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_pwindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    } 
    
    glViewport(0, 0, m_camModel.width, m_camModel.height);

    glfwSetFramebufferSizeCallback(m_pwindow, framebuffer_size_callback);

    return true;
}

void Renderman::renderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pTarget->draw();
}

void Renderman::mainLoop(void)
{
    while(!glfwWindowShouldClose(m_pwindow))
    {
	    renderScene();

        glfwSwapBuffers(m_pwindow);
        glfwPollEvents();    
    }

    glfwTerminate();
}

} // namespace epilog
