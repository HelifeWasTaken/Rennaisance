#pragma once

/*
Author: Mattis DALLEAU
Location: Keimyung University
File: Transform.hpp
License: GNU v3.0
*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace renn {

class Transform {
private:
    glm::vec3 m_translation;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;
    mutable glm::mat4 m_model;
    mutable bool m_dirty;

public:
    Transform();

    Transform(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation);

    const glm::vec3& getTranslation() const;
    const glm::vec3& getScale() const;
    const glm::vec3& getRotation() const;

    void setTranslation(const glm::vec3& translation);
    void setScale(const glm::vec3& scale);
    void setRotation(const glm::vec3& rotation);

    const glm::mat4& getModel() const;
};

}