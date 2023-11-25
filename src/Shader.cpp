/*
Author: Mattis DALLEAU
Location: Keimyung University
File: Shader.cpp
License: GNU v3.0
*/

#include "renn/Shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace renn {

    GLuint compileShader(const std::string& source, GLenum type) {
        std::cerr << "Compiling shader " << source << std::endl;

        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(shader);
            throw ShaderException("Failed to compile shader: " + std::string(infoLog.begin(), infoLog.end()));
        }

        return shader;
    }

    Shader::Shader(const std::vector<GLuint> &shaders, const std::vector<std::string>& uniforms) {
        m_program = glCreateProgram();
        for (const auto& shader : shaders) {
            glCall(glAttachShader(m_program, shader));
        }
        glCall(glLinkProgram(m_program));

        GLint isLinked = 0;
        glCall(glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked));
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glCall(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength));
            std::vector<GLchar> infoLog(maxLength);
            glCall(glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]));
            glCall(glDeleteProgram(m_program));
            throw ShaderException("Failed to link shader: " + std::string(infoLog.begin(), infoLog.end()));
        }

        glCall(glValidateProgram(m_program));
        GLint programValidationStatus;
        glCall(glGetProgramiv(m_program, GL_VALIDATE_STATUS, &programValidationStatus));
        if (programValidationStatus == GL_FALSE) {
            GLint maxLength = 0;
            glCall(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength));
            std::vector<GLchar> infoLog(maxLength);
            glCall(glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]));
            glCall(glDeleteProgram(m_program));
            throw ShaderException("Failed to validate shader: " + std::string(infoLog.begin(), infoLog.end()));
        }

        for (const auto& shader : shaders) {
            glCall(glDetachShader(m_program, shader));
        }

        for (const auto& uniform : uniforms) {
            try {
                addUniform(uniform);
            } catch (const ShaderException& e) {
                glCall(glDeleteProgram(m_program));
                std::cout << "Failed to add uniform " << uniform << ": " << e.what() << std::endl;
                throw ShaderException("Failed to add uniform " + uniform + ": " + e.what());
            }
        }
    }

    void Shader::use() const {
        glCall(glUseProgram(m_program));
    }

    void Shader::disable() const {
        glCall(glUseProgram(0));
    }

    void Shader::setUniform(const std::string& name, const glm::mat4* matrices, const size_t& count) const {
        glCall(glUniformMatrix4fv(getUniform(name), count, GL_FALSE, glm::value_ptr(*matrices)));
    }

    void Shader::setUniform(const std::string& name, const glm::mat3* matrices, const size_t& count) const {
        glCall(glUniformMatrix3fv(getUniform(name), count, GL_FALSE, glm::value_ptr(*matrices)));
    }

    void Shader::setUniform(const std::string& name, const glm::vec3* vectors, const size_t& count) const {
        glCall(glUniform3fv(getUniform(name), count, glm::value_ptr(*vectors)));
    }

    void Shader::setUniform(const std::string& name, const glm::vec4* vectors, const size_t& count) const {
        glCall(glUniform4fv(getUniform(name), count, glm::value_ptr(*vectors)));
    }

    void Shader::setUniform(const std::string& name, const glm::ivec3* vectors, const size_t& count) const {
        glCall(glUniform3iv(getUniform(name), count, glm::value_ptr(*vectors)));
    }

    void Shader::setUniform(const std::string& name, const glm::ivec4* vectors, const size_t& count) const {
        glCall(glUniform4iv(getUniform(name), count, glm::value_ptr(*vectors)));
    }   

    void Shader::setUniform(const std::string& name, const glm::vec2* vectors, const size_t& count) const {
        glCall(glUniform2fv(getUniform(name), count, glm::value_ptr(*vectors)));
    }

    void Shader::setUniform(const std::string& name, const glm::ivec2* vectors, const size_t& count) const {
        glCall(glUniform2iv(getUniform(name), count, glm::value_ptr(*vectors)));
    }

    void Shader::setUniform(const std::string& name, const GLfloat* values, const size_t& count) const {
        glCall(glUniform1fv(getUniform(name), count, values));
    }

    void Shader::setUniform(const std::string& name, const GLint* values, const size_t& count) const {
        glCall(glUniform1iv(getUniform(name), count, values));
    }

    void Shader::addUniform(const std::string& name) {
        GLint location = glGetUniformLocation(m_program, name.c_str());
        if (location == -1) {
            std::cerr << "Failed to add uniform " << name << std::endl;
            return;
            //throw ShaderException("Failed to add uniform " + name);
        }
        m_uniforms[name] = location;
    }

    GLint Shader::getUniform(const std::string& name) const {
        auto it = m_uniforms.find(name);
        if (it == m_uniforms.end()) {
            throw ShaderException("Failed to get uniform " + name);
        }
        return it->second;
    }

    Shader::~Shader() {
        glCall(glDeleteProgram(m_program));
    }
}