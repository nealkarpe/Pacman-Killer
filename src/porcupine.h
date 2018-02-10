#include "main.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H


class Porcupine {
public:
    Porcupine() {}
    Porcupine(float lbound, float rbound, float y, float R);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void tick();
    float R;
    double speed;
    float lbound;
    float rbound;
    float height;
    bool alive;
private:
    VAO *object;
};

#endif
