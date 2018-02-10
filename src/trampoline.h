#include "main.h"

#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H


class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float y, float bottom);
    glm::vec3 position;
    float radius;
    float blockWidth;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif
