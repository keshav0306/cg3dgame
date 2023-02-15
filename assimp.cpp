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

int window_height;
int window_width;
int prog_id;

float player_x;
float player_z;
float player_angle;
float val = 0.0f;
float dir_off;
float velocity = 0.0f;
float retardation = 0.1f;

void process_input(GLFWwindow * window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        velocity += 0.4f;
        dir_off+=0.5f;
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        player_x -= dir_off * sin(player_angle);
        player_z += dir_off * cos(player_angle);
        dir_off = 0.0f;
        player_angle += 5 * M_PI / 180;
    }
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        player_x -= dir_off * sin(player_angle);
        player_z += dir_off * cos(player_angle);
        dir_off = 0.0f;
        player_angle -= 5 * M_PI / 180;
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        velocity -= 0.1f;
        dir_off-=0.5f;
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
    // glViewport(0, 0, window_width * 2, window_height * 2);
    cout << "hi\n";
    Shader shader("mesh.vs", "mesh.fs");
    prog_id = shader.prog_id;
    Model m("/Users/keshavgupta/desktop/CarRace/stadium.glb", shader);
    cout << "after model 1 --------------------------------------------------\n";
    Model m2("/Users/keshavgupta/desktop/CarRace/mcqueen/scene.gltf", shader);
    player_x = 5.0f;
    player_z = 10.0f;
    dir_off = 0.0f;
    player_angle = 0.0f;

    while(!glfwWindowShouldClose(window)){
        glViewport(0, 0, window_width * 2, window_height * 2);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        if(velocity > 0){
            dir_off += velocity;
            velocity -= retardation;
        }

        if(velocity < 0){
            velocity = 0.0f;
        }

        GLfloat model2[] = {-1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat transform2[] = {cos(player_angle), 0.0f, -sin(player_angle), player_x - dir_off * sin(player_angle),
                                0.0f, 1.0f, 0.0f, -40.0f,
                                sin(player_angle), 0.0f, cos(player_angle), player_z + dir_off * cos(player_angle),
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat model[] = {50.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 50.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 50.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat transform[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat transform3[] = {cos(player_angle), 0.0f, -sin(player_angle), player_x - dir_off * sin(player_angle),
                                0.0f, 1.0f, 0.0f, -37.0f,
                                sin(player_angle), 0.0f, cos(player_angle), player_z + dir_off * cos(player_angle),
                                0.0f, 0.0f, 0.0f, 1.0f};

        // GLfloat view[] = {cos(glfwGetTime()), 0.0f, -sin(glfwGetTime()), -5.0f,
        //                 0.0f, 1.0, 0.0f, 0.0f,
        //                 sin(glfwGetTime()), 0.0f, cos(glfwGetTime()), -6.0f,
        //                 0.0f, 0.0f, 0.0f, 1.0f};

        float cam_x = player_x - (dir_off - 8.0) * sin(player_angle);
        float cam_z = player_z + (dir_off - 8.0) * cos(player_angle);
        float cam_y = -40.0f + 2.5f;

        GLfloat view[] = {cos(player_angle), 0.0f, sin(player_angle), -cos(player_angle) * cam_x - sin(player_angle) * cam_z,
                                0.0f, 1.0f, 0.0f, -cam_y,
                                -sin(player_angle), 0.0f, cos(player_angle), -(-sin(player_angle) * cam_x) - cos(player_angle) * cam_z,
                                0.0f, 0.0f, 0.0f, 1.0f};

// GLfloat view[] = {1.0f, 0.0f, 0.0f, -5.0f,
//                         0.0f, 1.0, 0.0f, 0.0f,
//                         0.0f, 0.0f, 1.0f, -6.0f,
//                         0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat view2[] = {1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0, 0.0f, 10.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f};

        m.Display(model, transform, view);
        // m2.Display(model2, transform3, view);
        m2.Display(model2, transform2, view);
        glViewport(0, 0, 300, 300);

        m.Display(model, transform, view2);
        m2.Display(model2, transform2, view2);
        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window);
    }
}