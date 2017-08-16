#pragma once

#include <opencv2/opencv.hpp>

#include <map>
#include <string>
#include <sstream>

#include "GeomTransform.hpp"

namespace epilog
{

#define DIFF_COEFF_SZ   5

class CameraModel
{
public:
    const size_t BORDER_SZ = 200;

    CameraModel();

    void setDiffCoeffs(float* diffCoeffs);
    void getDiffCoeffs(float* diffCoeffs);
    
    void setSize(size_t w, size_t h);
    
    void applyDiffMap(cv::Mat& renderImg, cv::Mat& distImg);

    operator std::string() const;

    size_t m_width;
    size_t m_height;
    size_t m_rendwidth;
    size_t m_rendheight;
    double m_focal_len;
    std::map<int, GeomTransform> m_transforms;

private:
    void genDiffMaps(void);

    float m_diffCoeffs[DIFF_COEFF_SZ];
    cv::Mat m_map_x;
    cv::Mat m_map_y;
};

}
