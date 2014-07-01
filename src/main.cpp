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

//user controlled block
b2Body* test = Physics::createDynamicBox(-1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);

b2Body* other = Physics::createDynamicBox(1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);

b2Body* ground = Physics::createStaticBox(0.0f, 1.0f, 10.0f, 2.0f, world);

//save button on the LEFT
b2Body* saveButton = Physics::createStaticBox(-3.0f, 1.2f, 1.5f, 0.5f, world);

//load button on the RIGHT
b2Body* loadButton = Physics::createStaticBox(3.0f, 1.2f, 1.5f, 0.5f, world);

//reset button in the CENTER
b2Body* resetButton = Physics::createStaticBox(0.0f, 1.2f, 1.5f, 0.5f, world);
b2Body* curr = NULL;

//used for writing to a save file
std::ofstream outfile;
std::stringstream out;

float w = 0.0f, h = 0.0f;

//Callback function for handling key events
static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

    //close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    //TEST move left
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        test->SetLinearVelocity(b2Vec2(-5,test->GetLinearVelocity().y));

    //TEST move right
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        test->SetLinearVelocity(b2Vec2(5,test->GetLinearVelocity().y));

    //TEST stop left
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        test->SetLinearVelocity(b2Vec2(0,test->GetLinearVelocity().y));

    //TEST stop right
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        test->SetLinearVelocity(b2Vec2(0,test->GetLinearVelocity().y));

    //TEST stop horizontal movement, maintain vertical
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        test->SetLinearVelocity(b2Vec2(0,test->GetLinearVelocity().y));

    //TEST jump
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        if(test->GetLinearVelocity().y < 0.1 && test->GetLinearVelocity().y > -0.1){
            test->SetLinearVelocity(b2Vec2(test->GetLinearVelocity().x,5));
        }

    //OTHER move left
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        other->SetLinearVelocity(b2Vec2(-5,other->GetLinearVelocity().y));

    //OTHER move right
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        other->SetLinearVelocity(b2Vec2(5,other->GetLinearVelocity().y));

    //OTHER stop left
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        other->SetLinearVelocity(b2Vec2(0,other->GetLinearVelocity().y));

    //OTHER stop right
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        other->SetLinearVelocity(b2Vec2(0,other->GetLinearVelocity().y));

    //OTHER stop horizontal movement, maintain vertical
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        other->SetLinearVelocity(b2Vec2(0,other->GetLinearVelocity().y));

    //OTHER jump
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        if(other->GetLinearVelocity().y < 0.1 && other->GetLinearVelocity().y > -0.1){
            other->SetLinearVelocity(b2Vec2(other->GetLinearVelocity().x,5));
        }
}

