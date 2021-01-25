#pragma once

#include "Oxide/Core/Base.h"

namespace Oxide {

    template<class T>
    class Uniform {
    public:

        Uniform(const std::string& name, const T val);
        Uniform(const std::string& name, const T *const val);
        Uniform(const std::string& name, std::function<T()> fn);

        const T GetValue() const {return m_GetValue();}

        inline const std::string& GetName() const {return m_Name;}

    private:

        constexpr T m_GetValue() const;

        std::string m_Name;

        int m_ValueType;

        std::function<T()> m_FnVal;
        const T *const m_PtrVal;
        const T m_ConstVal;

        // std::unordered_map<int, int> m_LocationIDs;\@
        // TODO:
        // Make a way for shaders to remember their location, for fewer interactions with the GPU
        // Plan is to map locations to shaderIDs, in case the same uniform is used for multiple shaders

    };
}