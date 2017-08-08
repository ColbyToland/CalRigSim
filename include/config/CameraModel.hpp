#pragma once

#include <string>
#include <sstream>

class CameraModel
{
public:
    CameraModel() : m_width(1024), 
                    m_height(768), 
                    m_fov(100.0)
    {}

    operator std::string() const
    {
        std::stringstream classStr;
        classStr << "Camera:\n";
        classStr << "\tWidth: " << m_width << "\n";
        classStr << "\tHeight: " << m_height << "\n";
        classStr << "\tfov: " << m_fov << "\n";
        return classStr.str();
    }

    size_t m_width;
    size_t m_height;
    double m_fov;

private:
};