static void mouseHandler(GLFWwindow* window, int button, int action, int mods) {
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    float x = (float)(mx - (Window::width()/2));
    float y = (float)(Window::height() - my);
    x *= w / Window::width();
    y *= h / Window::height();

    //checking if the user clicked the save button, saving current positions if so
    if(button == 0 && action == GLFW_PRESS && x >= -3.75 && x <= -2.25 && y >= .95 && y <= 1.45){
        out.str("");
        std::cout << "===SAVING==="<< std::endl;

        //number of boxes, excluding the ground and buttons
        std::cout << "Boxes: " << world.GetBodyCount()-4 << std::endl;

        for(b2Body *b = world.GetBodyList(); b != NULL; b = b->GetNext()){
            //only save things above the ground
            if(b->GetPosition().y > 2 && b->GetUserData() != test && b->GetUserData() != other){
                out << b->GetPosition().x << "," << b->GetPosition().y << std::endl;
            }else if(b->GetUserData() == test){
                out << "TEST" << b->GetPosition().x << "," << b->GetPosition().y << std::endl;
            }else if(b->GetUserData() == other){
                out << "OTHER" << b->GetPosition().x << "," << b->GetPosition().y << std::endl;
            }
        }
        outfile.open("saveFile.txt");
        outfile << out.str();
        outfile.close();
        out.str("");

    //checking if the user clicked the load button, loading if so
    }else if(button == 0 && action == GLFW_PRESS && x <= 3.75 && x >= 2.25 && y >= .95 && y <= 1.45){

        //remove old blocks above the ground
        for(b2Body *b = world.GetBodyList(); b != NULL; b = b->GetNext()){
            if(b->GetPosition().y > 2.0 && b != test && b != other){
                world.DestroyBody(b);
            }
        }

        //each line in the file
        std::string line;

        //the individual coordinates
        std::string token;

        //i don't know how boolean works in c++
        int isX = 1;

        //still don't know how boolean works
        int isTest = 0;
        int isOther = 0;

        //checking that the file exists and loading it
        std::ifstream infile("saveFile.txt");
        std::cout << "===LOADING==="<< std::endl;
        if(infile.is_open()){
            while(getline(infile,line)){
                std::stringstream in(line);

                //assigning each individual x and y
                while(std::getline(in,token,',')){
                    if(token.substr(0, 4)=="TEST"){
                        isTest = 1;
                        token = token.substr(4,token.length());
                    }
                    if(token.substr(0, 5)=="OTHER"){
                        isOther = 1;
                        token = token.substr(5,token.length());
                    }
                    if(isX==1){
                        x=atof(token.c_str());
                        isX=0;
                    }else{
                        y=atof(token.c_str());
                        isX=1;
                    }
                }

                //loading test
                if(isTest==1){
                    std::cout << "spawning test at " << x << ", " << y << std::endl;
                    world.DestroyBody(test);
                    test = Physics::createDynamicBox(x, y, 0.5f, 1.0f, 1.0f, 0.0f, world);
                    test -> SetFixedRotation(true);
                    test -> SetUserData(test);
                    isTest = 0;

                //loading other
                }else if(isOther==1){
                    std::cout << "spawning other at " << x << ", " << y << std::endl;
                    world.DestroyBody(other);
                    other = Physics::createDynamicBox(x, y, 0.5f, 1.0f, 1.0f, 0.0f, world);
                    other -> SetFixedRotation(true);
                    other -> SetUserData(other);
                    isOther = 0;

                //loading the boxes
                }else{
                    std::cout << "spawning at " << x << ", " << y << std::endl;
                    curr = Physics::createDynamicBox(x, y, 0.5f, 0.5f, 1.0f, 1.0f, world);
                    out << x << "," << y << std::endl;
                }
            }
            infile.close();
        }

    //removing all blocks to reset
    }else if(button == 0 && action == GLFW_PRESS && x <= 0.75 && x >= -0.75 && y >= .95 && y <= 1.45){
        std::cout<<"===RESETTING==="<<std::endl;
        for(b2Body *b = world.GetBodyList(); b != NULL; b = b->GetNext()){
            if(b->GetPosition().y > 2.0){
                world.DestroyBody(b);
            }
        }

        //reset test
        test = Physics::createDynamicBox(-1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
        test -> SetFixedRotation(true);
        test -> SetUserData(test);

        //reset other
        other = Physics::createDynamicBox(1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
        other -> SetFixedRotation(true);
        other -> SetUserData(other);

    //spawn boxes
    }else if (button == 0 && action == GLFW_PRESS) {
        std::cout << "spawning at " << x << ", " << y << std::endl;
        curr = Physics::createDynamicBox(x, y, 0.5f, 0.5f, 1.0f, 1.0f, world);
    }
}

int main(int argc, char *argv[]) {

    test->SetFixedRotation(true);
    test->SetUserData(test);

    other->SetFixedRotation(true);
    other->SetUserData(other);

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
    //just noticed this and laughed in the office i dont care i love it
    glfwSwapInterval(VSYNC);

    w = 10.0f * (float)Window::width() / (float)Window::height();
    h = 10.0f;

    glViewport(0, 0, Window::width(), Window::height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2, w/2, 0.0f, h, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update(float time) {
    world.Step(time, 8, 3);

    //if the test leaves the bottom or sides of the screen, respawn
    if(test->GetWorldCenter().y < -1.0f || test->GetWorldCenter().x < -w/2 || test->GetWorldCenter().x > w/2){
        std::cout << "Test left the screen: resetting" << std::endl;
        world.DestroyBody(test);
        test = Physics::createDynamicBox(0.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
        test -> SetFixedRotation(true);
        test -> SetUserData(test);
    }

    //if the other leaves the bottom or sides of the screen, respawn
    if(other->GetWorldCenter().y < -1.0f || other->GetWorldCenter().x < -w/2 || other->GetWorldCenter().x > w/2){
        std::cout << "Other left the screen: resetting" << std::endl;
        world.DestroyBody(other);
        other = Physics::createDynamicBox(0.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
        other -> SetFixedRotation(true);
        other -> SetUserData(other);
    }

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
    glColor3f(1.0f, 0.8f, 0.6f);
    drawBody(test);
    glColor3f(0.6f, 0.8f, 1.0f);
    drawBody(other);
    glColor3f(0.5f, 0.8f, 1.0f);
    drawBody(saveButton);
    drawBody(resetButton);
    drawBody(loadButton);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext())
        if (body!=ground&&body!=test&&body!=other&&body!=saveButton&&body!=resetButton&&body!=loadButton)
            drawBody(body);
}
