#include "CalTex.hpp"
#include "config/CalData.hpp"

using namespace cv;

namespace epilog
{

CalTex::CalTex(TextureConfigData& texConfig)
{
    if (TextureType::CHARUCO_TEXTURE == texConfig.m_type)
    {
        Ptr<aruco::Dictionary> dict = aruco::getPredefinedDictionary(
                                                        texConfig.m_markerDict);
        Ptr<aruco::CharucoBoard> pboard = aruco::CharucoBoard::create(
                                                        texConfig.m_chessCols, 
                                                        texConfig.m_chessRows, 
                                                        texConfig.m_chessSqSz, 
                                                        texConfig.m_arucoSz,
                                                        dict);
        pboard->draw( Size(texConfig.m_pxWidthTarget, 
                            texConfig.m_pxHeightTarget), 
                     m_image, 0, 1 );
    }
    else if (TextureType::IMAGE_TEXTURE == texConfig.m_type)
    {    
        m_image = imread(texConfig.m_imgFilename);
    }
    else /// TextureType::INVALID
    {
        m_image = cv::Mat(100, 100, CV_8UC3, Scalar(0,255,0));
    }
}

} // namespace epilog
