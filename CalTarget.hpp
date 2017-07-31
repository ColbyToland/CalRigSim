#pragma once

#include "CalTex.hpp"

namespace epilog
{

class CalTarget
{
public:
    CalTarget() { }
    CalTarget(CalTex& texture) : m_tex(texture.m_image) { }

    void draw(void) const;

    cv::Mat m_tex;

private:

};

} // namespace epilog
