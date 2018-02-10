#include "main.h"

#ifndef POND_H
#define POND_H


class Pond {
public:
    Pond() {}
    Pond(float x, float y);
    glm::vec3 position;
    float radius;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif
