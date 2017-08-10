#pragma once

#include <string>
#include <sstream>

#include "GeomTransform.hpp"

namespace epilog
{

class CameraModel
{
public:
    CameraModel() : m_width(1024), 
                    m_height(768), 
                    m_focal_len(430)
    {}

    operator std::string() const
    {
        std::stringstream classStr;
        classStr << "Camera:\n";
        classStr << "\tWidth: " << m_width << "\n";
        classStr << "\tHeight: " << m_height << "\n";
        classStr << "\tFocal Length: " << m_focal_len << "\n";
        return classStr.str();
    }

    size_t m_width;
    size_t m_height;
    double m_focal_len;
    std::map<int, GeomTransform> m_transforms;

private:
};

}
