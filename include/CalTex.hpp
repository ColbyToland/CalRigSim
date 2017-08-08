#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/aruco/charuco.hpp>

#include <string>

namespace epilog
{

class CalTex
{
public:
    CalTex() { }

    void genChArUco();
    void readImage(std::string filename);

    cv::Mat& getTexture() { return m_image; }

private:
    cv::Mat m_image;
};

} // namespace epilog
