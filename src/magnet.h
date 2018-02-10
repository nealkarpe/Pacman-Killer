#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y);
    glm::vec3 position;
    float rotation;
    int flag;
    int ticks;
    int dashIn;
    int stay;
    float x;
    float y;
    float speed;
    bool field;
    void draw(glm::mat4 VP);
    void start(int type);
    void tick();
private:
    VAO *object;
    VAO *object2;
};

#endif
