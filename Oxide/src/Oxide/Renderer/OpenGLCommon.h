#pragma once

#include "Oxide/Core/Base.h"
#include <GL/glew.h>
#include <tracy/TracyOpenGL.hpp>

namespace Oxide {

    static size_t OpenGLGetOxideTypeSize(const OxideType& type) {

        const size_t OxideTypeSize[] = {
            0,
            4,
            4,
            4,
            8,
            4,
		    2,
		    2
        };
        return OxideTypeSize[type];

    }

    static GLenum OpenGLGetType(const OxideType& type) {

        const GLenum OxideGLType[] = {
            GL_NONE,
            GL_INT,
            GL_UNSIGNED_INT,
            GL_FLOAT,
            GL_DOUBLE,
            GL_BOOL,
            GL_SHORT,
            GL_UNSIGNED_SHORT
        };

        return OxideGLType[type];

    }

}