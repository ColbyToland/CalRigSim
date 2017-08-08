#pragma once

#include <map>
#include <string>
#include <sstream>

namespace epilog
{

enum class GeomTransformType {ROTATION, TRANSLATION, INVALID};

class GeomTransform
{
public:
    GeomTransform(void) :   m_type(GeomTransformType::TRANSLATION),
                            m_angle(0.0f)
    {
        m_axis[0] = 0.0f;
        m_axis[1] = 0.0f;
        m_axis[2] = 0.0f;
    }

    operator std::string() const
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
    
    static GeomTransformType mapStringToTransformType(std::string transformStr)
    {
        if (transformStr == "R")
            return GeomTransformType::ROTATION;
        else if (transformStr == "T")
            return GeomTransformType::TRANSLATION;
            
        return GeomTransformType::INVALID;
    }

    GeomTransformType m_type;
    float m_axis[3];
    float m_angle;

private:
};

class CapturePositionConfig
{
public:
    CapturePositionConfig(void) : m_targetID(0)
    { }

    operator std::string() const
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
    
    int m_targetID;
    std::map<int, GeomTransform> m_transforms;

private:
};

} /// namespace epilog
