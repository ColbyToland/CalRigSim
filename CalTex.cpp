#include "CalTex.hpp"

using namespace cv;

namespace epilog
{

void CalTex::genChArUco(cv::aruco::PREDEFINED_DICTIONARY_NAME dict,
                size_t rows, size_t cols)
{
    m_dictionary = aruco::getPredefinedDictionary(dict); 
    m_pboard = aruco::CharucoBoard::create(cols, rows, 0.04, 0.02, &m_dictionary); 
}

void CalTex::genTexture(size_t width, size_t height)
{
    m_pboard->draw( Size(width, height), m_image, 10, 1 );
}

} // namespace epilog
