#include "Platform/OpenGL/OpenGLShader.h"

#include "Oxide/Core/Log.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace Oxide {

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& filePath) {

        m_Name = name;
        ReadShaders(filePath);

    }

	OpenGLShader::OpenGLShader(const std::string& filePath) {

        ReadShaders(filePath);

    }

    OpenGLShader::~OpenGLShader() {

        glDeleteProgram(m_ProgramID);

    }

    const std::string& OpenGLShader::GetName() const {return m_Name;}

    void OpenGLShader::Bind() {

        glUseProgram(m_ProgramID);

    }
    
    void OpenGLShader::Unbind() {

        glUseProgram(0);

    }

    void OpenGLShader::SetUniform(const std::string& name, const bool& value) {
        Bind();
        glUniform1i(GetUniformLocation(name), value);
    }
    void OpenGLShader::SetUniform(const std::string& name, const int& value) {
        Bind();
        glUniform1i(GetUniformLocation(name), value);
    }
    void OpenGLShader::SetUniform(const std::string& name, const float& value) {
        Bind();
        glUniform1f(GetUniformLocation(name), value);
    }
    void OpenGLShader::SetUniform(const std::string& name, const glm::vec2& value) {
        Bind();
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }
    void OpenGLShader::SetUniform(const std::string& name, const glm::vec3& value) {
        Bind();
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }
    void OpenGLShader::SetUniform(const std::string& name, const glm::vec4& value) {
        Bind();
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }
    void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& value) {
        Bind();
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    int32_t OpenGLShader::GetUniformLocation(const std::string& name) {
        return glGetUniformLocation(m_ProgramID, name.c_str());
    }

    void OpenGLShader::ReadShaders(const std::string& filePath) {

        std::ifstream file(filePath);

        std::string fileContents;

        if (file) {
            std::stringstream ss;
            ss << file.rdbuf();
            file.close();
            fileContents = ss.str();
        } else {
            CO_CORE_INFO("Couldn't fint load file: %s", filePath.c_str());
            return;
        }

        std::unordered_map<GLenum, std::string> splitFile;
        PreProcess(fileContents, splitFile);
        Compile(splitFile);

    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& fileContent, std::unordered_map<GLenum, std::string>& splitFile) {
        uint pos = 0;
        GLenum curShaderType = GL_FALSE;
        while (pos < fileContent.size()) {
            if (fileContent[pos] == '@') {
                std::string tempShaderType;
                pos++;
                for (; pos < fileContent.size() && fileContent[pos] != '\n' && fileContent[pos] != ' '; pos++) {
                    tempShaderType.push_back(fileContent[pos]);
                }
                pos++;
                if (tempShaderType == "vertex") curShaderType = GL_VERTEX_SHADER;
                else if (tempShaderType == "geometry")  curShaderType = GL_GEOMETRY_SHADER;
                else if (tempShaderType == "fragment")  curShaderType = GL_FRAGMENT_SHADER;
                else if (tempShaderType == "END") break;
                else CO_CORE_WARN("This shadertype is not supported!\n"); 
                continue;
            }
            if (fileContent[pos] == '/' && fileContent[pos+1] == '/') continue;
            for (;pos < fileContent.size() && fileContent[pos] != '\n'; pos++) {
                splitFile[curShaderType].push_back(fileContent[pos]);
            }
            splitFile[curShaderType].push_back('\n');
            pos++;
        }
        return splitFile;
    }
    
    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& splitFile) { //TODO: Fix more shader types

        bool usingGeom = splitFile.find(GL_GEOMETRY_SHADER) != splitFile.end();

        if (splitFile.find(GL_VERTEX_SHADER) == splitFile.end()) {
            CO_CORE_ERROR("You must have a vertex shader in your program!");
            return;
        }
        GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vShaderSourceStr = splitFile.at(GL_VERTEX_SHADER).c_str();
        glShaderSource(vShader, 1, &vShaderSourceStr, NULL);
        if (splitFile.find(GL_FRAGMENT_SHADER) == splitFile.end()) {
            CO_CORE_ERROR("You must have a fragment shader in your program!");
            return;
        }
        GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fShaderSourceStr = splitFile.at(GL_FRAGMENT_SHADER).c_str();
        glShaderSource(fShader, 1, &fShaderSourceStr, NULL);

        GLuint gShader;
        if (usingGeom) {
            gShader = glCreateShader(GL_GEOMETRY_SHADER);
            const char *gShaderSourceStr = splitFile.at(GL_GEOMETRY_SHADER).c_str();
            glShaderSource(gShader, 1, &gShaderSourceStr, NULL);
        }

        glCompileShader(vShader);
        GLint vertexCompiled;
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertexCompiled);
        if (vertexCompiled != GL_TRUE) {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(vShader, 1024, &log_length, message);
            CO_CORE_ERROR("Vertex error:\n", message);
        }

        glCompileShader(fShader);
        GLint fragmentCompiled;
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragmentCompiled);
        if (fragmentCompiled != GL_TRUE) {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(fShader, 1024, &log_length, message);
            CO_CORE_ERROR("Fragment error:\n", message);
        }

        m_ProgramID = glCreateProgram();
        glAttachShader(m_ProgramID, vShader);
        glAttachShader(m_ProgramID, fShader);

        if (usingGeom) {
            glCompileShader(gShader);
            GLint geometryCompiled;
            glGetShaderiv(gShader, GL_COMPILE_STATUS, &geometryCompiled);
            if (geometryCompiled != GL_TRUE) {
                GLsizei log_length = 0;
                GLchar message[1024];
                glGetShaderInfoLog(gShader, 1024, &log_length, message);
                CO_CORE_ERROR("Geometry error:\n", message);
            }
            glAttachShader(m_ProgramID, gShader);
        }

        glLinkProgram(m_ProgramID);

        GLint programLinked;
        glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &programLinked);
        if (programLinked != GL_TRUE) {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetProgramInfoLog(m_ProgramID, 1024, &log_length, message);
            CO_CORE_ERROR("Program linkingrror:\n", message);
        }
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        if (usingGeom)
            glDeleteShader(gShader);

        return;
    }
}