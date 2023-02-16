#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <glm/glm.hpp>

using namespace std;
#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "cars.h"

int window_height;
int window_width;

float val = 0.0f;
float velocity = 0.0f;
float retardation = 0.1f;

QueenCar qcar;

void process_input(GLFWwindow * window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        velocity += 0.4f;
        qcar.dir_off += 0.5f;
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        qcar.x -= qcar.dir_off * sin(qcar.player_angle);
        qcar.z += qcar.dir_off * cos(qcar.player_angle);
        qcar.dir_off = 0.0f;
        qcar.player_angle += 5 * M_PI / 180;
    }
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        qcar.x -= qcar.dir_off * sin(qcar.player_angle);
        qcar.z += qcar.dir_off * cos(qcar.player_angle);
        qcar.dir_off = 0.0f;
        qcar.player_angle -= 5 * M_PI / 180;
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        velocity -= 0.1f;
        qcar.dir_off -= 0.5f;
    }
}

float t = 0;
void fb_size_cb(GLFWwindow * window, int width, int height){
    glViewport(0, 0, width, height);
    window_height = height;
    window_width = width;
}

void mouseCb(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        cout << "something\n";
    else if(action == GLFW_RELEASE){
        cout << "released\n";
        // double x_pos, y_pos;
        // glfwGetCursorPos(window, &x_pos, &y_pos);
        // cout << x_pos << " " << y_pos << endl;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    // cout << xpos << " " << ypos << endl;
}


int main(){
    srand(time(NULL));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window_height = 800;
    window_width = 800;
    GLFWwindow * window = glfwCreateWindow(window_width, window_height, "Game Again", NULL, NULL);
    if(!window){
        printf("create window error\n");
        glfwTerminate();
        return 0;
    }
    int w,h;
    glfwGetWindowSize(window, &w, &h);
    cout << w << " " << h << endl;
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseCb);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    // glfwSetFramebufferSizeCallback(window, fb_size_cb);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("glad initialization error\n");
        return 0;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cout << "hi\n";
    Shader shader("mesh.vs", "mesh.fs");
    prog_id = shader.prog_id;
    Model stadium("/Users/keshavgupta/desktop/CarRace/stadium.glb", shader);
    Model car("/Users/keshavgupta/desktop/CarRace/mcqueen/scene.gltf", shader);

    // GLfloat modelt[] = {1.0f, 0.0f, 0.0f, 0.0f,
    //                             0.0f, 0.0f, 1.0f, 0.0f,
    //                             0.0f, -1.0f, 0.0f, 0.0f,
    //                             0.0f, 0.0f, 0.0f, 1.0f};

    GLfloat modelt[] = {1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f};

    OpponentCar opcar(&m2, modelt, 40.0f, 50.0f, -40.0f);
    OpponentCar opcar2(&m2, modelt, 40.0f, 80.0f, -40.0f);
    OpponentCar opcar3(&m2, modelt, 40.0f, 70.0f, -40.0f);

    cout << "minmax : " << m2.max_x << " " << m2.min_x << endl;
    cout << "minmax : " << m2.max_y << " " << m2.min_y << endl;
    cout << "minmax : " << m2.max_z << " " << m2.min_z << endl;

    qcar.init(&m2, modelt, 45.0f, -40.0f, 0.0f);


    while(!glfwWindowShouldClose(window)){
        glViewport(0, 0, window_width * 2, window_height * 2);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(velocity > 0){
            qcar.dir_off += velocity;
            velocity -= retardation;
        }

        if(velocity < 0){
            velocity = 0.0f;
        }

        GLfloat model2[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, -1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat model[] = {50.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 50.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 50.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat transform[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f};

        float cam_x = qcar.x - (qcar.dir_off - 8.0) * sin(qcar.player_angle);
        float cam_z = qcar.z + (qcar.dir_off - 8.0) * cos(qcar.player_angle);
        float cam_y = -40.0f + 5.5f;

        GLfloat view[] = {cos(qcar.player_angle), 0.0f, sin(qcar.player_angle), -cos(qcar.player_angle) * cam_x - sin(qcar.player_angle) * cam_z,
                                0.0f, 1.0f, 0.0f, -cam_y,
                                -sin(qcar.player_angle), 0.0f, cos(qcar.player_angle), -(-sin(qcar.player_angle) * cam_x) - cos(qcar.player_angle) * cam_z,
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat projection[] = {1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f};

        stadium.Display(model, transform, view, projection);

        qcar.Display(view, projection);
        opcar.Display(view, projection);
        opcar2.Display(view, projection);
        opcar3.Display(view, projection);




        glViewport(0, 0, 300, 300);

        cam_x = qcar.x; // need base_x though
        cam_y = qcar.y + 30;
        cam_z = qcar.z;

        GLfloat projection2[] = {1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f};

        GLfloat view2[] = {sin(qcar.player_angle), cos(qcar.player_angle), 0.0f, -cam_x * cos(qcar.player_angle) - cam_y * (-sin(qcar.player_angle)),
                        -cos(qcar.player_angle), sin(qcar.player_angle), 0.0f, -sin(qcar.player_angle) * cam_x - cam_y * cos(qcar.player_angle),
                        0.0f, -1.0f, 0.0f, cam_y,
                        0.0f, 0.0f, 0.0f, 1.0f};

        stadium.Display(model, transform, view2, projection2);
        qcar.Display(view2, projection2);
        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window);
    }
}