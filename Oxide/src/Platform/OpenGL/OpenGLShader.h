#pragma once

#include <string>

#include "Oxide/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Oxide {

	class OpenGLShader : public Shader {

	public:


		OpenGLShader(const std::string& name, const std::string& filePath);

		OpenGLShader(const std::string& filePath);

		virtual ~OpenGLShader();

        virtual const std::string& GetName() const;

		virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SetUniform(const std::string& name, const bool& value) override;
        virtual void SetUniform(const std::string& name, const int& value) override;
        virtual void SetUniform(const std::string& name, const float& value) override;
        virtual void SetUniform(const std::string& name, const glm::vec2& value) override;
        virtual void SetUniform(const std::string& name, const glm::vec3& value) override;
        virtual void SetUniform(const std::string& name, const glm::vec4& value) override;
        virtual void SetUniform(const std::string& name, const glm::mat4& value) override;

	private:

		void ReadShaders(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& fileContent, std::unordered_map<GLenum, std::string>& splitFile);
		void Compile(const std::unordered_map<GLenum, std::string>& sources);

		int32_t GetUniformLocation(const std::string& name);

	private:
		std::string m_Name;
		std::unordered_map<std::string, int32_t> m_UniformLocations;
		uint32_t m_ProgramID;

	};

}