#include "config/CapturePositionConfig.hpp"

#include <iostream>

namespace epilog
{

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
