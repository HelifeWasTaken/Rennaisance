/*
Author: Mattis DALLEAU
Location: Keimyung University
File: ShaderToyRenderer.hpp
License: GNU v3.0
*/
#pragma once

#include "renn/Window.hpp"
#include "renn/Shader.hpp"

namespace renn {

class ShaderToyRenderer {
private:
    renn::Shader m_shader;
    GLuint m_quad_vao, m_quad_vbo;

    struct ShaderToyUniforms {
        glm::vec3 m_resolution;
        GLfloat m_time;
        GLfloat m_timeDelta;
        GLfloat m_frameRate;
        GLint m_frame;
        GLfloat m_channelTime[4];
        glm::vec3 m_channelResolution[4];
        glm::vec4 m_mouse;

        // sampler2D 0..3 ?

        glm::vec4 m_date;
        float m_sampleRate;
    } m_old_uniforms, m_new_uniforms;

public:
    ShaderToyRenderer(Window& window, const std::string& fragmentShaderSource);

    void Update(Window& window, const glm::vec2& mousePos, bool mouseButtonLeft, bool mouseButtonRight);

    void Render() const;
};

}