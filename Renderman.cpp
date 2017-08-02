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

bool Renderman::initGLFW(void)
{
    CalData* config = CalData::getInstance();

    glfwInit();

    // Create a GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_pwindow = glfwCreateWindow(config->m_previewWidth, 
                                    config->m_previewHeight, 
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

    return true;
}

bool Renderman::initOffscreenShaders(void)
{
    CalData* config = CalData::getInstance();

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
        return false;
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
        return false;
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
        return false;
    }

    // Assign Shader Program
    glUseProgram(m_shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool Renderman::initPreviewShaders(void)
{
    CalData* config = CalData::getInstance();

    // Setup Preview Vertex Shader
    int  success;
    char infoLog[512];
    const char* previewVSCode = config->m_previewVSSource.c_str();
    GLuint previewVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(previewVertexShader, 1, &previewVSCode, NULL);
    glCompileShader(previewVertexShader);
    glGetShaderiv(previewVertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(previewVertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PREVIEWVERTEX::COMPILATION_FAILED\n" << infoLog 
                    << std::endl;
        return false;
    }

    // Setup Preview Fragment Shader
    const char* previewFSCode = config->m_previewFSSource.c_str();
    GLuint previewFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(previewFragmentShader, 1, &previewFSCode, NULL);
    glCompileShader(previewFragmentShader);
    glGetShaderiv(previewFragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(previewFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PREVIEWFRAGMENT::COMPILATION_FAILED\n" << infoLog 
                    << std::endl;
        return false;
    }

    // Setup Preview Shader Program
    m_previewShaderProgram = glCreateProgram();
    glAttachShader(m_previewShaderProgram, previewVertexShader);
    glAttachShader(m_previewShaderProgram, previewFragmentShader);
    glLinkProgram(m_previewShaderProgram);
    glGetProgramiv(m_previewShaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_previewShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PREVIEWPROGRAM::LINK_FAILED\n" << infoLog 
                    << std::endl;
        return false;
    }

    // Assign Preview Shader Program
    glUseProgram(m_previewShaderProgram);
    glDeleteShader(previewVertexShader);
    glDeleteShader(previewFragmentShader);

    return true;
}

bool Renderman::init(void)
{
    CalData* config = CalData::getInstance();

    bool success = true;

    success = initGLFW();
    if (!success)
    {
        std::cout << "initGLFW failed" << std::endl;
        return false;
    }

    success = initOffscreenShaders();
    if (!success)
    {
        std::cout << "initOffscreenShaders failed" << std::endl;
        return false;
    } 

    success = initPreviewShaders();
    if (!success)
    {
        std::cout << "initPreviewShaders failed" << std::endl;
        return false;
    }

    return true;
}

void Renderman::mainLoop(void)
{  
    CalData* config = CalData::getInstance();

    // Perspective Projection pipeline matrices
    float aspectRatio = 
        (float)config->m_camModel.width / (float)config->m_camModel.height;
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 
                                        aspectRatio, 
                                        0.1f, 100.0f);

    glm::mat4 model;
    model = glm::rotate(model, glm::radians(-45.0f), 
                        glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));    

    float texAspectRatio = 
        (float)config->m_pxWidthTarget / (float)config->m_pxHeightTarget;
    glm::mat4 texturePrescaler;
    float scaleFactor = 7.0f;
    texturePrescaler[0][0] = scaleFactor*texAspectRatio;
    texturePrescaler[1][1] = scaleFactor;

    // Setup the vertex buffer, vertex array buffer, and element buffer for
        // offscreen rendering
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Setup calibration target data
    m_pTarget->setupData();

    // Setup preview "screen" quad    
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 
                            (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 
                            (void*)(2 * sizeof(float)));

    // Create a framebuffer for offscreen rendering to a texture
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo); // Set frame buffer
    GLuint offscreenTextId;
    glGenTextures(1, &offscreenTextId);
    glBindTexture(GL_TEXTURE_2D, offscreenTextId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                    config->m_camModel.width, config->m_camModel.height, 
                    0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
                            offscreenTextId, 0);

    // Render so long as the window is open
    while(!glfwWindowShouldClose(m_pwindow))
    {
        /// Temporary rotation animation code               ///
        model = glm::rotate(model, glm::radians(-1.0f),     ///
                            glm::vec3(0.0f, 0.5f, -1.0f));  ///
        /// Temporary rotation animation code               ///

        // Offscreen render pass
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glUseProgram(m_shaderProgram);

        int prescaleLoc = glGetUniformLocation(m_shaderProgram, "prescaler");
        glUniformMatrix4fv(prescaleLoc, 1, GL_FALSE, glm::value_ptr(texturePrescaler));
        int modelLoc = glGetUniformLocation(m_shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(m_shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(m_shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(VAO);
        m_pTarget->bindTexture();
        m_pTarget->draw();

        // Preview render pass 
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_previewShaderProgram);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, offscreenTextId);
        glDrawArrays(GL_TRIANGLES, 0, 6); 

        glfwSwapBuffers(m_pwindow);
        glfwPollEvents();    
    }

    glDeleteFramebuffers(1, &fbo); 

    glfwTerminate();
}

} // namespace epilog
