#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glew.h>

namespace Oxide {

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& filePath) : OpenGLShader(filePath) {
        m_Name = name;
    }

	OpenGLShader::OpenGLShader(const std::string& filePath) {

    }

    void OpenGLShader::Bind() {
        
    }
    
    void OpenGLShader::Unbind() {

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

}