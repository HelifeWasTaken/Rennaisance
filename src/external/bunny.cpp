#include "bunnydata2.h"
#include "external/Bunny.hpp"

#include <iostream>

Bunny::Bunny()
{
    glCall(glGenVertexArrays(1, &vao));
    glCall(glBindVertexArray(vao));

    glCall(glGenBuffers(1, &vbo));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    // vec3 position, vec3 normal, vec2 uv 
    glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertices), modelVertices, GL_STATIC_DRAW));
    glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), nullptr));
    glCall(glEnableVertexAttribArray(0));
    glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)offsetof(ModelVertex, normal)));
    glCall(glEnableVertexAttribArray(1));
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)offsetof(ModelVertex, uv));
    //glEnableVertexAttribArray(2);

    glCall(glGenBuffers(1, &ibo));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndices), modelIndices, GL_STATIC_DRAW));
    std::cout << "sizeof(modelIndices): " << sizeof(modelIndices) << std::endl;
    int size;
    glCall(glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size));
    std::cout << "Esize: " << size << std::endl;

    glBindVertexArray(0);
}

void Bunny::draw() const
{
    glCall(glBindVertexArray(vao));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    GLint64 size;
    glCall(glGetBufferParameteri64v(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size));
    glCall(glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, nullptr));
    glCall(glBindVertexArray(0));
}

Bunny::~Bunny()
{
    glCall(glDeleteBuffers(1, &ibo));
    glCall(glDeleteBuffers(1, &vbo));
    glCall(glDeleteVertexArrays(1, &vao));
}