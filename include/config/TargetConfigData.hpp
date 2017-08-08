#pragma once

#include <memory>
#include <string>
#include <sstream>

namespace epilog
{

class TargetConfigData
{
public:
    const size_t VERTEX_SIZE = 5;
    const size_t TRI_SIZE = 3;

    TargetConfigData(void) :    m_texID(0),
                                m_vertexCount(0),
                                m_triCount(0)
    { }

    operator std::string() const
    {
        std::stringstream classStr;
        classStr << "Target:\n";
        classStr << "\tTexture ID: " << m_texID << "\n";
        classStr << "\tVertices:";
        for (size_t ind = 0; ind < m_vertexCount*VERTEX_SIZE; ++ind)
        {
            if (ind % VERTEX_SIZE == 0)
                classStr << "\n\t\t";
            classStr << m_vertexData.get()[ind] << "\t";
        }
        classStr << "\n";
        classStr << "\tTriangle Vertex Indices:";
        for (size_t ind = 0; ind < m_triCount*TRI_SIZE; ++ind)
        {
            if (ind % TRI_SIZE == 0)
                classStr << "\n\t\t";
            classStr << m_indices.get()[ind] << "\t";
        }
        classStr << "\n";
        return classStr.str();
    }

    int m_texID;
    std::shared_ptr<float> m_vertexData;
    size_t m_vertexCount;
    std::shared_ptr<unsigned int> m_indices;
    size_t m_triCount;

private:
};

} /// namespace epilog
