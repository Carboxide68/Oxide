#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Texture.h"

namespace Oxide {

    //Type index:
    /**
     * int/bool = 0;
     * float = 1;
     * vec2 = 2;
     * vec3 = 3;
     * vec4 = 4;
     * mat4 = 5;
     * Tex2D = 6;
     * TexCube = 7;
     */

    class Shader;
    class UniformShell {
    public:

        ~UniformShell();
        void SetUniform(const Shader* targetShader) const;

        uint Type;

    };

    template<class T>
    class Uniform : public UniformShell {
    public:
        Uniform<T>::Uniform(const std::string& name, const T val) : m_ValueType(0), m_ConstVal(val), m_Name(name) {m_TypeMe();}
        Uniform<T>::Uniform(const std::string& name, const T *const val) : m_ValueType(1), m_PtrVal(val), m_Name(name) {m_TypeMe();}
        Uniform<T>::Uniform(const std::string& name, Ref<T> val) : m_ValueType(2), m_RefVal(val), m_Name(name) {m_TypeMe();}
        Uniform<T>::Uniform(const std::string& name, std::function<T()> fn) : m_ValueType(3), m_FnVal(val), m_Name(name) {m_TypeMe();}
        Uniform<Texture2D>::Uniform(const Ref<Texture2D> texture);
        Uniform<TextureCubeMap>::Uniform(const Ref<TextureCubeMap> texture);

        const T GetValue() const {return m_GetValue();}
        void SetValue(T& val);

        inline const std::string& GetName() const {return m_Name;}


    private:

        constexpr T m_GetValue() const;

        std::string m_Name;

        int m_ValueType;

        std::function<T()> m_FnVal;
        const T *const m_PtrVal;
        const Ref<T> m_RefVal;
        const T m_ConstVal;

        // std::unordered_map<int, int> m_LocationIDs;
        // TODO:
        // Make a way for shaders to remember their location, for fewer interactions with the GPU
        // Plan is to map locations to shaderIDs, in case the same uniform is used for multiple shaders

    };
}