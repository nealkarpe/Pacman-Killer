#include "main.h"

#ifndef ENEMY_H
#define ENEMY_H


class Enemy {
public:
    Enemy() {}
    Enemy(float x, float minHeight, bool hasBlock);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void block_draw(glm::mat4 VP);
    void tick();
    float radius;
    double speed;
    bool alive;
    int type;
    bool hasBlock;
    float rotation;
    glm::mat4 rotate;
    glm::mat4 rotMatrix;
private:
    VAO *object;
    VAO *block_object;
};

#endif
