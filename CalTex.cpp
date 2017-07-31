#include "CalTex.hpp"

using namespace cv;

namespace epilog
{

CalTex::CalTex()
{
    // Create and save board image
    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_250); 
    m_pboard = aruco::CharucoBoard::create(5, 7, 0.04, 0.02, &dictionary); 
    m_pboard->draw( Size(600, 500), m_image, 10, 1 );
}

} // namespace epilog
