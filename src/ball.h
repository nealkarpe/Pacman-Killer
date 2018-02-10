#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void fallWater(float pondX, float pondY, float R);
    void rollAlongPond(float pondX, float pondY, float R);
    void magTick(int type);
    double speed;
    double speedVertical;
    double speedVerticalDown;
    bool goDown;
    double acc;
    double initVelocity;
    bool grounded;
    bool sink;
    float waterSpeed;
    float waterSpeedLR;
    float waterMovement;
    float magVelocity;
    float magAcceleration;
    float speedHoriz;
private:
    VAO *object;
};

#endif // BALL_H
