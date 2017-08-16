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
    CameraModel();

    void setDiffCoeffs(float* diffCoeffs);
    void getDiffCoeffs(float* diffCoeffs);
    
    void getDiffMap(cv::Mat& mapx, cv::Mat& mapy);

    operator std::string() const;

    size_t m_width;
    size_t m_height;
    double m_focal_len;
    std::map<int, GeomTransform> m_transforms;

private:
    void genDiffMaps(void);

    float m_diffCoeffs[DIFF_COEFF_SZ];
    cv::Mat m_map_x;
    cv::Mat m_map_y;
};

}
