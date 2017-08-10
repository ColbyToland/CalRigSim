#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

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

} /// namespace epilog
