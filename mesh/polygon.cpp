#include "polygon.h"

Polygon::Polygon(ModelID id, std::vector<glm::vec3> &pos, std::vector<glm::vec2> &texCoords) : Polygon(id, pos, texCoords, {}){}

Polygon::Polygon(ModelID id, std::vector<glm::vec3> &pos, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> normals) {

    ID = id;
    const size_t posSize = pos.size();

    m_FallBackTexCoord = glm::vec2(0);
    m_Normals = {glm::vec3(0)};

    m_Pos.resize(posSize);
    for (int i = 0; i < (const int)posSize; i++) {
        m_Pos[i] = &pos[i];
    }
    size_t texSize = texCoords.size();
    m_TexCoords.resize(texSize);
    for (int i = 0; i < (const int)texSize; i++) {
        m_TexCoords[i] = &texCoords[i];
    }

    if (texSize == 0) {
        m_TexCoords.push_back(&m_FallBackTexCoord);
    }

    else if (posSize > normals.size()) {
        if (normals.size() == 1) { 
            m_Normals.resize(posSize);
            for (int i = 0; i < (const int)posSize; i++) {
                m_Normals[i] = normals[0];
            }
        }
        else {generateNormal(true);}
    }
    else {m_Normals = normals;}
}

Polygon::Polygon(ModelID id, std::vector<glm::vec3*> &pos, std::vector<glm::vec2*> &texCoords) : Polygon(id, pos, texCoords, {}){}
Polygon::Polygon(ModelID id, std::vector<glm::vec3*> &pos, std::vector<glm::vec2*> &texCoords, std::vector<glm::vec3> normals) {
    
    ID = id;
    const size_t posSize = pos.size();

    m_FallBackTexCoord = glm::vec2(0);
    m_Normals = {glm::vec3(0)};

    m_Pos.resize(posSize);
    for (int i = 0; i < (const int)posSize; i++) {
        m_Pos[i] = pos[i];
    }
    size_t texSize = texCoords.size();
    m_TexCoords.resize(texSize);
    for (int i = 0; i < (const int)texSize; i++) {
        m_TexCoords[i] = texCoords[i];
    }

    if (texSize == 0) {
        m_TexCoords.push_back(&m_FallBackTexCoord);
    }

    if (posSize > normals.size()) {
        if (normals.size() == 1) { 
            m_Normals.resize(posSize);
            for (int i = 0; i < (const int)posSize; i++) {
                m_Normals[i] = normals[0];
            }
        }
        else {generateNormal(true);}    }
    else {m_Normals = normals;}}

void Polygon::generateNormal(bool force) {
    if (!CheckForErrors()) {
        return;
    }
    if (force || m_Normals[0].length() < 0.9f) {
        const size_t posSize = m_Pos.size();
        m_Normals.resize(posSize);
        for (int i = 0; i < (const int)posSize; i++) {
            m_Normals[i] = glm::normalize(glm::cross(*m_Pos[1] - *m_Pos[0], *m_Pos[2] - *m_Pos[0]));
        }
    }
}

bool Polygon::CheckForErrors() const {

    if (m_Pos.size() < 3) {
        printf("Polygon is not complete! Only %lu corners!\n", m_Pos.size());
        return false;
    }

    if (m_TexCoords.size() != 1 && m_TexCoords.size() != m_Pos.size()) {
        printf("Texture Coordinates don't match specifications!\n");
        return false;
    }
    return true;
}

std::vector<TextureVertex> Polygon::getTextureVertices() const {

    const size_t positionsSize = m_Pos.size();
    std::vector<TextureVertex> textureVertices;
    textureVertices.reserve(positionsSize);

    uint usingTex = (m_TexCoords.size() == positionsSize) ? 1 : 0;
    for (int i = 0; i < (int)positionsSize; i++) {
        textureVertices.push_back({*m_Pos[i], m_Normals[i], *m_TexCoords[i * usingTex]});
    }
    return textureVertices;
}

std::vector<StandardVertex> Polygon::getStandardVertices() const {
    const size_t positionsSize = m_Pos.size();
    std::vector<StandardVertex> standardVertices;
    standardVertices.reserve(positionsSize);

    for (int i = 0; i < (int)positionsSize; i++) {
        standardVertices.push_back({*m_Pos[i], m_Normals[i]});
    }
    return standardVertices;
}

std::vector<uint> Polygon::assembleIndices() {

    std::vector<uint> indices;

    if (!CheckForErrors()) {
        return {};
    }

    for (int i = 2; i < (int)m_Pos.size(); i++) {
        indices.push_back(0);
        indices.push_back(i - 1);
        indices.push_back(i);
    }
    return indices;
}

std::vector<Triangle> Polygon::assembleTriangleMesh() {

    std::vector<Triangle> triangles;

    if (!CheckForErrors()) {
        return {};
    }

    float usingTex = (m_TexCoords.size() < 1) ? 0.0f : 1.0f;
    for (int i = 2; i < (int)m_Pos.size(); i++) {
        triangles.push_back({
            *m_Pos[0],
            *m_TexCoords[0 * usingTex],
            m_Normals[0],
            *m_Pos[i - 1],
            *m_TexCoords[(i - 1) * usingTex],
            m_Normals[0],
            *m_Pos[i],
            *m_TexCoords[i * usingTex],
            m_Normals[0]
        });
    }
    return triangles;
}