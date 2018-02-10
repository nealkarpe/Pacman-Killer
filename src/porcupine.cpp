#include "porcupine.h"
#include "main.h"

Porcupine::Porcupine(float lbound, float rbound, float y, float R) {
    this->R = R;
    this->lbound = lbound;
    this->rbound = rbound;
    this->position = glm::vec3(lbound+R, y, 0);
    this->height = 0.2;
    this->speed = 0.01;
    this->alive = true;
    float r = R/3;
    float h = this->height;

    static const GLfloat vertex_buffer_data[] = {
        -3*r, 0, 0,
        -2*r, h, 0,
        -r, 0, 0,

        -r, 0, 0,
        0, h, 0,
        r, 0, 0,

        r, 0, 0,
        2*r, h, 0,
        3*r, 0, 0,
    };

    this->object = create3DObject(GL_TRIANGLES, 3*3, vertex_buffer_data, COLOR_GOLD, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::tick() {
    this->position.x += this->speed;
    if(this->position.x+R >= this->rbound || this->position.x-R <= this->lbound) this->speed = -this->speed;
}
