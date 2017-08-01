#include "CalData.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace epilog
{

// Singleton pattern code ///
CalData* CalData::s_pInstance = nullptr;

CalData::CalData()
    :   m_markerDict(cv::aruco::DICT_4X4_250),
        m_chessRows(8),
        m_chessCols(8),
        m_pxWidth(100),
        m_pxHeight(100)
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
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        vShaderFile.open(m_vertexShaderSourceFile);
        fShaderFile.open(m_fragmentShaderSourceFile);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		

        m_vertexShaderSource = vShaderStream.str();
        m_fragmentShaderSource = fShaderStream.str();

        vShaderFile.close();
        fShaderFile.close();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
}

} // namespace epilog
