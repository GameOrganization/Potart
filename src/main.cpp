#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "Window.h"
#include "Physics.h"
#include "Character.h"
#include "Entity.h"

//change to 1 to enable vsync
#define VSYNC 1

void init();
void update(float time);
void draw();

b2World world(b2Vec2(0.0f, -9.8f));

//user controlled blocks
Character testRed = Character("testRed", Vec2f(-1.0f, 2.5f), NULL, OTHER);
Character testGreen = Character("testGreen", Vec2f(0.0f, 2.5f), NULL, OTHER);
Character testBlue = Character("testBlue", Vec2f(1.0f, 2.5f), NULL, OTHER);

Character activeCharacter = testGreen;

b2Body* ground = Physics::createStaticBox(0.0f, 1.0f, 10.0f, 2.0f, world);

//save button on the LEFT
b2Body* saveButton = Physics::createStaticBox(-3.0f, 1.2f, 1.5f, 0.5f, world);

//load button on the RIGHT
b2Body* loadButton = Physics::createStaticBox(3.0f, 1.2f, 1.5f, 0.5f, world);

//reset button in the CENTER
b2Body* resetButton = Physics::createStaticBox(0.0f, 1.2f, 1.5f, 0.5f, world);
b2Body* curr = NULL;

int control = 1;

//used for writing to a save file
std::ofstream outfile;
std::stringstream out;

float w = 0.0f, h = 0.0f;

//Callback function for handling key events
static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

    //close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    //move left
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        if(control==0){
            testRed.moveLeft();
        }else if(control==1){
            testGreen.moveLeft();
        }else{
            testBlue.moveLeft();
        }
    }

    //move right
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        if(control==0){
            testRed.moveRight();
        }else if(control==1){
            testGreen.moveRight();
        }else{
            testBlue.moveRight();
        }
    }

    //stop moving
    if ((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_RELEASE){
        if(control==0){
            testRed.stop();
        }else if(control==1){
            testGreen.stop();
        }else{
            testBlue.stop();
        }
    }

    //jump
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        if(control==0){
            testRed.jump();
        }else if(control==1){
            testGreen.jump();
        }else{
            testBlue.jump();
        }
    }

    //cycle which block you're controlling
    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        control--;}
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        control++;}
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
        std::cout << "Boxes (including test boxes): " << world.GetBodyCount()-4 << std::endl;

        for(b2Body *b = world.GetBodyList(); b != NULL; b = b->GetNext()){
            //only save things above the ground
            if(b->GetPosition().y > 2 && b != testRed.getBody() && b != testGreen.getBody() && b!=testBlue.getBody()){
                out << b->GetPosition().x << "," << b->GetPosition().y << std::endl;
            }else if(b == testRed.getBody()){
                out << "R" << b->GetPosition().x << "," << b->GetPosition().y << std::endl;
            }else if(b == testGreen.getBody()){
                out << "G" << b->GetPosition().x << "," << b->GetPosition().y << std::endl;
            }else if(b == testBlue.getBody()){
                out << "B" << b->GetPosition().x << "," << b->GetPosition().y << std::endl;
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
            if(b->GetPosition().y > 2.0){
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
        int loadTestRed = 0;
        int loadTestGreen = 0;
        int loadTestBlue = 0;

        //checking that the file exists and loading it
        std::ifstream infile("saveFile.txt");
        std::cout << "===LOADING==="<< std::endl;
        if(infile.is_open()){
            while(getline(infile,line)){
                std::stringstream in(line);

                //assigning each individual x and y
                while(std::getline(in,token,',')){
                    if(token.substr(0, 1)=="R"){
                        loadTestRed = 1;
                        token = token.substr(1,token.length());
                    }
                    else if(token.substr(0, 1)=="G"){
                        loadTestGreen = 1;
                        token = token.substr(1,token.length());
                    }
                    else if(token.substr(0, 1)=="B"){
                        loadTestBlue = 1;
                        token = token.substr(1,token.length());
                    }
                    if(isX==1){
                        x=atof(token.c_str());
                        isX=0;
                    }else{
                        y=atof(token.c_str());
                        isX=1;
                    }
                }

                //loading red
                if(loadTestRed==1){
                    std::cout << "Loading RED at " << x << ", " << y << std::endl;
                    testRed.reset(x, y, 0.5f, 1.0f, 1.0f, 0.0f, world);
                    loadTestRed = 0;

                //loading green
                }else if(loadTestGreen==1){
                    std::cout << "Loading GREEN at " << x << ", " << y << std::endl;
                    testGreen.reset(x, y, 0.5f, 1.0f, 1.0f, 0.0f, world);
                    loadTestGreen = 0;

                //loading blue
                }else if(loadTestBlue==1){
                    std::cout << "Loading BLUE at " << x << ", " << y << std::endl;
                    testBlue.reset(x, y, 0.5f, 1.0f, 1.0f, 0.0f, world);
                    loadTestBlue=0;

                //loading everything else
                }else{
                    std::cout << "Spawning at " << x << ", " << y << std::endl;
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

        //reset red
        testRed.reset(-1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);

        //reset green
        testGreen.reset(0.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);

        //reset blue
        testBlue.reset(1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);

    //spawn boxes
    }else if (button == 0 && action == GLFW_PRESS) {
        std::cout << "spawning at " << x << ", " << y << std::endl;
        curr = Physics::createDynamicBox(x, y, 0.5f, 0.5f, 1.0f, 1.0f, world);
    }
}

int main(int argc, char *argv[]) {

    testRed.reset(-1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
    testGreen.reset(0.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
    testBlue.reset(1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);

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

    if(control>2){
        control = 0;
    }else if(control<0){
        control = 2;
    }else if(control == 0){
        activeCharacter = testRed;
    }else if(control == 1){
        activeCharacter = testGreen;
    }else if(control == 2){
        activeCharacter = testBlue;
    }

    if(testRed.getBody()->GetWorldCenter().y<-1.0f||testRed.getBody()->GetWorldCenter().x<-w/2||testRed.getBody()->GetWorldCenter().x > w/2){
        std::cout << "RED left the screen: resetting" << std::endl;
        world.DestroyBody(testRed.getBody());
        testRed.reset(-1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
    }

    if(testGreen.getBody()->GetWorldCenter().y<-1.0f||testGreen.getBody()->GetWorldCenter().x<-w/2||testGreen.getBody()->GetWorldCenter().x > w/2){
        std::cout << "GREEN left the screen: resetting" << std::endl;
        world.DestroyBody(testGreen.getBody());
        testGreen.reset(0.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
    }

    if(testBlue.getBody()->GetWorldCenter().y<-1.0f||testBlue.getBody()->GetWorldCenter().x<-w/2||testBlue.getBody()->GetWorldCenter().x > w/2){
        std::cout << "BLUE left the screen: resetting" << std::endl;
        world.DestroyBody(testBlue.getBody());
        testBlue.reset(1.0f, 2.5f, 0.5f, 1.0f, 1.0f, 0.0f, world);
    }

    if(activeCharacter.getBody()->GetLinearVelocity().x<0){
        glTranslatef(0.007,0,0);
    }

    if(activeCharacter.getBody()->GetLinearVelocity().x>0){
        glTranslatef(-0.007,0,0);
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
    glColor3f(1.0f, 0.6f, 0.6f);
    drawBody(testRed.getBody());
    glColor3f(0.6f, 0.6f, 1.0f);
    drawBody(testGreen.getBody());
    glColor3f(0.6f, 1.0f, 0.6f);
    drawBody(testBlue.getBody());
    glColor3f(0.5f, 0.8f, 1.0f);
    drawBody(saveButton);
    drawBody(resetButton);
    drawBody(loadButton);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext())
        if (body!=ground&&body!=testRed.getBody()&&body!=testGreen.getBody()&&body!=testBlue.getBody()&&body!=saveButton&&body!=resetButton&&body!=loadButton)
            drawBody(body);
}
