#pragma once

#include <opencv2/aruco/charuco.hpp>

#include <map>
#include <string>
#include <sstream>

#include "CameraModel.hpp"
#include "TextureConfigData.hpp"
#include "TargetConfigData.hpp"
#include "CapturePositionConfig.hpp"

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

    operator std::string() const
    {
        std::stringstream classStr;
        classStr << "CalData:\n";
        classStr << std::string(m_camModel);
        classStr << "\n";
        classStr << "Renderer Settings:\n";
        classStr << "\tVertex Shader Source: " << m_vertexShaderSourceFile << "\n";
        classStr << "\tFragment Shader Source: " << m_fragmentShaderSourceFile << "\n";
        classStr << "\tPreview Vertex Shader Source: " << m_previewVSSourceFile << "\n";
        classStr << "\tPreview Fragment Shader Source: " << m_previewFSSourceFile << "\n";
        classStr << "\tPreview Window Width: " << m_previewWidth << "\n";
        classStr << "\tPreview Window Height: " << m_previewHeight << "\n";
        classStr << "\n";
        classStr << "Textures:\n";
        for (auto key : m_textures)
        {
            classStr << key.first << " - ";
            classStr << std::string(key.second);
            classStr << "\n";
        }
        classStr << "Targets:\n";
        for (auto key : m_targets)
        {
            classStr << key.first << " - ";
            classStr << std::string(key.second);
            classStr << "\n";
        }
        classStr << "Captures:\n";
        for (auto cap : m_captures)
        {
            classStr << std::string(cap);
            classStr << "\n";
        }
        return classStr.str();
    }
    
    // Read shaders from text files
    void readShaders(void);

    // Camera parameters
    CameraModel m_camModel;

    // Shaders
    std::string m_vertexShaderSourceFile;
    std::string m_fragmentShaderSourceFile;
    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;

    std::string m_previewVSSourceFile;
    std::string m_previewFSSourceFile;
    std::string m_previewVSSource;
    std::string m_previewFSSource;

    // Preview window
    size_t m_previewWidth;
    size_t m_previewHeight;

    // Texture settings
    std::map<int, TextureConfigData> m_textures;

    // Target settings
    std::map<int, TargetConfigData> m_targets;
    
    // Capture settings
    std::vector<CapturePositionConfig> m_captures;

    // Calibration images
    std::vector<cv::Mat> m_calImages;
    bool m_calImagesReady;
    
    // Calibration results
    cv::Mat m_calCamMatrix;
    cv::Mat m_calDistCoeffs;
    std::vector<cv::Mat> m_calRMats;
    std::vector<cv::Mat> m_calTMats;
    int m_calFlags;
    double m_calRepError;
};

} // namespace epilog
