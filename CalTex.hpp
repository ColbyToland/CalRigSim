#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/aruco/charuco.hpp>

namespace epilog
{

class CalTex
{
public:
    CalTex();

    cv::Ptr<cv::aruco::CharucoBoard> m_pboard;
    cv::Mat m_image;

private:
};

} // namespace epilog
