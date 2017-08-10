#include "config/CapturePositionConfig.hpp"

#include <iostream>

namespace epilog
{

GeomTransform::GeomTransform(void) :   m_type(GeomTransformType::TRANSLATION),
                        m_angle(0.0f)
{
    m_axis[0] = 0.0f;
    m_axis[1] = 0.0f;
    m_axis[2] = 0.0f;
}

GeomTransform::operator std::string() const
{
    std::stringstream classStr;
    switch (m_type)
    {
        case GeomTransformType::ROTATION:
            classStr << "Rotation:\n";
            classStr << "\tAxis: [" 
                        << m_axis[0] << ", "
                        << m_axis[1] << ", "
                        << m_axis[2] << "]\n";
            classStr << "\tAngle: " << m_angle << "\n";
            break;
            
        case GeomTransformType::TRANSLATION:
            classStr << "Translation:\n";
            classStr << "\tAxis: [" 
                        << m_axis[0] << ", "
                        << m_axis[1] << ", "
                        << m_axis[2] << "]\n";
            break;
            
        case GeomTransformType::INVALID:
            classStr << "Invalid\n";
            break;
    }
    return classStr.str();
}

GeomTransformType GeomTransform::mapStringToTransformType(std::string transformStr)
{
    if (transformStr == "R")
        return GeomTransformType::ROTATION;
    else if (transformStr == "T")
        return GeomTransformType::TRANSLATION;
        
    return GeomTransformType::INVALID;
}

} /// namespace epilog
