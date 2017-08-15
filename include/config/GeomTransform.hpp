#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include <string>
#include <sstream>

namespace epilog
{

enum class GeomTransformType {ROTATION, TRANSLATION, ROTATION_CYCLE, INVALID};

class GeomTransform
{
public:
    static std::string INVALID_TAG;
    static std::string ROTATION_TAG;
    static std::string TRANSLATION_TAG;
    static std::string ROTATION_CYCLE_TAG;

    GeomTransform(void);

    operator std::string() const;
    
    static GeomTransformType mapStringToTransformType(std::string transformStr);
    static std::string mapTransformTypeToString(GeomTransformType transform);

    GeomTransformType m_type;
    float m_axis[3];
    float m_angle;

private:
};

} /// namespace epilog
