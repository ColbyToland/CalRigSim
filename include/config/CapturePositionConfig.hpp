#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include <map>
#include <string>
#include <sstream>

#include "GeomTransform.hpp"

namespace epilog
{

class CapturePositionConfig
{
public:
    CapturePositionConfig(void) : m_targetID(0)
    { }

    operator std::string() const;
    
    int m_targetID;
    std::map<int, GeomTransform> m_transforms;

private:
};

} /// namespace epilog
