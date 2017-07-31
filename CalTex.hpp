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
    CalTex() { }

    void genChArUco(cv::aruco::PREDEFINED_DICTIONARY_NAME dict,
                    size_t rows, size_t cols);
    void genTexture(size_t width, size_t height);

    cv::Mat& getTexture() { return m_image; }

private:
    cv::aruco::Dictionary m_dictionary;
    cv::Ptr<cv::aruco::CharucoBoard> m_pboard;
    cv::Mat m_image;
};

} // namespace epilog
