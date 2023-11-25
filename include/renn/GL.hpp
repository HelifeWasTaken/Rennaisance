#pragma once

/*
Author: Mattis DALLEAU
Location: Keimyung University
File: GL.hpp
License: GNU v3.0
*/

#include <glm/glm.hpp>
#include <GL/gl3w.h>
#include <glfw/glfw3.h>

#include <iostream>

//#ifndef RENN_DEBUG
//#define RENN_DEBUG
//#endif

#ifndef glCall

    #ifdef RENN_DEBUG
        #define RENN_LOG(x, file, line) Log(x, file, line)
    #else
        #define RENN_LOG(x, file, line)
    #endif

    #define glCall(x) GLClearError();\
        x;\
        RENN_LOG(#x, __FILE__, __LINE__);\
        GLCheckError(#x, __FILE__, __LINE__)

static inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static inline void GLCheckError(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
    }
}

static inline void Log(const char* x, const char* file, int line)
{
    std::cout << "[OpenGL Call] " << x << " " << file << ":" << line << std::endl;
}

#endif