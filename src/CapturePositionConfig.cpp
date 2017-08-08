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

glm::mat4 CapturePositionConfig::composite(void)
{
    glm::mat4 modelview;
    
    for (auto kv : m_transforms)
    {
        if (GeomTransformType::ROTATION == kv.second.m_type)
        {
            modelview = glm::rotate(modelview, 
                                    glm::radians(kv.second.m_angle),
                                    glm::vec3(  kv.second.m_axis[0],
                                                kv.second.m_axis[1],
                                                kv.second.m_axis[2]));
        }
        else if (GeomTransformType::TRANSLATION == kv.second.m_type)
        {
            modelview = glm::translate( modelview,
                                        glm::vec3(  kv.second.m_axis[0],
                                                    kv.second.m_axis[1],
                                                    kv.second.m_axis[2]) );
        }
        else
        {
            std::cerr << "Invalid transform:" << std::endl;
            std::cerr << std::string(kv.second) << std::endl;
        }
    }
    
    return modelview;
}

CapturePositionConfig::operator std::string() const
{
    std::stringstream classStr;
    classStr << "Capture:\n";
    classStr << "\tTarget ID: " << m_targetID << "\n";
    classStr << "\tTransforms:\n";
    for (auto key : m_transforms)
    {
        classStr << key.first << " - ";
        classStr << std::string(key.second);
        classStr << "\n";
    }
    return classStr.str();
}

} /// namespace epilog
