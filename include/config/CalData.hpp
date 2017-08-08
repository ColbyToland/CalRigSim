#pragma once

#include <opencv2/aruco/charuco.hpp>

#include <map>
#include <string>
#include <sstream>

#include "CameraModel.hpp"
#include "TextureConfigData.hpp"
#include "TargetConfigData.hpp"
#include "CapturePositionConfig.hpp"

#include "CalTex.hpp"
#include "CalTarget.hpp"

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

    operator std::string() const;
    
    // Populate shaders, textures, and targets
    void setupData(void);
    
    // Read shaders from text files
    void readShaders(void);
    
    // Generate render data
    void generateTextures(void);
    void generateTargets(void);

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
    std::map<int, TextureConfigData> m_textureSettings;
    std::map<int, CalTex> m_textures;

    // Target settings
    std::map<int, TargetConfigData> m_targetSettings;
    std::map<int, CalTarget> m_targets;
    
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
