#include "config/CapturePositionConfig.hpp"

#include <iostream>

namespace epilog
{

std::string GeomTransform::INVALID_TAG = "Invalid";
std::string GeomTransform::ROTATION_TAG = "R";
std::string GeomTransform::TRANSLATION_TAG = "T";
std::string GeomTransform::ROTATION_CYCLE_TAG = "R_cycle";

GeomTransform::GeomTransform(void) :    m_type(GeomTransformType::TRANSLATION),
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
            
        case GeomTransformType::ROTATION_CYCLE:
            classStr << "Rotation Cycle:\n";
            classStr << "\tAxis: [" 
                        << m_axis[0] << ", "
                        << m_axis[1] << ", "
                        << m_axis[2] << "]\n";
            classStr << "\tDelta Angle: " << m_angle << "\n";
            break;
            
        case GeomTransformType::INVALID:
            classStr << "Invalid\n";
            break;
    }
    return classStr.str();
}

GeomTransformType GeomTransform::mapStringToTransformType(std::string transformStr)
{
    if (transformStr == ROTATION_TAG)
        return GeomTransformType::ROTATION;
    else if (transformStr == TRANSLATION_TAG)
        return GeomTransformType::TRANSLATION;
    else if (transformStr == ROTATION_CYCLE_TAG)
        return GeomTransformType::ROTATION_CYCLE;
        
    return GeomTransformType::INVALID;
}

std::string GeomTransform::mapTransformTypeToString(GeomTransformType transform)
{
    std::string typeStr = INVALID_TAG;

    if (transform == GeomTransformType::ROTATION)
    {
        typeStr = ROTATION_TAG;
    }
    else if (transform == GeomTransformType::TRANSLATION)
    {
        typeStr = TRANSLATION_TAG;
    }
    else if (transform == GeomTransformType::ROTATION_CYCLE)
    {
        typeStr = ROTATION_CYCLE_TAG;
    }
    
    return typeStr;
}

} /// namespace epilog
