#pragma once

/*
Author: Mattis DALLEAU
Location: Keimyung University
File: Window.hpp
License: GNU v3.0
*/

#include "GL.hpp"

#include "renn/Utils.h"

#include <string>
#include <glm/glm.hpp>
#include <exception>
#include <functional>
#include <stdexcept>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#ifndef RENN_WINDOW_OPENGL_VERSION_MAJOR
#define RENN_WINDOW_OPENGL_VERSION_MAJOR 4
#endif

#ifndef RENN_WINDOW_OPENGL_VERSION_MINOR
#define RENN_WINDOW_OPENGL_VERSION_MINOR 2
#endif

#if (RENN_WINDOW_OPENGL_VERSION_MAJOR << 8 | RENN_WINDOW_OPENGL_VERSION_MINOR << 4) >= 0x420
#define RENN_OPENGL_GLSL_VERSION "#version " RENN_STRINGIFY_MACRO(RENN_WINDOW_OPENGL_VERSION_MAJOR) RENN_STRINGIFY_MACRO(RENN_WINDOW_OPENGL_VERSION_MINOR) "0 core\n"
#else
#define RENN_OPENGL_GLSL_VERSION "#version 420 core\n"
#pragma message("OpenGL version is lower than 4.2, using #version 420 core this may cause issues")
#endif

#ifndef RENN_WINDOW_OPENGL_PROFILE
#define RENN_WINDOW_OPENGL_PROFILE GLFW_OPENGL_CORE_PROFILE
#endif

namespace renn {
    class Window {
    public:
        explicit Window(const std::string& title, const glm::ivec2& size);

        void setTitle(const std::string& title);
        void setSize(const glm::ivec2& size);

        [[nodiscard]] const std::string& getTitle() const;
        [[nodiscard]] const glm::ivec2& getSize() const;

        void pollEvents() const;
        void render() const; // (swap buffers)

        [[nodiscard]] bool shouldClose() const;
        void close() const;

        void clear() const;
        void setClearColor(const glm::vec4& color) const;

        void setCallbacks(
            void (*keyCallback)(GLFWwindow*, int, int, int, int)=nullptr,
            void (*cursorPosCallback)(GLFWwindow*, double, double)=nullptr,
            void (*mouseButtonCallback)(GLFWwindow*, int, int, int)=nullptr,
            void (*scrollCallback)(GLFWwindow*, double, double)=nullptr,
            void (*windowSizeCallback)(GLFWwindow*, int, int)=nullptr
        );

        ~Window();

        GLFWwindow* getHandle() { return m_window; }

    private:
        std::string m_title;
        glm::ivec2 m_size;
        GLFWwindow* m_window;
    };

    class WindowException : public std::runtime_error {
    public:
        explicit WindowException(const std::string& message) : std::runtime_error(message) {}
    };
}
