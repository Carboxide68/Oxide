#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Resource/Uniform.h"

#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Oxide  {

    class Shader {

    public:

        virtual ~Shader() = default;

        virtual const std::string& GetName() const = 0;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetUniform(const std::string& name, const bool& value) = 0;
        virtual void SetUniform(const std::string& name, const int& value) = 0;
        virtual void SetUniform(const std::string& name, const float& value) = 0;
        virtual void SetUniform(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetUniform(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetUniform(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetUniform(const std::string& name, const glm::mat4& value) = 0;
        virtual void SetUniform(const Ref<Texture2D> texture) = 0;

        template <class T>
        inline void SetUniform(const Uniform<T>& uniform) const {SetUniform(uniform.GetName(), uniform.GetValue());}
        template <class T>
        inline void SetUniform(const Uniform<T>* uniform) const {SetUniform(uniform->GetName(), uniform->GetValue());}
        
        inline void SetUniform(const UniformShell* uniform) const {uniform->SetUniform(this);}

        static Ref<Shader> Create(const std::string& name, const std::string& filePath);
        static Ref<Shader> Create(const std::string& filePath);

    };

    class ShaderLibrary {
    public:

        void Add(const std::string& name, const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader);

        Ref<Shader> Load(const std::string& name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);

        bool Exists(const std::string& name) const;

    private:

        std::unordered_map<std::string, Ref<Shader>> m_ShaderReferences;

    };

}