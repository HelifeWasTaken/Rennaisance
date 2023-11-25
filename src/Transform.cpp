#include "renn/Transform.hpp"

namespace renn {

    Transform::Transform()
        : m_translation(0.0f), m_scale(1.0f), m_rotation(0.0f), m_dirty(true)
    {}

    Transform::Transform(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation)
        : m_translation(translation), m_scale(scale), m_rotation(rotation), m_dirty(true)
    {}

    const glm::vec3& Transform::getTranslation() const
    {
        return m_translation;
    }

    const glm::vec3& Transform::getScale() const
    {
        return m_scale;
    }

    const glm::vec3& Transform::getRotation() const
    {
        return m_rotation;
    }

    void Transform::setTranslation(const glm::vec3& translation)
    {
        m_translation = translation;
        m_dirty = true;
    }

    void Transform::setScale(const glm::vec3& scale)
    {
        m_scale = scale;
        m_dirty = true;
    }

    void Transform::setRotation(const glm::vec3& rotation)
    {
        m_rotation = rotation;
        m_dirty = true;
    }

    const glm::mat4& Transform::getModel() const
    {
        if (m_dirty) {
            m_model = glm::translate(glm::mat4(1.0f), m_translation);
            m_model = glm::rotate(m_model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            m_model = glm::scale(m_model, m_scale);
            m_dirty = false;
        }
        return m_model;
    }
}