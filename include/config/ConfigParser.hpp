#pragma once

#include <opencv2/opencv.hpp>

#include <string>

#include "CalData.hpp"
#include "ConfigTags.hpp"

namespace epilog
{

class ConfigParser
{ 
public:
    static bool readFile(std::string filename);
    
private:
    /// static interface only ///
    ConfigParser(void) {}
    ConfigParser(ConfigParser const&) {}
    ConfigParser& operator=(ConfigParser const&) {} 
    /// end list of default functions to hide ///
    
    static void readCamSettings(cv::FileNode& cameraNode);
    static void readRendererSettings(cv::FileNode& rendererNode);
    static void readTextureSettings(cv::FileNode& textureNode);
    static bool readTargetSettings(cv::FileNode& targetNode);
    static void readCaptureSettings(cv::FileNode& captureNode);  
};

} /// namespace epilog
