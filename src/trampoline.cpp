#include "trampoline.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, float bottom) {
    this->position = glm::vec3(x, y, 0);
    this->radius = 0.5;
    this->blockWidth = 0.1;

    float n = 360;
    float r = this->radius;
    float theta = 2*3.14159/n;

    GLfloat vertex_buffer_data[180*9 + 4*9];

    float b = 0.25; //semi minor axis of ellipse

    int i;
    for(i=0; i<180; i++)
    {
        float angleBefore = theta * (180 + i);
        float angle = angleBefore + theta;

        float x3 = r * cos(angle);
        float y3 = b * sin(angle);
        float z3 = 0.0f;

        float x1 = r * cos(angleBefore);
        float y1 = b * sin(angleBefore);
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
        vertex_buffer_data[i*9+6] = x3;
        vertex_buffer_data[i*9+7] = y3;
        vertex_buffer_data[i*9+8] = z3;
    }

    float width = this->blockWidth; // width of side blocks of trampoline
    float dist = y-bottom;

    static const GLfloat static_vertex_buffer_data[] = {
        -r, 0, 0, // vertex 1
        -r, -dist, 0, // vertex 2
        -r-width, 0, 0, // vertex 3

        -r-width, 0, 0, // vertex 3
        -r-width, -dist, 0, // vertex 4
        -r,  -dist, 0, // vertex 2

        r, 0, 0, // vertex 1
        r,  -dist, 0, // vertex 2
        r+width, 0, 0, // vertex 3

        r+width, 0, 0, // vertex 3
        r+width, -dist, 0, // vertex 4
        r,  -dist, 0, // vertex 2
    };

    for(i=0; i<(4*9); i++)
    {
        vertex_buffer_data[180*9 + i] = static_vertex_buffer_data[i];
    }

    this->object = create3DObject(GL_TRIANGLES, 180*3 + 4*3, vertex_buffer_data, COLOR_ORANGE, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
