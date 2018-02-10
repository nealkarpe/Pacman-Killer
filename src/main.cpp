#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "pond.h"
#include "trampoline.h"
#include "magnet.h"
#include "porcupine.h"
#include "enemy.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

#define NUM_ENEMIES 1000

Ball player;
Ground ground;
Pond pond;
Trampoline trampoline;
Magnet magnet;
Porcupine porcupines[2];
Enemy enemies[NUM_ENEMIES];
float floorHeight = -1.7;
int i;
int score = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ground.draw(VP);
    pond.draw(VP);
    player.draw(VP);
    trampoline.draw(VP);
    magnet.draw(VP);
    if(porcupines[0].alive) porcupines[0].draw(VP);
    if(porcupines[1].alive) porcupines[1].draw(VP);
    for(i=0; i<NUM_ENEMIES; i++)
    {
        if(enemies[i].alive) {enemies[i].draw(VP); enemies[i].block_draw(VP);}
    }

}

void tick_input(GLFWwindow *window) {
    int A  = glfwGetKey(window, GLFW_KEY_A);
    int D = glfwGetKey(window, GLFW_KEY_D);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    if (A) {
        player.position.x -= (player.sink ? player.waterMovement : player.speed);
    }
    if (D) {
        player.position.x += (player.sink ? player.waterMovement : player.speed);
    }
    if (space) {
        jump();
    }

    int LEFT  = glfwGetKey(window, GLFW_KEY_LEFT);
    int RIGHT = glfwGetKey(window, GLFW_KEY_RIGHT);
    int UP = glfwGetKey(window, GLFW_KEY_UP);
    int DOWN = glfwGetKey(window, GLFW_KEY_DOWN);

    int flag=0;
    if (LEFT) {
        flag=1;
        pan_left();
    }
    if (RIGHT) {
        flag=1;
        pan_right();
    }
    if (UP) {
        flag=1;
        pan_up();
    }
    if (DOWN) {
        flag=1;
        pan_down();
    }

    if(flag) reset_screen();
}

void pan_left()
{
    screen_center_x -= 0.1;
    float left = screen_center_x - 4/screen_zoom;
    if(left < -40.0)
    {
        screen_center_x = -40.0 + 4/screen_zoom;
    }
}

void pan_right()
{
    screen_center_x += 0.1;
    float right = screen_center_x + 4/screen_zoom;
    if(right > 40.0)
    {
        screen_center_x = 40.0 - 4/screen_zoom;
    }
}

void pan_up()
{
    screen_center_y += 0.1;
    float up = screen_center_y + 4/screen_zoom;
    if(up > 40.0)
    {
        screen_center_y = 40.0 - 4/screen_zoom;
    }
}

void pan_down()
{
    screen_center_y -= 0.1;
    float down = screen_center_y - 4/screen_zoom;
    if(down < -40.0)
    {
        screen_center_y = -40.0 + 4/screen_zoom;
    }
}

