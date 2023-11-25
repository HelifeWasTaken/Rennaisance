#pragma once

/*
Author: Mattis DALLEAU
Location: Keimyung University
File: Shader.hpp
License: GNU v3.0
*/

#include "renn/GL.hpp"

#include <string>
#include <vector>
#include <unordered_map>

#include <stdexcept>
#include <typeinfo>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renn/Utils.h"

namespace renn {

    GLuint compileShader(const std::string& source, GLenum type);
    
    class Shader {
    private:
        GLuint m_program;
        std::unordered_map<std::string, GLint> m_uniforms;

    public:
        Shader(const std::vector<GLuint> &shaders, const std::vector<std::string>& uniforms={});

        void use() const;
        void disable() const;

        void setUniform(const std::string& name, const glm::mat4* matrices, const size_t& count) const;
        void setUniform(const std::string& name, const glm::mat3* matrices, const size_t& count) const;
        void setUniform(const std::string& name, const glm::vec3* vectors, const size_t& count) const;
        void setUniform(const std::string& name, const glm::vec4* vectors, const size_t& count) const;
        void setUniform(const std::string& name, const glm::ivec3* vectors, const size_t& count) const;
        void setUniform(const std::string& name, const glm::ivec4* vectors, const size_t& count) const;
        void setUniform(const std::string& name, const glm::vec2* vectors, const size_t& count) const;
        void setUniform(const std::string& name, const glm::ivec2* vectors, const size_t& count) const;
        void setUniform(const std::string& name, const GLfloat* values, const size_t& count) const;
        void setUniform(const std::string& name, const GLint* values, const size_t& count) const;

        template<typename T>
        void setUniform(const std::string& name, const T* values, const size_t& count) const {
            throw ShaderException("Unsupported uniform type " + std::string(typeid(T).name()));
        }

        void addUniform(const std::string& name);
        GLint getUniform(const std::string& name) const;

        ~Shader();
    };

    class ShaderException : public std::runtime_error {
    public:
        explicit ShaderException(const std::string& message) : std::runtime_error(message) {}
    };


    template<typename... Args> static inline std::vector<GLuint> buildShader(const std::string& source, GLenum type, Args&&... args);
    static inline std::vector<GLuint> buildShaderHelper(std::vector<GLuint>& shaders);
    template<typename... Args> static inline std::vector<GLuint> buildShaderHelper(std::vector<GLuint>& shaders, const std::string& source, GLenum type, Args&&... args);

    template<typename... Args>
    static inline std::vector<GLuint> buildShaderHelper(std::vector<GLuint>& shaders, const std::string& source, GLenum type, Args&&... args) {
        shaders.push_back(compileShader(source, type));
        return buildShaderHelper(shaders, std::forward<Args>(args)...);
    }
    static inline std::vector<GLuint> buildShaderHelper(std::vector<GLuint>& shaders) {
        return shaders;
    }
    template<typename... Args>
    static inline std::vector<GLuint> buildShader(const std::string& source, GLenum type, Args&&... args) {
        std::vector<GLuint> shaders;
        const auto delete_shaders_on_error = [&shaders]() {
            for (auto shader : shaders) {
                glDeleteShader(shader);
            }
        };

        try {
            shaders.push_back(compileShader(source, type));
            return buildShaderHelper(shaders, std::forward<Args>(args)...);
        } catch (const ShaderException& e) {
            delete_shaders_on_error();
            throw e;
        } catch (const std::exception& e) {
            delete_shaders_on_error();
            throw e;
        }
    }

}
