#include <string>
#include <sstream>

#include "config/CameraModel.hpp"

using namespace std;
using namespace cv;

namespace epilog
{

CameraModel::CameraModel() : m_width(1024), 
                m_height(768), 
                m_focal_len(430)
{
    memset(m_diffCoeffs, 0, DIFF_COEFF_SZ*sizeof(float));
    m_map_x.create(m_height, m_width, CV_32FC1);
    m_map_y.create(m_height, m_width, CV_32FC1);
}

void CameraModel::setDiffCoeffs(float* diffCoeffs)
{
    memcpy(m_diffCoeffs, diffCoeffs, DIFF_COEFF_SZ*sizeof(float));
    genDiffMaps();
}

void CameraModel::getDiffCoeffs(float* diffCoeffs)
{
    memcpy(diffCoeffs, m_diffCoeffs, DIFF_COEFF_SZ*sizeof(float));
}   

void CameraModel::getDiffMap(Mat& mapx, Mat& mapy)
{
    m_map_x.copyTo(mapx);
    m_map_y.copyTo(mapy);
}

CameraModel::operator std::string() const
{
    std::stringstream classStr;
    classStr << "Camera:\n";
    classStr << "\tWidth: " << m_width << "\n";
    classStr << "\tHeight: " << m_height << "\n";
    classStr << "\tFocal Length: " << m_focal_len << "\n";
    return classStr.str();
}

void CameraModel::genDiffMaps()
{
    // Map the 1D radial function defined by the distortion coeffs (no tangential component)
        // Key: clip_to_uint(distorted radius * DIVISIONS)
        // Val: pair<float,float>(radius, distorted radius)
            // Note: all radii are in normalized coordinates 
            //          (e.g. furthest distance is 1/sqrt(2))
    const unsigned int DIVISIONS = 10000;
    const unsigned int SUB_DIVISION = 1000;
    float deltaR = 1.0f / (float)(DIVISIONS);
    float maxRn = sqrt(0.5f);
    map<unsigned int, pair<float,float> > radialFuncMap;
    for (float rn = 0.0f; rn <= (maxRn + deltaR); rn += deltaR)
    {    
        float r2 = rn*rn;
        float distFactor = 1 + m_diffCoeffs[0] * r2 
                             + m_diffCoeffs[1] * r2 * r2 
                             + m_diffCoeffs[4] * r2 * r2 *r2;
        float rnDist = rn * distFactor;
    
        unsigned int rKey = (unsigned int)(rnDist*DIVISIONS);
        radialFuncMap[rKey] = make_pair(rn,rnDist);
    }
    
    // Generate the map
    m_map_x.create(m_height, m_width, CV_32FC1);
    m_map_y.create(m_height, m_width, CV_32FC1);
    float cx = (float)m_width / 2.0f;
    float cy = (float)m_height / 2.0f;
    for (size_t row = 0; row < m_height; ++row)
    {
        float ynd = (row - cy) / m_height;
        for (size_t col = 0; col < m_width; ++col)
        {
            float xnd = (col - cx) / m_width;
            float rnDist = sqrt(xnd*xnd + ynd*ynd);
            float theta = atan2(ynd,xnd);
            unsigned int key = (unsigned int)(rnDist*DIVISIONS);
            float rn = 0.0f;
            // Get the initial solution from the map of radial values
            if (radialFuncMap.count(key) == 0)
            {
                // Find lower bound key
                for (unsigned int lowKey = (key - 1); 
                    lowKey > (key - SUB_DIVISION); 
                    --lowKey)
                {
                    if (radialFuncMap.count(lowKey) > 0)
                    {
                        rn = radialFuncMap[lowKey].first;
                        break;
                    }
                }
            }
            else
            {
                rn = radialFuncMap[key].first;
            }
            
            // Refine solution
            for (size_t scale = 1; scale < 3; ++scale)
            {
                // Calculate 1/DIVISIONS * 10^-scale
                float sf = deltaR;
                for (size_t cnt = 0; cnt < scale; ++cnt)
                {
                    sf = sf / 10.0f;
                }
                
                // Find at what step value we get the closest answer
                int bestStep = 0;
                float eps = FLT_MAX;
                for (int step = -9; step < 10; ++step)
                {
                    float r2 = rn + (step*sf);
                    r2 = r2*r2;
                    float distFactor = 1 + m_diffCoeffs[0] * r2 
                                         + m_diffCoeffs[1] * r2 * r2 
                                         + m_diffCoeffs[4] * r2 * r2 *r2;
                    float rnDist2 = rn*distFactor;
                    float delta = abs(rnDist - rnDist2);
                    if (delta < eps)
                    {
                        bestStep = step;
                        eps = delta; 
                    }
                }
                
                // Update our answer and move to 
                    // the next smaller scale for further refinement
                rn = rn + (bestStep*sf);
            }
            
            float xn = rn*cos(theta);
            float yn = rn*sin(theta);
            m_map_x.at<float>(row,col) = xn*m_width + cx;
            m_map_y.at<float>(row,col) = yn*m_height + cy;
        }
    }
}

}
