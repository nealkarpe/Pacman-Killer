#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->radius = 0.2;
    this->grounded = false;
    this->sink = false;
    this->waterSpeed = 0.01; // Terminal speed of falling down in water
    this->waterSpeedLR = 0.01; // Speed of movement along pond base
    this->waterMovement = 0.02; // Speed of left-right key press in water
    speed = 0.04; // Speed of left-right key press in air
    speedVertical = 0.0; // Vertical speed in air - UP
    speedVerticalDown = 0.0; // Vertical speed in air - DOWN
    goDown = false;
    acc = 0.01; // Gravity acceleration
    initVelocity = 0.16;

    this->magVelocity = 0.0;
    this->magAcceleration = 0.0002;

    this->speedHoriz = 0.0;

    float n = 360;
    float r = this->radius;
    float theta = 2*3.14159/n;

    GLfloat vertex_buffer_data[360*9];

    int i;
    for(i=0; i<n; i++)
    {
            float angleBefore = theta * i;
            float angle = angleBefore + theta;

            float x = r * cos(angle);
            float y = r * sin(angle);
            float z = 0.0f;

            float x1 = r * cos(angleBefore);
            float y1 = r * sin(angleBefore);
            float z1 = 0.0f;

            float x2 = 0.0f;
            float y2 = 0.0f;
            float z2 = 0.0f;

            vertex_buffer_data[i*9] = x2;
            vertex_buffer_data[i*9+1] = y2;
            vertex_buffer_data[i*9+2] = z2;
            vertex_buffer_data[i*9+3] = x1;
            vertex_buffer_data[i*9+4] = y1;
            vertex_buffer_data[i*9+5] = z1;
            vertex_buffer_data[i*9+6] = x;
            vertex_buffer_data[i*9+7] = y;
            vertex_buffer_data[i*9+8] = z;
    }

    this->object = create3DObject(GL_TRIANGLES, 360*3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->grounded = false;

    if(this->speedVertical > 0)
    {
        this->position.y += speedVertical;
        this->speedVertical -= this->acc;
        if(this->speedVertical<=0){
            this->goDown = true;
            this->speedVerticalDown = 0.0;
        }
    }

    if(this->goDown)
    {
        if(this->sink && this->position.y<floorHeight)
        {
            this->goDown=false;
        }
        else
        {
            if(this->position.y - this->speedVerticalDown > floorHeight) this->position.y -= this->speedVerticalDown;
            else
            {
                this->position.y = floorHeight;
                this->goDown = false;
                this->speedVerticalDown = 0.0;
                this->grounded = true;

                this->speedHoriz = 0.0;
            }
            this->speedVerticalDown += this->acc;
        }
    }

    if(!this->sink)
    {
        this->position.x += this->speedHoriz;
        this->position.x += this->magVelocity;
    }
}

void Ball::fallWater(float pondX, float pondY, float R)
{
    if(!this->sink) return;

    float r = R - this->radius;
    float diff = sqrt(r*r - (this->position.x - pondX)*(this->position.x - pondX));
    float correspondingY = pondY - diff;

    if(this->position.y - this->waterSpeed > correspondingY) this->position.y -= this->waterSpeed;
    else
    {
        this->position.y = correspondingY;
    }
}

void Ball::rollAlongPond(float pondX, float pondY, float R)
{
    if(!this->sink || this->position.x==pondX) return;

    float r = R - this->radius;
    float diff = sqrt(r*r - (this->position.x - pondX)*(this->position.x - pondX));
    float correspondingY = pondY - diff;

    if(this->position.y > correspondingY) return;

    if(abso(this->position.x - pondX) > this->waterSpeedLR)
    {
        if(this->position.x > pondX) this->position.x -= this->waterSpeedLR;
        else if(this->position.x < pondX) this->position.x += this->waterSpeedLR;
        diff = sqrt(r*r - (this->position.x - pondX)*(this->position.x - pondX));
        this->position.y = pondY - diff;
    }
    else
    {
        this->position.x = pondX;
        this->position.y = pondY - R + this->radius;
    }
}

void Ball::magTick(int type)
{
    this->magVelocity += (type==2 ? -1.0 : 1.0) * this->magAcceleration;
    if(this->sink) magVelocity = 0;
}
