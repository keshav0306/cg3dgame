#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <tuple>
#include <glm/glm.hpp>

using namespace std;
#include "bs.h"
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

int collision_check(BoundingSpheres bs1, BoundingSpheres bs2, vector<vector<float> > vv1, vector<vector<float> > vv2){

    float r1 = bs1.radius;
    float r2 = bs2.radius;
    int flag = 0;
    for(int i=0;i<bs1.n;i++){
        for(int j=0;j<bs2.n;j++){
            float v1x = vv1[i][0];
            float v1y = vv1[i][1];
            float v1z = vv1[i][2];

            float v2x = vv2[j][0];
            float v2y = vv2[j][1];;
            float v2z = vv2[j][2];;
            float dist = sqrt((v1x - v2x) * (v1x - v2x) + (v1y - v2y) * (v1y - v2y) + (v1z - v2z) * (v1z - v2z));
            // cout << "first one : " << v1x << " " << v1y << " " << v1z << endl;
            // cout << "second one : " << v2x << " " << v2y << " " << v2z << endl;

            // cout << "dist ";
            // cout << dist << "\n";
            // cout << "sum of radius : ";
            // cout << r1 + r2 << "\n";
            if(dist < r1 + r2){
                cout << "collision!\n";
                cout << i << " " << j << endl;
                flag = 1;
            }
        }
    }
    if(flag){
        return 1;
    }
    return 0;
}

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

    stbi_set_flip_vertically_on_load(true);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cout << "hi\n";
    Shader shader("mesh.vs", "mesh.fs");
    Model stadium("/Users/keshavgupta/desktop/CarRace/stadium.glb", shader);
    Model car("/Users/keshavgupta/desktop/CarRace/mcqueen/scene.gltf", shader);

    GLfloat modelt[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, -1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f};

    // GLfloat modelt[] = {1.0f, 0.0f, 0.0f, 0.0f,
    //                         0.0f, 1.0, 0.0f, 0.0f,
    //                         0.0f, 0.0f, 1.0f, 0.0f,
    //                         0.0f, 0.0f, 0.0f, 1.0f};

    OpponentCar opcar(&car, modelt, 40.0f, 50.0f, -40.0f);
    // OpponentCar opcar2(&car, modelt, 40.0f, 80.0f, -40.0f);
    OpponentCar opcar3(&car, modelt, 40.0f, 70.0f, -40.0f);

    cout << "minmax : " << car.max_x << " " << car.min_x << endl;
    cout << "minmax : " << car.max_y << " " << car.min_y << endl;
    cout << "minmax : " << car.max_z << " " << car.min_z << endl;

    qcar.init(&car, modelt, 45.0f, -40.0f, 0.0f);

    while(!glfwWindowShouldClose(window)){
        glViewport(0, 0, window_width * 2, window_height * 2);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // opcar.get_the_bs();
        // qcar.get_the_bs();
        // if(collision_check(opcar.bs, opcar.bs, opcar.get_the_bs(), qcar.get_the_bs())){
        //     velocity = 0.0f;
        // }
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
        // opcar2.Display(view, projection);
        opcar3.Display(view, projection);




        glViewport(0, 0, 300, 300);

        cam_x = qcar.x - qcar.dir_off * sin(qcar.player_angle); // need base_x though
        cam_y = qcar.y + 30.0f;
        cam_z = qcar.z + qcar.dir_off * cos(qcar.player_angle);

        GLfloat projection2[] = {1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f};

        GLfloat view2[] = {1.0f, 0.0f, 0.0f, -cam_x,
                        0.0f, 0.0f, 1.0f, -cam_z,
                        0.0f, -1.0f, 0.0f, cam_y,
                        0.0f, 0.0f, 0.0f, 1.0f};

        // GLfloat view2[] = {1.0f, 0.0f, 0.0f, -cam_x,
        //                 0.0f, 0.0f, 1.0f, -cam_z * cos(qcar.player_angle),
        //                 0.0f, -1.0f, 0.0f, cam_y,
        //                 0.0f, 0.0f, 0.0f, 1.0f};

        stadium.Display(model, transform, view2, projection2);
        qcar.Display(view2, projection2);
        opcar.Display(view2, projection2);
        // opcar2.Display(view2, projection2);
        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window);
    }
}