void tick_elements() {
    magnet.tick();
    porcupines[0].tick();
    porcupines[1].tick();

    if(magnet.field)
    {
        player.magTick(magnet.flag);
    }
    else
    {
        player.magVelocity = 0.0;
    }

    player.tick();

    if(detect_water())
    {
        player.sink = true;
    }
    else
    {
        player.sink = false;
    }

    player.fallWater(pond.position.x, pond.position.y, pond.radius);
    player.rollAlongPond(pond.position.x, pond.position.y, pond.radius);

    if(!(player.speedVertical>0))
    {
        for(i=0; i<NUM_ENEMIES; i++)
        {
            if(enemies[i].alive && detect_collision(i))
            {
                if(!enemies[i].hasBlock)
                {
                    enemies[i].alive = false; //enemy is killed
                    int add = 4-enemies[i].type;
                    score += add;
                    printf("Pacman killed: +%d\n", add);
                    player.goDown = false;
                    player.speedVerticalDown = 0.0;
                    player.speedVertical = 0.165;

                    player.speedHoriz = 0.0;
                }

                else
                {
                    float ang = (-2.0)*enemies[i].rotation*M_PI/180.0;
                    float spd = player.speedVerticalDown;
                    player.goDown = false;
                    player.speedVerticalDown = 0.0;
                    player.speedVertical = spd * cos(ang);
                    player.speedHoriz = spd * sin(ang);
                }
            }
        }
    }

    if(detect_tramp())
    {
        player.goDown = false;
        player.speedVerticalDown = 0.0;
        player.speedVertical = 0.25;

        player.speedHoriz = 0.0;
    }

    for(i=0; i<NUM_ENEMIES; i++)
    {
        enemies[i].tick();
    }

    detect_porcupine(0);
    detect_porcupine(1);

    float left = screen_center_x - 4/screen_zoom;
    if(player.position.x - player.radius < left)
    {
        if(player.position.x - player.radius < -40.0) player.position.x = -40.0 + player.radius;
        screen_center_x = (player.position.x - player.radius) + 4/screen_zoom;
    }

    float right = screen_center_x + 4/screen_zoom;
    if(player.position.x + player.radius > right)
    {
        if(player.position.x + player.radius > 40.0) player.position.x = 40.0 - player.radius;
        screen_center_x = (player.position.x + player.radius) - 4/screen_zoom;
    }

    reset_screen();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    player      = Ball(0, floorHeight, COLOR_GREY);
    ground      = Ground(floorHeight-player.radius, -4.0);
    pond        = Pond(2.0,floorHeight-player.radius);
    trampoline  = Trampoline(-2.0, floorHeight + player.radius, floorHeight - player.radius);
    magnet      = Magnet(4.0,2.0);
    porcupines[0]   = Porcupine(trampoline.position.x+trampoline.radius+trampoline.blockWidth, pond.position.x-pond.radius, floorHeight-player.radius, 0.15);
    porcupines[1]   = Porcupine(-4.0, trampoline.position.x-trampoline.radius-trampoline.blockWidth, floorHeight-player.radius, 0.15);

    for(i=0; i<NUM_ENEMIES; i++)
    {
        bool hasBlock = (rand()%10 == 1);
        enemies[i] = Enemy(-1.0-(i*0.4), floorHeight + 2*player.radius + 0.6, hasBlock);
    }

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 800;
    int height = 800;

    window = initGLFW(width, height);

    initGL (window, width, height);

    int tickcount = 2*magnet.dashIn + magnet.stay;
    int gap = 1000; // Time between departure of old magnet and arrival of new

    glfwSetScrollCallback(window, scroll_callback);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            tickcount++;
            if(tickcount == gap + 2*magnet.dashIn + magnet.stay)
            {
                tickcount=0;
                magnet.start(rand()%2 + 1);
            }
            // printf("%d\n", tickcount);
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);

            char titleString[64];
            sprintf(titleString, "Score: %d", score);
            glfwSetWindowTitle(window,titleString);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

void jump()
{
    if(player.speedVertical > 0 || player.goDown) return;

    if(player.sink)
    {
        if(player.position.y <= pond.position.y)
        {
            player.speedVertical = 0.1;
        }
    }
    else player.speedVertical = player.initVelocity;
}

float abso(float x)
{
    if(x<0) x=-x;
    return x;
}

bool detect_collision(int i)
{
    Enemy enemy = enemies[i];
    if(player.speedVertical > 0) return false;
    if(abso(player.position.x - enemy.position.x) < (player.radius + enemy.radius)
        && player.position.y > enemy.position.y
        && player.position.y - enemy.position.y <= (player.radius + enemy.radius)
        ) return true;
    return false;
}

bool detect_tramp()
{
    if(player.speedVertical > 0) return false;
    if(abso(player.position.x - trampoline.position.x) < (player.radius + trampoline.radius + trampoline.blockWidth)
        &&
        (
            (player.position.y > trampoline.position.y && player.position.y - trampoline.position.y <= (player.radius))
            || player.grounded)
        ) return true;
    return false;
}

bool detect_water()
{
    if(abso(player.position.x - pond.position.x) <= (pond.radius - player.radius)
        && player.position.y <= 0.0
        ) return true;
    else if(abso(player.position.x - pond.position.x) < (pond.radius) && player.position.y==floorHeight)
    {
        if(player.position.x > pond.position.x) player.position.x = pond.position.x + pond.radius - player.radius;
        else player.position.x = pond.position.x - pond.radius + player.radius;
        return true;
    }
    else if(abso(player.position.x - pond.position.x) < (pond.radius) && player.position.y<floorHeight)
    {
        if(player.position.x > pond.position.x) player.position.x = pond.position.x + pond.radius;
        else player.position.x = pond.position.x - pond.radius;
        player.position.y = floorHeight;
        return false;
    }
    return false;
}

void detect_porcupine(int i)
{
    if(!porcupines[i].alive) return;
    if( abs(player.position.x - porcupines[i].position.x) < (player.radius + porcupines[i].R)
        && (player.position.y - porcupines[i].position.y) < (player.radius + porcupines[i].height)
    ) {
        printf("****************** Porcupine touched: -50 ******************\n");
        score-=50;
        porcupines[i].alive=false;
    }
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    if(top>40.0 || right>40.0 || left<-40.0 || bottom<-40.0) return;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
