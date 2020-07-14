#include "mesh.h"


Mesh::Mesh(ModelID ID) {
    this->ID = ID;
    Enabled = true;
    char temp[32];
    sprintf(temp, "Mesh %d", (int)ID.mesh);
    Name = temp;
    m_GroupMap = {};
    m_PolygonMap = {};
    Polygons = {};
    Vertices = {};
    TexCoords = {};
}

void Mesh::loadData(std::vector<std::array<float, 3>> positions, std::vector<std::array<float, 2>> texCoords) {
    const size_t posSize = positions.size();
    for (int i = 0; i < (int)posSize; i++) {
        Vertices.push_back({positions[i][0], positions[i][1], positions[i][1]});
    }
    const size_t texSize = texCoords.size();
    for (int i = 0; i < (int)texSize; i++) {
        TexCoords.push_back({texCoords[i][0], texCoords[i][1]});
    }
}

std::vector<Polygon*> Mesh::createPolygons(IndexArray positionIndices) {return this->createPolygons(positionIndices, {}, {});}
std::vector<Polygon*> Mesh::createPolygons(IndexArray positionIndices, IndexArray texIndices) {return this->createPolygons(positionIndices, texIndices, {});}
std::vector<Polygon*> Mesh::createPolygons(IndexArray positionIndices, std::vector<std::vector<glm::vec3>> normals) {return this->createPolygons(positionIndices, {}, normals);}
std::vector<Polygon*> Mesh::createPolygons(IndexArray positionIndices, IndexArray texIndices, std::vector<std::vector<glm::vec3>> normals) {

    std::vector<Polygon*> polygonRefs;

    bool usingTex = 1;
    bool usingNorm = 1;

    auto startPos = Polygons.end() - Polygons.begin();
    const size_t posIndSize =  positionIndices.size();
    Polygons.reserve(Polygons.size() + posIndSize);
    if (posIndSize > texIndices.size()) {
        usingTex = 0;
        texIndices = {{}};
    }
    if (posIndSize > normals.size()) {
        usingNorm = 0;
        normals = {{}};
    }
    for (int i = 0; i < posIndSize; i++) {
        std::vector<glm::vec3> normal = normals[i * usingNorm];
        std::vector<glm::vec3*> positions(positionIndices[i].size());
        for (int k = 0; k < (const int)positionIndices[i].size(); k++) {
            positions[k] = &Vertices[positionIndices[i][k]];
        }
        std::vector<glm::vec2*> texcoords(texIndices[i * usingTex].size());
        for (int k = 0; k < (const int)texIndices[i * usingTex].size(); k++) {
            texcoords[k] = &TexCoords[texIndices[i][k]];
        }
        Polygons.push_back(Polygon(GenerateID(), positions, texcoords, normal));
    }
    polygonRefs.reserve((Polygons.end() - Polygons.begin() - startPos));
    for (auto it = Polygons.begin() + startPos; it != Polygons.end(); it++) {
        polygonRefs.push_back(&*it);
    }
    return polygonRefs;
}


Polygon* Mesh::createPolygon(std::vector<uint> positionIndex) {return this->createPolygon(positionIndex, {}, std::vector<glm::vec3>());}
Polygon* Mesh::createPolygon(std::vector<uint> positionIndex, std::vector<uint> texIndex) {return this->createPolygon(positionIndex, texIndex, std::vector<glm::vec3>());}
Polygon* Mesh::createPolygon(std::vector<uint> positionIndex, std::vector<glm::vec3> normal) {return this->createPolygon(positionIndex, {}, normal);}
Polygon* Mesh::createPolygon(std::vector<uint> positionIndex, std::vector<uint> texIndex, std::vector<glm::vec3> normal) {
    std::vector<glm::vec3*> positions(positionIndex.size());
    for (int i = 0; i < positionIndex.size(); i++) {
        positions[i] = &Vertices[positionIndex[i]];
    }
    std::vector<glm::vec2*> texcoords(positionIndex.size());
    for (int i = 0; i < texIndex.size(); i++) {
        texcoords[i] = &TexCoords[texIndex[i]];
    }
    Polygons.push_back(Polygon(GenerateID(), positions, texcoords, normal));
    return &(Polygons.back());
}

void Mesh::addGroup(std::string groupName, uint begin, uint end) {

    std::vector<Groupspan> tmp = {{std::shared_ptr<std::string>(new std::string(groupName)), begin, end}};

    for (int i = 0; i < (const int)m_GroupMap.size(); i++) {
        if (m_GroupMap[i].begin > begin) {
            m_GroupMap.insert(m_GroupMap.begin() + i - 1, tmp.begin(), tmp.end());
            return;
        }
    }
    m_GroupMap.insert(m_GroupMap.end(), tmp.begin(), tmp.end());
}

void Mesh::addGroup(std::string groupName) {
    m_GroupMap = {{std::shared_ptr<std::string>(new std::string(groupName)), 0, (uint)Polygons.size()}};
}

std::vector<Mesh::Groupspan> Mesh::getGroup(std::string name) {
    std::vector<Groupspan> group;
    if (name == STANDARD_NAME) {
        const size_t temp = m_GroupMap.size();
        if (temp == 0) {
            return {{std::make_shared<std::string>(STANDARD_NAME), 0, (uint)Polygons.size()}};
        }
        for (int i = 0; i < (int)temp; i++) {
            if (i == 0) {
                if (0 != m_GroupMap[i].begin) {
                    group.push_back({std::make_shared<std::string>(STANDARD_NAME), 0, m_GroupMap[0].begin});
                }
            }
            else if (i == (int)(temp - 1)) {
                if ((uint)Polygons.size() != m_GroupMap[i].end) {
                    group.push_back({std::make_shared<std::string>(STANDARD_NAME), m_GroupMap[i].end, (uint)Polygons.size()});
                }
            }
            else if (m_GroupMap[i].end != m_GroupMap[i+1].begin) {
                group.push_back({std::make_shared<std::string>(STANDARD_NAME), m_GroupMap[i].end, m_GroupMap[i+1].begin});
            }
        }
    } else {
        for (int i = 0; i < (const int)m_GroupMap.size(); i++) {
            if (*(m_GroupMap[i].name) == name) {
                group.push_back(m_GroupMap[i]);
            }
        }
    }
    return group;
}

std::vector<StandardVertex> Mesh::getStandardVertices() const {
    std::vector<StandardVertex> vertices;
    for (int i = 0; i < (int)Polygons.size(); i++) {
        std::vector<StandardVertex> tempVertices = Polygons[i].getStandardVertices();
        vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
    }
    return vertices;
}

std::vector<TextureVertex> Mesh::getTextureVertices() const {
    std::vector<TextureVertex> vertices;
    for (int i = 0; i < (int)Polygons.size(); i++) {
        std::vector<TextureVertex> tempVertices = Polygons[i].getTextureVertices();
        vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
    }
    return vertices;
}

void Mesh::updatePolygonMap() {
    const size_t polygonSize = Polygons.size();
    m_PolygonMap.reserve(polygonSize + 1);
    m_PolygonMap.push_back(0); //First index will be zero, second will be after the first is done etc
    uint offset = 0;
    for (int i = 0; i < (int)polygonSize; i++) {
        offset += Polygons[i].m_Pos.size();
        m_PolygonMap.push_back(offset); //Change this to a per-model standard
    }
}

void Mesh::updatePolygonNormals(bool force) {
    const size_t polygonSize = Polygons.size();
    for (int i = 0; i < (int)polygonSize; i++) {
        Polygons[i].generateNormal(force);
    }
}

void Mesh::update() {
    updatePolygonMap();
    updatePolygonNormals();
}