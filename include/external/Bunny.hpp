/*
Author: Mattis DALLEAU
Location: Keimyung University
File: Bunny.hpp
License: GNU v3.0
*/

#pragma once

#include "renn/GL.hpp"

class Bunny {
private:
    GLuint vao;
    GLuint vbo;
    GLuint ibo;

public:
    Bunny();
    void draw() const;
    ~Bunny();
};