#include "pond.h"
#include "main.h"

Pond::Pond(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->radius = 1.0;

    float n = 360;
    float r = this->radius;
    float theta = 2*3.14159/n;

    int numTriangles = 180;

    GLfloat vertex_buffer_data[numTriangles*9];

    int i;
    for(i=0; i<numTriangles; i++)
    {
            float angleBefore = theta * (180 + i);
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

    this->object = create3DObject(GL_TRIANGLES, numTriangles*3, vertex_buffer_data, COLOR_DARKBLUE, GL_FILL);
}

void Pond::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
