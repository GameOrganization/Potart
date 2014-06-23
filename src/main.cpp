#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "Window.h"
#include "Physics.h"

//change to 1 to enable vsync
#define VSYNC 1

void init();
void update(float time);
void draw();

b2World world(b2Vec2(0.0f, -9.8f));
b2Body* ground = Physics::createStaticBox(0.0f, 1.0f, 10.0f, 2.0f, world);

//save button on the LEFT
b2Body* saveButton = Physics::createStaticBox(-3.0f, 1.2f, 1.5f, 0.5f, world);

//load button on the RIGHT
b2Body* loadButton = Physics::createStaticBox(3.0f, 1.2f, 1.5f, 0.5f, world);
b2Body* curr = NULL;

//used for writing to a save file
std::ofstream outfile;
std::stringstream out;

float w = 0.0f, h = 0.0f;

//Callback function for handling key events
static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void mouseHandler(GLFWwindow* window, int button, int action, int mods) {
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    float x = (float)(mx - (Window::width()/2));
    float y = (float)(Window::height() - my);
    x *= w / Window::width();
    y *= h / Window::height();

    //checking if the user clicked the save button, saving current positions if so
    if(button == 0 && action == GLFW_PRESS && x > -3.75 && x < -2.25 && y > .95 && y < 1.45){
        std::cout << "===SAVING==="<< std::endl;
        std::cout << "Body Count: " << world.GetBodyCount() << std::endl;
        for(b2Body *b = world.GetBodyList(); b != NULL; b = b->GetNext()){
            //only save things above the ground
            if(b->GetPosition().y > 2.0){
                out << b->GetPosition().x << "," << b->GetPosition().y << std::endl;
            }
        }
        outfile.open("saveFile.txt");
        outfile << out.str();
        outfile.close();

    //checking if the user clicked the load button, loading if so
    }else if(button == 0 && action == GLFW_PRESS && x < 3.75 && x > 2.25 && y > .95 && y < 1.45){

        //remove old blocks above the ground
        for(b2Body *b = world.GetBodyList(); b != NULL; b = b->GetNext()){
            if(b->GetPosition().y > 2.0){
                world.DestroyBody(b);
            }
        }

        //each line in the file
        std::string line;

        //the individual coordinates
        std::string token;

        //i don't know how boolean works in c++
        int isX =1;

        //checking that the file exists and loading it
        std::ifstream infile("saveFile.txt");
        std::cout << "===LOADING==="<< std::endl;
        if(infile.is_open()){
            while(getline(infile,line)){
                std::stringstream in(line);

                //assigning each individual x and y
                while(std::getline(in,token,',')){
                    if(isX==1){
                        x=atof(token.c_str());
                        isX=0;
                    }else{
                        y=atof(token.c_str());
                        isX=1;
                    }
                }

                //spawning the boxes
                std::cout << "spawning at " << x << ", " << y << std::endl;
                curr = Physics::createDynamicBox(x, y, 0.5f, 0.5f, 1.0f, world);
                out << x << "," << y << std::endl;
            }
            infile.close();
        }
    }else if (button == 0 && action == GLFW_PRESS) {
        std::cout << "spawning at " << x << ", " << y << std::endl;
        curr = Physics::createDynamicBox(x, y, 0.5f, 0.5f, 1.0f, world);
    }
}

int main(int argc, char *argv[]) {
    //Attempt to create window
    if (Window::create()) {
        Window::destroy();
        return 1;
    }

    //Initialize world
    init();

    //Loop updates world, draws it, and then shows it on the screen
    while (Window::isOpen()) {
        update((float)Window::getFrameTime());
        draw();
        Window::update();
    }

    //Close the window
    Window::destroy();
    return 0;
}

void init() {
    //Set the key handler callback
    glfwSetKeyCallback(Window::ptr, keyHandler);
    glfwSetMouseButtonCallback(Window::ptr, mouseHandler);

    //Enable vsync so mike's computer doesn't shit itself
    glfwSwapInterval(VSYNC);

    w = 10.0f * (float)Window::width() / (float)Window::height();
    h = 10.0f;

    glViewport(0, 0, Window::width(), Window::height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float x = 5.0f * (float)Window::width() / (float)Window::height();
    glOrtho(-w/2, w/2, 0.0f, h, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update(float time) {
    world.Step(time, 8, 3);
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        if (body->GetWorldCenter().y < -10.0f) {
            b2Body* toDestroy = body;
            body = body->GetNext();
            world.DestroyBody(toDestroy);
            std::cout << "Destroying body" << std::endl;
            if (!body) break;
        }

    }
}

void drawBody(b2Body* body) {
    b2Vec2 center = body->GetWorldCenter();
    float angle = 180.0f / M_PI * body->GetAngle();
    glTranslatef(center.x, center.y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    b2Vec2* vertices = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->m_vertices;
    for (int i = 0; i < 4; i++)
        glVertex2f(vertices[i].x, vertices[i].y);
    glEnd();
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-center.x, -center.y, 0.0f);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5f, 0.5f, 0.5f);
    drawBody(ground);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext())
        if (body != ground)
            drawBody(body);
}
