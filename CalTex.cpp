#include "CalTex.hpp"
#include "CalData.hpp"

using namespace cv;

namespace epilog
{

void CalTex::genChArUco()
{
    CalData* config = CalData::getInstance();

    Ptr<aruco::Dictionary> dict = aruco::getPredefinedDictionary(config->m_markerDict);
    Ptr<aruco::CharucoBoard> pboard = aruco::CharucoBoard::create(
                                                            config->m_chessCols, 
                                                            config->m_chessRows, 
                                                            config->m_chessSqSz, 
                                                            config->m_arucoSz,
                                                            dict);
    pboard->draw( Size(config->m_pxWidthTarget, config->m_pxHeightTarget), 
                    m_image, 0, 1 );
}

void CalTex::readImage(std::string filename)
{
    m_image = imread(filename);
}

} // namespace epilog
