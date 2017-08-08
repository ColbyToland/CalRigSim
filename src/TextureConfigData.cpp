#include "config/TextureConfigData.hpp"

namespace epilog
{

TextureType TextureConfigData::mapStringToTextureType(std::string typeStr)
{
    if (typeStr == "image")
        return TextureType::IMAGE_TEXTURE;
    else if (typeStr == "charuco")        
        return TextureType::CHARUCO_TEXTURE;
        
    return TextureType::INVALID;
}

cv::aruco::PREDEFINED_DICTIONARY_NAME TextureConfigData::mapStringToArUcoDict(std::string dictStr)
{
    if (dictStr == "DICT_4X4_50")
    {
        return cv::aruco::DICT_4X4_50;
    }
    else if (dictStr == "DICT_4X4_100")
    {
        return cv::aruco::DICT_4X4_100;
    }
    else if (dictStr == "DICT_4X4_250")
    {
        return cv::aruco::DICT_4X4_250;
    }
    else if (dictStr == "DICT_4X4_1000")
    {
        return cv::aruco::DICT_4X4_1000;
    }
    else if (dictStr == "DICT_5X5_50")
    {
        return cv::aruco::DICT_5X5_50;
    }
    else if (dictStr == "DICT_5X5_100")
    {
        return cv::aruco::DICT_5X5_100;
    }
    else if (dictStr == "DICT_5X5_250")
    {
        return cv::aruco::DICT_5X5_250;
    }
    else if (dictStr == "DICT_5X5_1000")
    {
        return cv::aruco::DICT_5X5_1000;
    }
    else if (dictStr == "DICT_6X6_50")
    {
        return cv::aruco::DICT_6X6_50;
    }
    else if (dictStr == "DICT_6X6_100")
    {
        return cv::aruco::DICT_6X6_100;
    }
    else if (dictStr == "DICT_6X6_250")
    {
        return cv::aruco::DICT_6X6_250;
    }
    else if (dictStr == "DICT_6X6_1000")
    {
        return cv::aruco::DICT_6X6_1000;
    }
    else if (dictStr == "DICT_7X7_50")
    {
        return cv::aruco::DICT_7X7_50;
    }
    else if (dictStr == "DICT_7X7_100")
    {
        return cv::aruco::DICT_7X7_100;
    }
    else if (dictStr == "DICT_7X7_250")
    {
        return cv::aruco::DICT_7X7_250;
    }
    else if (dictStr == "DICT_7X7_1000")
    {
        return cv::aruco::DICT_7X7_1000;
    }
    
    return cv::aruco::DICT_ARUCO_ORIGINAL;
}
    
};
