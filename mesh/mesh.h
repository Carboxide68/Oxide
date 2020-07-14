#pragma once

#include "../common/common.h"
#include "modelCommon.h"
#include "polygon.h"
#include <utility>

class Model;
class Group;

class Mesh {
private:
    using IndexArray = std::vector<std::vector<uint>>;
public:

    struct Groupspan {
        std::shared_ptr<std::string> name;
        uint begin;
        uint end;
    };

    Mesh(ModelID ID);

    void loadData(std::vector<std::array<float, 3>> positions, std::vector<std::array<float, 2>> texCoords);

    std::vector<Triangle> assembleToTriangleMesh();
    std::vector<StandardVertex> getStandardVertices() const;
    std::vector<TextureVertex> getTextureVertices() const;

    //Bytelocation of polygon in m_Polygons;
    inline size_t getPolygonLocation(const ModelID id) const {return m_PolygonMap[GetPolygonIndexFromID(id)];}
    
    void generatePolygons();

    std::vector<Polygon*> createPolygons(IndexArray positionIndices);
    std::vector<Polygon*> createPolygons(IndexArray positionIndices, IndexArray texIndices);
    std::vector<Polygon*> createPolygons(IndexArray positionIndices, std::vector<std::vector<glm::vec3>> normals);
    std::vector<Polygon*> createPolygons(IndexArray positionIndices, IndexArray texIndices, std::vector<std::vector<glm::vec3>> normals);

    Polygon* createPolygon(std::vector<uint> positionIndex);
    Polygon* createPolygon(std::vector<uint> positionIndex, std::vector<uint> texIndex);
    Polygon* createPolygon(std::vector<uint> positionIndex, std::vector<glm::vec3> normal);
    Polygon* createPolygon(std::vector<uint> positionIndex, std::vector<uint> texIndex, std::vector<glm::vec3> normal);

    void addGroup(std::string groupName, uint start, uint end);
    void addGroup(std::string groupName);

    inline std::vector<Groupspan>& getGroupMap() {return m_GroupMap;}
    std::vector<Groupspan> getGroup(std::string name);

    void update();
    void updatePolygonMap();
    void updatePolygonNormals(bool force = false);

    std::string Name;
    
    std::vector<Polygon> Polygons;
    std::vector<glm::vec3> Vertices; //Vertex data is stored on a per-mesh basis
    std::vector<glm::vec2> TexCoords;

    bool Enabled;

    ModelID ID;
    friend class Model;

private:


    //Function is used to update the internal buffermap of the mesh object. This should be done whenever you change something
    //in the object and want to load the changes into the GPU buffer;  If the GPU buffer and the MeshMap are not synched, this may 
    //lead to problems.
    void UpdateGroupMap();

    std::vector<uint> m_PolygonMap; //Index N is index (N - 1) + PolygonTriangleCount
    inline uint GetPolygonIndexFromID(ModelID id) const {return id.polygon;}

    inline ModelID GenerateID() {auto temp = ID; ID.polygon++; return temp;}

    std::vector<Groupspan> m_GroupMap;

};