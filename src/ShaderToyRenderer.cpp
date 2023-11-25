/*
Author: Mattis DALLEAU
Location: Keimyung University
File: ShaderToyRenderer.cpp 
License: GNU v3.0
*/

#include "renn/ShaderToyRenderer.hpp"

namespace renn {

static const char *shader_toy_lib = R"(
#version 450

precision highp float;
precision highp int;
precision highp sampler2D;

in vec2 vtexCoord;
layout(location = 0) out vec4 fragColor;

uniform vec3 iResolution;
uniform float iTime;
uniform float iTimeDelta;
uniform int iFrame;
uniform float iFrameRate;
uniform float iChannelTime[4];
uniform vec3 iChannelResolution[4];
uniform vec4 iMouse;
layout(binding = 0) uniform sampler2D iChannel0;
layout(binding = 1) uniform sampler2D iChannel1;
layout(binding = 2) uniform sampler2D iChannel2;
layout(binding = 3) uniform sampler2D iChannel3;
uniform vec4 iDate;
uniform float iSampleRate;
)";

static const char *shader_toy_main = R"(
void main(void) {
	fragColor = vec4(1.0, 0.0, 0.0, 1.);
    mainImage(fragColor, vtexCoord.xy * iResolution.xy);
}
)";

static const char *shader_toy_vertex_shader = R"(
precision highp float;
precision highp int;
precision highp sampler2D;

// Vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

// Texture coord for fragment
out vec2 vtexCoord;

void main() {
	vtexCoord = texCoord;
	gl_Position = vec4(position, 1.0);
}
    )";

static const GLfloat shader_toy_data[] = {
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // Top-left
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // Top-right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f  // Bottom-right
};

ShaderToyRenderer::ShaderToyRenderer(Window& window, const std::string& fragmentShaderSource)
    : m_shader(renn::buildShader(
        shader_toy_vertex_shader, GL_VERTEX_SHADER,
        shader_toy_lib + fragmentShaderSource + shader_toy_main, GL_FRAGMENT_SHADER),
        {"iResolution", "iTime", "iTimeDelta", "iFrame", "iFrameRate", "iMouse", "iDate"})
{
    ShaderToyUniforms uniforms;
    std::memset(&uniforms, 0, sizeof(ShaderToyUniforms));
    std::memset(&m_new_uniforms, 0, sizeof(ShaderToyUniforms));
    uniforms.m_resolution = glm::vec3(window.getSize(), 0.0f);
    std::memcpy(&m_old_uniforms, &uniforms, sizeof(ShaderToyUniforms));

    glCall(glGenVertexArrays(1, &m_quad_vao));
    glCall(glBindVertexArray(m_quad_vao));

    glCall(glGenBuffers(1, &m_quad_vbo));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo));
    glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(shader_toy_data), shader_toy_data, GL_STATIC_DRAW));

    glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0));
    glCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3)));
    glCall(glEnableVertexAttribArray(0));
    glCall(glEnableVertexAttribArray(1));

    glCall(glBindVertexArray(0));

    Update(window, glm::vec2(0.0f), false, false);
}

#define CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(x) try { x; } catch (const ShaderException&) {}

void ShaderToyRenderer::Update(Window& window, const glm::vec2& mousePos, bool mouseButtonLeft, bool mouseButtonRight)
{
    m_shader.use();

    m_new_uniforms.m_time = (GLfloat)glfwGetTime();
    m_new_uniforms.m_timeDelta = m_new_uniforms.m_time - m_old_uniforms.m_time;
    m_new_uniforms.m_frameRate = 1.0f / m_new_uniforms.m_timeDelta;
    m_new_uniforms.m_frame = m_old_uniforms.m_frame + 1;
    m_new_uniforms.m_mouse = glm::vec4(mousePos, mouseButtonLeft ? 1.0f : 0.0f, mouseButtonRight ? 1.0f : 0.0f);

    if (m_new_uniforms.m_resolution != glm::vec3(window.getSize(), 0.0f)) {
        m_new_uniforms.m_resolution = glm::vec3(window.getSize(), 0.0f);
        CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iResolution", &m_new_uniforms.m_resolution, 1));
    }

    CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iTime", &m_new_uniforms.m_time, 1));
    CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iTimeDelta", &m_new_uniforms.m_timeDelta, 1));
    CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iFrameRate", &m_new_uniforms.m_frameRate, 1));
    CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iFrame", &m_new_uniforms.m_frame, 1));

    if (memcmp(&m_new_uniforms.m_channelTime, &m_old_uniforms.m_channelTime, sizeof(GLfloat) * 4) != 0) {
        CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iChannelTime", m_new_uniforms.m_channelTime, 4));
    }
    if (memcmp(&m_new_uniforms.m_channelResolution, &m_old_uniforms.m_channelResolution, sizeof(glm::vec3) * 4) != 0) {
        CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iChannelResolution", m_new_uniforms.m_channelResolution, 4));
    }

    if (m_new_uniforms.m_mouse != m_old_uniforms.m_mouse) {
        CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iMouse", &m_new_uniforms.m_mouse, 1));
    }

    // iChannel0..3 ?

    if (m_new_uniforms.m_date != m_old_uniforms.m_date) {
        CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iDate", &m_new_uniforms.m_date, 1));
    }

    if (m_new_uniforms.m_sampleRate != m_old_uniforms.m_sampleRate) {
        CATCH_SHADER_EXCEPTION_AND_DO_NOTHING(m_shader.setUniform("iSampleRate", &m_new_uniforms.m_sampleRate, 1));
    }

    m_old_uniforms = m_new_uniforms;
}

void ShaderToyRenderer::Render() const
{
    glCall(glViewport(0, 0, m_new_uniforms.m_resolution.x, m_new_uniforms.m_resolution.y));
    m_shader.use();
    glCall(glBindVertexArray(m_quad_vao));
    glCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
}

}