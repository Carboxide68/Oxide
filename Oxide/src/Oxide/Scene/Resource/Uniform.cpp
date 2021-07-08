#include "Uniform.h"
#include "Oxide/Renderer/Shader.h"

namespace Oxide {

    class Uniform<int> : private UniformShell {
    private:
        void m_TypeMe() {
            m_Type = 0;
        }
    };

    class Uniform<float> : private UniformShell {
    private:
        void m_TypeMe() {
            m_Type = 1;
        }
    };

    class Uniform<glm::vec2> : private UniformShell {
    private:
        void m_TypeMe() {
            m_Type = 2;
        }
    };

    class Uniform<glm::vec3> : private UniformShell {
    private:
        void m_TypeMe() {
            m_Type = 3;
        }
    };

    class Uniform<glm::vec4> : private UniformShell {
    private:
        void m_TypeMe() {
            m_Type = 4;
        }
    };

    class Uniform<glm::mat4> : private UniformShell {
    private:
        void m_TypeMe() {
            m_Type = 5;
        }
    };

    class Uniform<Texture2D> : private UniformShell {
    private:
        void m_TypeMe() {
            m_Type = 6;
        }
    };

    class Uniform<TextureCubeMap> : private UniformShell {
    private:
        void m_TypeMe() {
            m_Type = 7;
        }
    };

    UniformShell::~UniformShell() {
        switch(m_Type) {
            case 0:
                ((Uniform<int>*)this)->~Uniform();
            case 1:
                ((Uniform<float>*)this)->~Uniform();
            case 2:
                ((Uniform<glm::vec2>*)this)->~Uniform();
            case 3:
                ((Uniform<glm::vec3>*)this)->~Uniform();
            case 4:
                ((Uniform<glm::vec4>*)this)->~Uniform();
            case 5:
                ((Uniform<glm::mat4>*)this)->~Uniform();
            case 6:
                ((Uniform<Ref<Texture2D>>*)this)->~Uniform();
            case 7:
                ((Uniform<Ref<TextureCubeMap>>*)this)->~Uniform();
            default:
                CO_ASSERT(true, "Type was not found in UniformShell! Possible memory leak as a cause!\n");
                return;
        }
    }

    void UniformShell::SetUniform(const Shader* targetShader) const {
        switch(m_Type) {
            case 0:
                targetShader->SetUniform((Uniform<int>*)this);
            case 1:
                targetShader->SetUniform((Uniform<float>*)this);
            case 2:
                targetShader->SetUniform((Uniform<glm::vec2>*)this);
            case 3:
                targetShader->SetUniform((Uniform<glm::vec3>*)this);
            case 4:
                targetShader->SetUniform((Uniform<glm::vec4>*)this);
            case 5:
                targetShader->SetUniform((Uniform<glm::mat4>*)this);
            case 6:
                targetShader->SetUniform((Uniform<Ref<Texture2D>>*)this);
            case 7:
                targetShader->SetUniform((Uniform<Ref<TextureCubeMap>>*)this);
            default:
                CO_ASSERT(true, "Type was not found in UniformShell! Possible memory leak as a cause!\n");
                return;
        }
    }

    template <class T>
    const T Uniform<T>::GetValue() const {
        switch (m_ValueType) {
        case 0:
            return fnVal();

        case 1:
            return *m_PtrVal;

        case 2:
            return m_RefVal;
        
        case 3:
            return m_ConstVal;
        }
    }
}