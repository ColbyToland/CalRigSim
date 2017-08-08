#pragma once

#include <opencv2/aruco/charuco.hpp>

#include <string>
#include <sstream>

namespace epilog
{

enum class TextureType { IMAGE_TEXTURE, CHARUCO_TEXTURE, INVALID };

class TextureConfigData
{
public:
    TextureConfigData(void) :   m_type(TextureType::CHARUCO_TEXTURE), 
                                m_markerDict(cv::aruco::DICT_4X4_1000),
                                m_chessRows(8),
                                m_chessCols(8),
                                m_chessSqSz(0.04f), 
                                m_arucoSz(0.02f), 
                                m_pxWidthTarget(100),
                                m_pxHeightTarget(100)
    {}

    operator std::string() const
    {
        std::stringstream classStr;
        classStr << "Texture:\n";
        classStr << "\tChess Rows: " << m_chessRows << "\n";
        classStr << "\tChess Cols: " << m_chessCols << "\n";
        classStr << "\tChess Square Size: " << m_chessSqSz << "\n";
        classStr << "\tArUco Size: " << m_arucoSz << "\n";
        classStr << "\tPixel Width: " << m_pxWidthTarget << "\n";
        classStr << "\tPixel Height: " << m_pxHeightTarget << "\n";
        return classStr.str();
    }

    TextureType m_type;
    
    // Conversion functions
    static TextureType mapStringToTextureType(std::string typeStr);
    static cv::aruco::PREDEFINED_DICTIONARY_NAME mapStringToArUcoDict(std::string dictStr);
    
    // Image texture settings
    std::string m_imgFilename;
    
    // Charuco texture settings
    cv::aruco::PREDEFINED_DICTIONARY_NAME m_markerDict;
    int     m_chessRows;
    int     m_chessCols;
    float   m_chessSqSz; 
    float   m_arucoSz; 
    int     m_pxWidthTarget;
    int     m_pxHeightTarget;

private:
};

} /// namespace epilog
