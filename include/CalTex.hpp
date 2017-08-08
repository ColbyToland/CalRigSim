#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/aruco/charuco.hpp>

#include <string>

#include "config/TextureConfigData.hpp"

namespace epilog
{

class CalTex
{
public:
    CalTex(TextureConfigData& texConfig);

    cv::Mat& getTexture() { return m_image; }

private:
    CalTex() { }
    
    void setupImage(void);
    
    cv::Mat m_image;
};

} // namespace epilog
