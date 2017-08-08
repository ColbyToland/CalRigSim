#include "config/CalData.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

namespace epilog
{

// Singleton pattern code ///
CalData* CalData::s_pInstance = nullptr;

CalData::CalData()
    :   m_previewWidth(100),
        m_previewHeight(100),
        m_calImagesReady(false),
        m_calFlags(0),
        m_calRepError(std::numeric_limits<double>::max())
{
}

CalData* CalData::getInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CalData;
    }
    return s_pInstance;
}
/// End Singleton pattern code ///

void CalData::readShaders(void)
{
    // Setup input streams
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream previewVShaderFile;
    std::ifstream previewFShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    previewVShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    previewFShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // Read shader files
        vShaderFile.open(m_vertexShaderSourceFile);
        fShaderFile.open(m_fragmentShaderSourceFile);
        previewVShaderFile.open(m_previewVSSourceFile);
        previewFShaderFile.open(m_previewFSSourceFile);

        std::stringstream vShaderStream, fShaderStream;
        std::stringstream previewVSStream, previewFSStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        previewVSStream << previewVShaderFile.rdbuf();
        previewFSStream << previewFShaderFile.rdbuf();		

        // Save shader source code to strings
        m_vertexShaderSource = vShaderStream.str();
        m_fragmentShaderSource = fShaderStream.str();
        m_previewVSSource = previewVSStream.str();
        m_previewFSSource = previewFSStream.str();

        // Clean-up
        vShaderFile.close();
        fShaderFile.close();	
        previewVShaderFile.close();
        previewFShaderFile.close();	
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
}

} // namespace epilog
