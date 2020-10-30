#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace Oxide {

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& filePath) : OpenGLShader(filePath) {

        m_Name = name;

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
        glUniform1i(GetUniformLocation(name), value);
    }
    void OpenGLShader::SetUniform(const std::string& name, const int& value) {
        glUniform1i(GetUniformLocation(name), value);
    }
    void OpenGLShader::SetUniform(const std::string& name, const float& value) {
        glUniform1f(GetUniformLocation(name), value);
    }
    void OpenGLShader::SetUniform(const std::string& name, const glm::vec2& value) {
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }
    void OpenGLShader::SetUniform(const std::string& name, const glm::vec3& value) {
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }
    void OpenGLShader::SetUniform(const std::string& name, const glm::vec4& value) {
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }
    void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& value) {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    int32_t OpenGLShader::GetUniformLocation(const std::string& name) {
        return glGetUniformLocation(m_ProgramID, name.c_str());
    }

    void OpenGLShader::ReadShaders(const std::string& filePath) {

    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& filePath) {

    }
    
    void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& sources) {

    }

}