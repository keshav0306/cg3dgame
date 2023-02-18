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
#include <pthread.h>
#include <unistd.h>
#include <tuple>
#include <glm/glm.hpp>

using namespace std;
#include "bs.h"
#include "shader.h"
#include "object.h"
#include "textobject.h"
#include "mesh.h"
#include "model.h"
#include "cars.h"

int window_height;
int window_width;


float velocity = 0.0f;
float retardation = 0.1f;
float car_scale = 10.0;
float inner_radius = 1400;
float outer_radius = 2100;
float ycoord = -210;
float fuel_x;
float fuel_y;
float fuel_z;
float fuel_angle;
bool loaded = false;
int third_person = 1;
float cam_queen_dist;


Model stadium;
Model car;
Model fuel_tank;
QueenCar qcar;

int collision_check(BoundingSpheres bs1, BoundingSpheres bs2, vector<vector<float> > vv1, vector<vector<float> > vv2){
    // cout << "----\n";
    float r1 = bs1.radius;
    float r2 = bs2.radius;
    int flag = 0;
    for(int i=0;i<bs1.n;i++){
        for(int j=0;j<bs2.n;j++){
            float v1x = vv1[i][0];
            float v1y = vv1[i][1];
            float v1z = vv1[i][2];

            float v2x = vv2[j][0];
            float v2y = vv2[j][1];
            float v2z = vv2[j][2];
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

int fuel_collision(BoundingSpheres bs, vector<vector<float>> vv1){
    float r = bs.radius;
    for(int i=0;i<bs.n;i++){
        float v1x = vv1[i][0];
        float v1y = vv1[i][1];
        float v1z = vv1[i][2];
        
        float v2x = fuel_x;
        float v2y = fuel_y;
        float v2z = fuel_z;
        
        float dist = sqrt((v1x - v2x) * (v1x - v2x) + (v1y - v2y) * (v1y - v2y) + (v1z - v2z) * (v1z - v2z));

        if(dist < r){
            return 1;
        }
    }
    return 0;
}

void process_input(GLFWwindow * window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        velocity += 0.15f;
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
    else if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        third_person ^= 1;
        cam_queen_dist = ((car_scale - 1) * 8) * third_person + 8;
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


void show_entry_scene(GLFWwindow * window, TextObject to){
    while(!glfwWindowShouldClose(window)){
        glViewport(0, 0, window_width * 2, window_height * 2);
        glClearColor(0.0f, 0.0f, 0.0f, 5.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        to.RenderText("Welcome to the Game!!", -0.8, 0.8, 0.002);
        to.RenderText("Press UP To Move Forward", -0.8, 0.5, 0.002);
        to.RenderText("Press DOWN To Stop", -0.8, 0.3, 0.002);
        to.RenderText("Press RIGHT To Rotate Right", -0.8, 0.1, 0.002);
        to.RenderText("Press LEFT To Rotate Left", -0.8, -0.1, 0.002);
        to.RenderText("Press ENTER To Start!!", -0.8, -0.8, 0.002);
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
            break;
        }
    }
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    Shader shader("mesh.vs", "mesh.fs");
    stadium.init("/Users/keshavgupta/desktop/CarRace/combo.obj", shader);
    car.init("/Users/keshavgupta/desktop/CarRace/mcqueen/scene.gltf", shader);
    fuel_tank.init("/Users/keshavgupta/desktop/CarRace/fuel_tank/scene.gltf", shader);
    TextObject to("fonts/COMIC.TTF");
    to.attatch_shader("text_vs.vs", "text_fs.fs");


    show_entry_scene(window, to);

    float center_x = (car.min_x + car.max_x) / 2;
    float center_y = (car.min_y + car.max_y) / 2;
    float center_z = (car.min_z + car.max_z) / 2;

    GLfloat modelt[] = {car_scale, 0.0f, 0.0f, -center_x * car_scale,
                        0.0f, 0.0f, car_scale, -center_z * car_scale,
                        0.0f, -car_scale, 0.0f, center_y * car_scale,
                        0.0f, 0.0f, 0.0f, 1.0f};

    OpponentCar opcar(&car, modelt, 1370.0f, 1680.0f, ycoord, car_scale);
    OpponentCar opcar2(&car, modelt, 1680.0f, 1890.0f, ycoord, car_scale);
    OpponentCar opcar3(&car, modelt, 1890.0f, 2000.0f, ycoord, car_scale);
    qcar.init(&car, modelt, 1605.0f, ycoord, 0.0f, car_scale);

    cam_queen_dist = car_scale * 8;
    fuel_x = 1700 * cos(120 * M_PI / 180);
    fuel_y = ycoord;
    fuel_z = 1700 * sin(120 * M_PI / 180);
    fuel_angle = 0;

    while(!glfwWindowShouldClose(window)){
        glViewport(0, 0, window_width * 2, window_height * 2);
        glClearColor(0.0f, 0.0f, 0.0f, 5.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(collision_check(opcar.bs, opcar.bs, opcar.get_the_bs(), qcar.get_the_bs())){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            to.RenderText("Collision!!", -0.8, 0.0, 0.004);
            glDisable(GL_BLEND);
            cout << "collision\n";
            velocity = 0.0f;
        }
        if(collision_check(opcar2.bs, opcar2.bs, opcar2.get_the_bs(), qcar.get_the_bs())){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            to.RenderText("Collision!!", -0.8, 0.0, 0.004);
            glDisable(GL_BLEND);
            cout << "collision\n";
            velocity = 0.0f;
        }
        if(collision_check(opcar3.bs, opcar3.bs, opcar3.get_the_bs(), qcar.get_the_bs())){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            to.RenderText("Collision!!", -0.8, 0.0, 0.004);
            glDisable(GL_BLEND);
            cout << "collision\n";
            velocity = 0.0f;
        }
        if(fuel_collision(qcar.bs, qcar.get_the_bs())){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            to.RenderText("Collision!!", -0.8, 0.0, 0.004);
            glDisable(GL_BLEND);
            cout << "collision\n";
            velocity += 0.5;
            float rand_dist = (float) rand() / (float) (RAND_MAX / (outer_radius - inner_radius)) + inner_radius;
            float rand_angle = (float) rand() / (float) (RAND_MAX / (60 * M_PI / 180)) + 120 * M_PI / 180;
            fuel_angle += rand_angle;
            fuel_x = rand_dist * cos(fuel_angle);
            fuel_z = rand_dist * sin(fuel_angle);
        }


        if(velocity > 0){
            qcar.dir_off += velocity;
            velocity -= retardation;
        }

        if(velocity < 0){
            velocity = 0.0f;
        }

        GLfloat model2[] = {40.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 40.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat model[] = {50.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 50.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 50.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat model3[] = {1/1.8, 0.0f, 0.0f, 0.0f,
                            0.0f, 1/1.8, 0.0f, 0.0f,
                            0.0f, 0.0f, 1, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat transform3[] = {1.0f, 0.0f, 0.0f, fuel_x,
                                0.0f, 1.0f, 0.0f, fuel_y,
                                0.0f, 0.0f, 1.0f, fuel_z,
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat transform[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f};

        float cam_x = qcar.x - (qcar.dir_off - (cam_queen_dist)) * sin(qcar.player_angle);
        float cam_z = qcar.z + (qcar.dir_off - (cam_queen_dist)) * cos(qcar.player_angle);
        float cam_y = qcar.y + 5.0f * car_scale;

        GLfloat view[] = {cos(qcar.player_angle), 0.0f, sin(qcar.player_angle), -cos(qcar.player_angle) * cam_x - sin(qcar.player_angle) * cam_z,
                                0.0f, 1.0f, 0.0f, -cam_y,
                                -sin(qcar.player_angle), 0.0f, cos(qcar.player_angle), -(-sin(qcar.player_angle) * cam_x) - cos(qcar.player_angle) * cam_z,
                                0.0f, 0.0f, 0.0f, 1.0f};

        GLfloat projection[] = {1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.00001f, -2.001f,
                            0.0f, 0.0f, 1.0f, 0.0f};

        stadium.Display(model, transform, view, projection);

        qcar.Display(view, projection);
        opcar.Display(view, projection);
        opcar2.Display(view, projection);
        opcar3.Display(view, projection);
        fuel_tank.Display(model3, transform3, view, projection);

        
        // opcar.get_the_bs();
        // qcar.get_the_bs();
        glDisable(GL_BLEND);

        opcar.dir_off += 1.5f;
        opcar2.dir_off += 1.4f;
        opcar3.dir_off += 1.1f;

        glViewport(0, 0, 300, 300);

        cam_x = qcar.x - qcar.dir_off * sin(qcar.player_angle); // need base_x though
        cam_y = qcar.y + 300.0f * car_scale;
        cam_z = qcar.z + qcar.dir_off * cos(qcar.player_angle);
        float cangle = cos(qcar.player_angle);
        float sangle = sin(qcar.player_angle);

        GLfloat projection2[] = {1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f};

        GLfloat view2[] = {1.0f, 0.0f, 0.0f, -cam_x,
                        0.0f, 0.0f, 1.0f, -cam_z,
                        0.0f, -1.0f, 0.0f, cam_y,
                        0.0f, 0.0f, 0.0f, 1.0f};

        stadium.Display(model, transform, view2, projection);
        qcar.Display(view2, projection);
        opcar.Display(view2, projection);
        opcar2.Display(view2, projection);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
        process_input(window);
    }
}