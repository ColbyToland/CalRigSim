#include <unistd.h>

#include "Renderman.hpp"
#include "config/CalData.hpp"

#include "Calibrator.hpp"

#include "Debugger.hpp"

namespace epilog
{

Renderman::Renderman(void) : m_shaderProgram(0)
{}

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

    // Setup calibration target data
    std::map<int, CalTarget>::iterator itr = config->m_targets.begin();
    std::map<int, CalTarget>::iterator endItr = config->m_targets.end();
    for (; itr != endItr; itr++)
    {
        itr->second.setupData();
    }
    
    // Setup preview pane
    GLuint quadVAO;
    setupPreview(quadVAO);

    // Create a framebuffer for offscreen rendering to a texture
    GLuint fbo;
    GLuint offscreenTextId;
    setupFramebuffer(fbo, offscreenTextId);

    // Array for retrieving the texture image
    size_t numBytes = config->m_camModel.m_width * config->m_camModel.m_height * 3;
    std::unique_ptr<GLubyte> texImg(new GLubyte[numBytes]);

    // Perspective Projection pipeline matrices
    glm::mat4 proj = projectiveCam(0.1f, 100.0f);
    glm::mat4 cameraTransform = composite(config->m_camModel.m_transforms);
    
    // Render so long as the window is open
    bool saveImages = true;
    size_t capInd = 0;
    size_t totalCaps = config->m_captures.size();
    
    for (capInd = 0; 
         capInd < totalCaps && !glfwWindowShouldClose(m_pwindow); 
         ++capInd)
    {
        // Get target information
        size_t targetInd = config->m_captures[capInd].m_targetID;
        
        // compose transform in model matrix
        glm::mat4 targetTransform = composite(config->m_targetSettings.at(targetInd).m_transforms);
        glm::mat4 captureTransform = composite(config->m_captures[capInd].m_transforms);
        glm::mat4 modelview = cameraTransform * captureTransform * targetTransform;
        
        // Offscreen render pass
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0,0,config->m_camModel.m_width,config->m_camModel.m_height);
        glClearColor(0.2f,0.2f,0.2f,1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glUseProgram(m_shaderProgram);
        
        int modelLoc = glGetUniformLocation(m_shaderProgram, "modelview");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelview));
        int projLoc = glGetUniformLocation(m_shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        
        config->m_targets.at(targetInd).draw();
        
        // Save rendering
        glBindTexture(GL_TEXTURE_2D, offscreenTextId);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, (void *)texImg.get());
        cv::Mat byteMat = cv::Mat(numBytes, 1, CV_8U, (void *)texImg.get()).clone();
        cv::Mat img = byteMat.reshape(3, config->m_camModel.m_height);
        cv::Mat distImg = img.clone();
        cv::Mat mapx, mapy;
        config->m_camModel.getDiffMap(mapx, mapy);
        remap(img, distImg, mapx, mapy, CV_INTER_CUBIC);
        config->m_calImages.push_back(distImg);
        if (saveImages)
        { 
            std::stringstream imgName;
            imgName << "output/cal_" << capInd << ".png";
            imwrite(imgName.str(), distImg);
        }
        
        // Preview render pass 
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,config->m_previewWidth,config->m_previewHeight);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_previewShaderProgram);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); 
        
        glfwSwapBuffers(m_pwindow);
        glfwPollEvents();
    }
    
    // If all images were captured, we are ready for calibration
    if (    (capInd == totalCaps) 
        &&  (totalCaps >= Calibrator::MIN_IMAGES) )
    {
        config->m_calImagesReady = true;
    }

    // Clean-up
    glDeleteFramebuffers(1, &fbo); 
    glfwTerminate();
}

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
    glViewport(0, 0, config->m_camModel.m_width, config->m_camModel.m_height);
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

glm::mat4 Renderman::convertCVtoGLCamera(cv::Mat& ocvCam, float near, float far)
{        
    // OpenCV Camera
    // [alpha   0       cx]
    // [0       beta    cy]
    // [0       0       1 ]
    
    // OpenGL Camera
    // [near/right  0           0                       0                       ]
    // [0           near/top    0                       0                       ]
    // [0           0           -(far+near)/(far-near)  0                       ]
    // [0           0           -1                      -2*far*near/(far-near)  ]

    glm::mat4 glCam;

    float m11 = ocvCam.at<float>(0,0) / ocvCam.at<float>(0,2); // alpha / cx
    float m22 = ocvCam.at<float>(1,1) / ocvCam.at<float>(1,2); // beta / cy
    float m33 = -(far+near)/(far-near);
    float m34 = -2*far*near/(far-near);
    
    glCam[0][0] = m11;
    glCam[1][1] = m22;
    glCam[2][2] = m33;
    glCam[3][2] = m34;
    glCam[2][3] = -1.0f;
    glCam[3][3] = 0.0f; // Loaded with identity by default

    return glCam;
}

glm::mat4 Renderman::projectiveCam(float near, float far)
{
    CalData* config = CalData::getInstance();
    
    cv::Mat ocvCam = cv::Mat::eye(3,3,CV_32F);
    ocvCam.at<float>(0,0) = config->m_camModel.m_focal_len;
    ocvCam.at<float>(1,1) = config->m_camModel.m_focal_len;
    ocvCam.at<float>(0,2) = config->m_camModel.m_width / 2.0f;
    ocvCam.at<float>(1,2) = config->m_camModel.m_height / 2.0f;
    
    return convertCVtoGLCamera(ocvCam, near, far);
}

void Renderman::setupPreview(GLuint& quadVAO)
{
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
    GLuint quadVBO;
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
}

void Renderman::setupFramebuffer(GLuint& fbo, GLuint& offscreenTextId)
{
    CalData* config = CalData::getInstance();
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &offscreenTextId);
    glBindTexture(GL_TEXTURE_2D, offscreenTextId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                    config->m_camModel.m_width, config->m_camModel.m_height, 
                    0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
                            offscreenTextId, 0);
}

glm::mat4 Renderman::composite(std::map<int, GeomTransform>& transforms)
{
    glm::mat4 modelview;
    
    for (auto kv : transforms)
    {
        if (GeomTransformType::ROTATION == kv.second.m_type)
        {
            modelview = glm::rotate(modelview, 
                                    glm::radians(kv.second.m_angle),
                                    glm::vec3(  kv.second.m_axis[0],
                                                kv.second.m_axis[1],
                                                kv.second.m_axis[2]));
        }
        else if (GeomTransformType::TRANSLATION == kv.second.m_type)
        {
            modelview = glm::translate( modelview,
                                        glm::vec3(  kv.second.m_axis[0],
                                                    kv.second.m_axis[1],
                                                    kv.second.m_axis[2]) );
        }
        else
        {
            std::cerr << "Invalid transform:" << std::endl;
            std::cerr << std::string(kv.second) << std::endl;
        }
    }
    
    return modelview;
}

} // namespace epilog
