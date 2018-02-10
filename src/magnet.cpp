#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->x = x;
    this->y = y;
    this->rotation = 0;
    this->flag = 0;
    this->speed = 0.02;
    this->ticks = 0;

    this->dashIn = 60;
    this->stay = 200;
    this->field = false;

    float top = 0.3;
    float width = 0.15;
    float right = 0.7;
    float left = -0.15;

    int numTriangles = 6;

    static const GLfloat vertex_buffer_data[] = {
        0, top, 0,
        0, top+width, 0,
        right, top+width, 0,

        right, top+width, 0,
        right, top, 0,
        0, top, 0,

        0, -top, 0,
        0, -top-width, 0,
        right, -top-width, 0,

        right, -top-width, 0,
        right, -top, 0,
        0, -top, 0,

        right, top+width, 0,
        right+width, top+width, 0,
        right, -top-width, 0,

        right, -top-width, 0,
        right+width, -top-width, 0,
        right+width, top+width, 0,
    };

    static const GLfloat vertex_buffer_data_2[] = {
        0, top, 0,
        0, top+width, 0,
        left, top+width, 0,

        left, top+width, 0,
        left, top, 0,
        0, top, 0,

        0, -top, 0,
        0, -top-width, 0,
        left, -top-width, 0,

        left, -top-width, 0,
        left, -top, 0,
        0, -top, 0,
    };

    this->object = create3DObject(GL_TRIANGLES, numTriangles*3, vertex_buffer_data, COLOR_DARKRED, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data_2, COLOR_LIGHTGREY, GL_FILL);
}

void Magnet::start(int type)
{
    if(type==2)
    {
        this->rotation = 180;
        this->position = glm::vec3(-this->x, this->y, 0);
    }
    else
    {
        this->rotation = 0;
        this->position = glm::vec3(this->x, this->y, 0);
    }

    this->flag=type;
    this->ticks=0;
}

void Magnet::tick()
{
    if(this->flag==0) return;

        if(this->ticks < this->dashIn) this->position.x += (flag==2 ? 1.0 : -1.0) * this->speed;
        else if(this->ticks >= dashIn && this->ticks < (dashIn+stay)) this->field=true;
        else if(this->ticks >= (dashIn+stay) && this->ticks < (2*dashIn+stay))
        {
            this->field=false;
            this->position.x -= (flag==2 ? 1.0 : -1.0) * this->speed;
        }
        else if(this->ticks==(2*dashIn+stay))
        {
            this->flag=0;
            this->ticks=0;
        }
        this->ticks++;
}

void Magnet::draw(glm::mat4 VP) {
    if(this->flag==0) return;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object2);
}
