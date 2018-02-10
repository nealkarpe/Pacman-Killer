#include "enemy.h"
#include "main.h"

Enemy::Enemy(float x, float minHeight, bool hasBlock) {

    this->alive = true;
    int max_angle = 40;
    this->rotation = (rand()%(1+2*max_angle)) - max_angle;
    this->rotate = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));

    float diff = 2.0 - minHeight;
    float multiplier = ((float) rand()) / (float) RAND_MAX;
    float y = minHeight + diff*multiplier;

    float Radii[4] = {0.15,0.2,0.25,0.3};
    color_t Colors[4] = {COLOR_SEAGREEN, COLOR_YELLOW, COLOR_RED, COLOR_BLUE};

    this->position = glm::vec3(x, y, 0);

    float diff2 = 0.02;
    float multiplier2 = ((float) rand()) / (float) RAND_MAX;
    this->speed = 0.005 + diff2*multiplier2;

    this->type = rand()%4;
    this->radius = Radii[this->type];

    this->hasBlock = hasBlock;

    float n = 360;
    float r = this->radius;
    float theta = 2*3.14159/n;

    int numTriangles = 300;

    GLfloat vertex_buffer_data[numTriangles*9];

    int i;
    for(i=0; i<numTriangles; i++)
    {
            float angleBefore = theta * (i+20);
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

    this->object = create3DObject(GL_TRIANGLES, numTriangles*3, vertex_buffer_data, Colors[this->type], GL_FILL);


    if(this->hasBlock)
    {
        float left = /*this->position.x*/ - this->radius - 0.2;
        float right = /*this->position.x*/ this->radius + 0.2;
        float down = /*this->position.y*/  this->radius - 0.05;
        float up = down + 0.1;

        static const GLfloat block_vertex_buffer_data[] = {
            left, down, 0,
            left, up, 0,
            right, up, 0,

            right, up, 0,
            right, down, 0,
            left, down, 0
        };

        this->block_object = create3DObject(GL_TRIANGLES, 2*3, block_vertex_buffer_data, COLOR_BROWN, GL_FILL);
    }
}

void Enemy::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Enemy::block_draw(glm::mat4 VP) {
    if(!this->hasBlock) return;

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate * this->rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->block_object);        
}

void Enemy::tick() {
    this->position.x += this->speed;
}
