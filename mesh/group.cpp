#include "group.h"
#include "model.h"
#include "../stb_image.h"


Group::Group(Model *parent) : m_ParentModel(parent) {
    material = EMPTY_MATERIAL;
    char temp[32];
    sprintf(temp, "Group %d", (int)parent->Groups.size());
    m_Name = temp;
}

void Group::setName(std::string name, bool update) {
    std::string tmp;
    if (update) {
        for (int i = 0; i < (int)m_ParentModel->Meshes.size(); i++) {
            std::vector<Mesh::Groupspan>& groupMap = m_ParentModel->Meshes[i].getGroupMap();
            for (int j = 1; j < (int)groupMap.size() - 1; j++) {
                if ((*groupMap[j].name) == m_Name) {
                    (*groupMap[j].name) = name;
                }
            }
        }
    }
    m_Name = name;
}

void Group::bindMaterial(Shader *shader) {
    
    shader->setUniform("material.ambient", material.ambient);
    shader->setUniform("material.diffuse", material.diffuse);
    shader->setUniform("material.specular", material.specular);
    shader->setUniform("material.illum", material.illum);
    shader->setUniform("material.specE", material.specE);
    shader->setUniform("material.opacity", material.opacity);
    shader->setUniform("material.opticalDensity", material.opticalDensity);

    shader->setUniform("material.usingambientTexture", false);
    shader->setUniform("material.usingdiffuseTexture", false);
    shader->setUniform("material.usingspecularTexture", false);
    shader->setUniform("material.usingspecularETexture", false);
    shader->setUniform("material.usingbumpMap", false);

    int c = 0;
    for (int i = 0; i < m_Textures.size(); i++) {
        if (m_Textures[i] != 0) {
            GLCall(glActiveTexture(GL_TEXTURE0 + c + 1));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i]));
            shader->setUniform("material." + material.textures[i].name, c + 1);
            shader->setUniform("material.using" + material.textures[i].name, true);
            c++;
        }
    }
}

void Group::UpdateIndices() {

    size_t polygonLoc;
    m_Indices.clear();

    for (int i = 0; i < (int)m_ParentModel->Meshes.size(); i++) {
        if (!m_ParentModel->Meshes[i].Enabled) continue;
        std::vector<Mesh::Groupspan> group = m_ParentModel->Meshes[i].getGroup(m_Name);
        for (int j = 0; j < (int)group.size(); j++) {
            for (int k = group[j].begin; k < (const int)group[j].end; k++) {
                std::vector<uint> tempIndices = m_ParentModel->Meshes[i].Polygons[k].assembleIndices();
                polygonLoc = m_ParentModel->getPolygonLocation(m_ParentModel->Meshes[i].Polygons[k].ID);
                for (auto index = tempIndices.begin(); index != tempIndices.end(); index++) {
                    m_Indices.push_back(polygonLoc + *(index));
                }
            }
        }
    }
    return;
}

void Group::update() {
    UpdateIndices();
}

void Group::generateTextures() {

    const size_t textureC = material.textures.size();

    stbi_set_flip_vertically_on_load(1);

    m_Textures.resize(textureC);
    for (int i = 0; i < textureC; i++) {
        int x, y, channels;
        unsigned char* data = stbi_load(material.textures[i].path.c_str(), &x, &y, &channels, 0);
        if (!data) {
            printf("Failed to load image %s!\n", material.textures[i].path.c_str());
            stbi_image_free(data);
            m_Textures[i] = 0;
            continue;
        }
        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;

        } else if (channels == 1) {
            internalFormat = GL_R8;
            dataFormat = GL_RED;
        }

        if ((internalFormat == dataFormat)) {
            printf("Format not supported!\n");
            stbi_image_free(data);
            m_Textures[i] = 0;
            continue;
        }
        
        GLCall(glGenTextures(1, &m_Textures[i]));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i]));


        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST))

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, dataFormat, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        stbi_image_free(data);
    }

}