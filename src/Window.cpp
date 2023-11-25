/*
Author: Mattis DALLEAU
Location: Keimyung University
File: Window.cpp
License: GNU v3.0
*/

#include "renn/Window.hpp"
#include <mutex>

static bool initialized = false;
static int windowCount = 0;

namespace renn {
    // dummy callbacks
    void d_keyCallback(GLFWwindow*, int, int, int, int) {}
    void d_cursorPosCallback(GLFWwindow*, double, double) {}
    void d_mouseButtonCallback(GLFWwindow*, int, int, int) {}
    void d_scrollCallback(GLFWwindow*, double, double) {}
    void d_windowSizeCallback(GLFWwindow*, int, int) {}

    static void initialize_glfw() {
        windowCount++;
        if (!initialized) {
            if (!glfwInit()) {
                throw WindowException("Failed to initialize GLFW");
            }
            initialized = true;
        }
    }

    static void uninitialized_glfw() {
        windowCount--;
        if (initialized && windowCount == 0) {
            glfwTerminate();
            initialized = false;
        }
        if (windowCount < 0) {
            throw WindowException("Window count is negative");
        }
    }

    Window::Window(const std::string& title, const glm::ivec2& size) : m_title(title), m_size(size) {
        initialize_glfw();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, RENN_WINDOW_OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, RENN_WINDOW_OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, RENN_WINDOW_OPENGL_PROFILE);

        m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);
        if (!m_window) {
            glfwTerminate();
            throw WindowException("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);

        if (gl3wInit()) {
            throw WindowException("Failed to initialize GL3W");
        }
        if (!gl3wIsSupported(RENN_WINDOW_OPENGL_VERSION_MAJOR, RENN_WINDOW_OPENGL_VERSION_MINOR)) {
            throw WindowException("OpenGL " + std::to_string(RENN_WINDOW_OPENGL_VERSION_MAJOR) + "." + std::to_string(RENN_WINDOW_OPENGL_VERSION_MINOR) + " is not supported");
        }
        setClearColor(glm::vec4(0.0f, 0.0f, 0.4f, 0.0f));
        setCallbacks();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
                       | ImGuiConfigFlags_DockingEnable
                     //| ImGuiConfigFlags_ViewportsEnable;
                     ;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init();
    }

    void Window::setTitle(const std::string& title) {
        m_title = title;
        glfwSetWindowTitle(m_window, m_title.c_str());
    }

    void Window::setSize(const glm::ivec2& size) {
        m_size = size;
        glfwSetWindowSize(m_window, m_size.x, m_size.y);
    }

    const std::string& Window::getTitle() const {
        return m_title;
    }

    const glm::ivec2& Window::getSize() const {
        return m_size;
    }

    void Window::pollEvents() const {
        glfwPollEvents();
    }

    void Window::render() const {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(m_window);
        }
        glfwSwapBuffers(m_window);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    void Window::close() const {
        glfwSetWindowShouldClose(m_window, true);
    }

    void Window::clear() const {
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::setClearColor(const glm::vec4& color) const {
        glCall(glClearColor(color.r, color.g, color.b, color.a));
    }

    void Window::setCallbacks(
            void (*keyCallback)(GLFWwindow*, int, int, int, int),
            void (*cursorPosCallback)(GLFWwindow*, double, double),
            void (*mouseButtonCallback)(GLFWwindow*, int, int, int),
            void (*scrollCallback)(GLFWwindow*, double, double),
            void (*windowSizeCallback)(GLFWwindow*, int, int)
    ) {
        keyCallback = keyCallback ? keyCallback : d_keyCallback;
        cursorPosCallback = cursorPosCallback ? cursorPosCallback : d_cursorPosCallback;
        mouseButtonCallback = mouseButtonCallback ? mouseButtonCallback : d_mouseButtonCallback;
        scrollCallback = scrollCallback ? scrollCallback : d_scrollCallback;
        windowSizeCallback = windowSizeCallback ? windowSizeCallback : d_windowSizeCallback;

        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetCursorPosCallback(m_window, cursorPosCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetScrollCallback(m_window, scrollCallback);
        glfwSetWindowSizeCallback(m_window, windowSizeCallback);
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        uninitialized_glfw();
    }
}