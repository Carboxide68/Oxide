#include "Oxide/Renderer/Renderer.h"

#include "Oxide/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Oxide {

    Ref<Shader> Shader::Create(const std::string& name, const std::string& filePath) {

        switch (Renderer::GetAPI()) {

            case Renderer::API::None: CO_CORE_ASSERT(false, "Renderer::API::None is currently not supported!"); return nullptr;
            case Renderer::API::OpenGL: return CreateRef<OpenGLShader>(name, filePath);

        }

        CO_CORE_ASSERT(false, "This RendererAPI doesn't exist!");
        return nullptr;

    }

    Ref<Shader> Shader::Create(const std::string& filePath) {

        switch (Renderer::GetAPI()) {

            case Renderer::API::None: CO_CORE_ASSERT(false, "Renderer::API::None is currently not supported!"); return nullptr;
            case Renderer::API::OpenGL: return CreateRef<OpenGLShader>(filePath);

        }

        CO_CORE_ASSERT(false, "Rendering API doesn't exist!");
        return nullptr;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath) {

        Ref<Shader> shader = Shader::Create(name, filePath);
        m_ShaderReferences[name] = shader;
        return shader;

    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
        
        CO_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_ShaderReferences[name] = shader;

    }

    void ShaderLibrary::Add(const Ref<Shader>& shader) {
        auto& name = shader->GetName();
        CO_CORE_ASSERT(name == "", "Shader needs to have a name!");
        Add(name, shader);

    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name) {
        CO_CORE_ASSERT(Exists(), "Shader doesn't exist!");
        return m_ShaderReferences[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const {
        return m_ShaderReferences.find(name) != m_ShaderReferences.end();
    }

}