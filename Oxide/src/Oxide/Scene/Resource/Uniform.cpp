#include "Uniform.h"

namespace Oxide {

    template <class T>
    const T Uniform<T>::GetValue() const {
        switch (m_ValueType) {
        case 0:
            return fnVal();

        case 1:
            return *m_PtrVal;
        
        case 2:
            return m_ConstVal;
        }
    }

    template <class T>
    Uniform<T>::Uniform(const std::string& name, const T *const val) : m_ValueType(0), m_ConstVal(val), m_Name(name) {}

    template <class T>
    Uniform<T>::Uniform(const std::string& name, const T *const val) : m_ValueType(0), m_PtrVal(val), m_Name(name) {}

    template <class T>
    Uniform<T>::Uniform(const std::string& name, std::function<T()> fn) : m_ValueType(0), m_FnVal(val), m_Name(name) {}
}