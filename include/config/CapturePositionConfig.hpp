#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include <map>
#include <string>
#include <sstream>

namespace epilog
{

enum class GeomTransformType {ROTATION, TRANSLATION, INVALID};

class GeomTransform
{
public:
    GeomTransform(void);

    operator std::string() const;
    
    static GeomTransformType mapStringToTransformType(std::string transformStr);

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
    
    glm::mat4 composite(void);

    operator std::string() const;
    
    int m_targetID;
    std::map<int, GeomTransform> m_transforms;

private:
};

} /// namespace epilog
