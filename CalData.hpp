#pragma once

#include <opencv2/aruco/charuco.hpp>

#include <string>

#include "CameraModel.hpp"

namespace epilog
{

class CalData
{
// Singleton pattern code ///
private:
    static CalData* s_pInstance;

    // Remove default constructor and copy mechanisms
    CalData();
    CalData(CalData const&) {}
    CalData& operator=(CalData const&) {}

public:
    static CalData* getInstance();
/// End Singleton pattern code ///

    // Populate from a file
    bool readConfig(std::string filename);
    
    // Read shaders from text files
    void readShaders(void);

    // ChArUco Settings
    cv::aruco::PREDEFINED_DICTIONARY_NAME m_markerDict;
    size_t m_chessRows;
    size_t m_chessCols;
    size_t m_pxWidthTarget;
    size_t m_pxHeightTarget;

    // Shaders
    std::string m_vertexShaderSourceFile;
    std::string m_fragmentShaderSourceFile;
    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;

    std::string m_previewVSSourceFile;
    std::string m_previewFSSourceFile;
    std::string m_previewVSSource;
    std::string m_previewFSSource;

    // Camera parameters
    CameraModel m_camModel;

    // Preview window
    size_t m_previewWidth;
    size_t m_previewHeight;

    // Calibration images
    std::vector<cv::Mat> m_calImages;
    bool m_calImagesReady;
};

} // namespace epilog
