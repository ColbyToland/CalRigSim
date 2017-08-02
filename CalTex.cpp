#include "CalTex.hpp"
#include "CalData.hpp"

using namespace cv;

namespace epilog
{

void CalTex::genChArUco()
{
    CalData* config = CalData::getInstance();

    // The destructor for pboard deletes the passed in dictionary which is stupid.
    // As a work around, I allocate a copy of the dictionary. So stupid...
    // Without this, the code seg faults. Lame.
    aruco::Dictionary dict = aruco::getPredefinedDictionary(config->m_markerDict); 
    aruco::Dictionary* pdict = new aruco::Dictionary(dict);
    Ptr<aruco::CharucoBoard> pboard = aruco::CharucoBoard::create(
                                                            config->m_chessCols, 
                                                            config->m_chessRows, 
                                                            0.04, 
                                                            0.02, 
                                                            pdict);
    pboard->draw( Size(config->m_pxWidthTarget, config->m_pxHeightTarget), 
                    m_image, 0, 1 );
}

void CalTex::readImage(std::string filename)
{
    m_image = imread(filename);
}

} // namespace epilog
