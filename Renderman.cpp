#include "Renderman.hpp"
#include "CalData.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

namespace epilog
{

CalTarget Renderman::s_defaultTarget = CalTarget();

// "Delegating Constructor"
// ========================
//  All other constructors call this constructor to initialize variables
Renderman::Renderman(CalTarget* target)
    :   m_pTarget(target),
        m_shaderProgram(0)
{
}

Renderman::Renderman(void) 
    : Renderman(&s_defaultTarget)
{}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}

bool Renderman::init()
{
    CalData* config = CalData::getInstance();

    glfwInit();

    // Create a GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_pwindow = glfwCreateWindow(config->m_camModel.width, 
                                    config->m_camModel.height, 
                                    "Calibration Target", 
                                    NULL, NULL);
    if (m_pwindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_pwindow);

    // Setup OpenGL hardward specific functions via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    } 
    
    // Setup resize callback
    glViewport(0, 0, config->m_camModel.width, config->m_camModel.height);
    glfwSetFramebufferSizeCallback(m_pwindow, framebuffer_size_callback);

    // Setup Vertex Shader
    int  success;
    char infoLog[512];
    const char* vertexShaderCode = config->m_vertexShaderSource.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog 
                    << std::endl;
    }

    // Setup Fragment Shader
    const char* fragmentShaderCode = config->m_fragmentShaderSource.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog 
                    << std::endl;
    }

    // Setup Shader Program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog 
                    << std::endl;
    }

    // Assign Shader Program
    glUseProgram(m_shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);            

    return true;
}

void Renderman::mainLoop(void)
{  
    CalData* config = CalData::getInstance();

    // Perspective Projection
    float aspectRatio = 
        (float)config->m_camModel.width / (float)config->m_camModel.height;
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 
                                        aspectRatio, 
                                        0.1f, 100.0f);
    std::cout << glm::to_string(proj) << std::endl;
    float texAspectRatio = (float)config->m_pxWidth / (float)config->m_pxHeight;
    glm::mat4 texturePrescaler;
    float scaleFactor = 7.0f;
    texturePrescaler[0][0] = scaleFactor*texAspectRatio;
    texturePrescaler[1][1] = scaleFactor;
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(-45.0f), 
                        glm::vec3(1.0f, 0.0f, 0.0f));
    std::cout << glm::to_string(model) << std::endl;
    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    std::cout << glm::to_string(view) << std::endl;

    glm::vec4 testPt(0.5f, 0.5f, 0.0f, 1.0f);   

    glm::vec4 result = /*proj*/model/*view*/*testPt;
    std::cout << glm::to_string(result) << std::endl;     

    // Setup Vertex Array buffer
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Setup Vertex Buffer
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Setup Element Buffer Object
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Setup target data
    m_pTarget->setupData();

    // Render so long as the window is open
    while(!glfwWindowShouldClose(m_pwindow))
    {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

        // Set rendering path
        glUseProgram(m_shaderProgram);


        model = glm::rotate(model, glm::radians(-1.0f), 
                            glm::vec3(0.0f, 0.0f, -1.0f));

        // Set vertex pipeline matrices
        int prescaleLoc = glGetUniformLocation(m_shaderProgram, "prescaler");
        glUniformMatrix4fv(prescaleLoc, 1, GL_FALSE, glm::value_ptr(texturePrescaler));
        int modelLoc = glGetUniformLocation(m_shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(m_shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(m_shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Setup buffers
        m_pTarget->bindTexture();
        glBindVertexArray(VAO);

        // Draw target
        m_pTarget->draw();

        glfwSwapBuffers(m_pwindow);
        glfwPollEvents();    
    }

    glfwTerminate();
}

} // namespace epilog
