#include "main.h"

#ifndef GROUND_H
#define GROUND_H


class Ground {
public:
    Ground() {}
    Ground(float top, float bottom);
    glm::vec3 position;
    void draw(glm::mat4 VP);
private:
    VAO *object1;
    VAO *object2;
};

#endif
