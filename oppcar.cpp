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

OpponentCar::OpponentCar(Model * m, float * model, float inner_radius, float outer_radius, float y){

    this->m = m;
    this->model = model;
    this->inner_radius = inner_radius;
    this->outer_radius = outer_radius;
    float x = this->x = (inner_radius + outer_radius )/ 2;
    float z = this->z = 0.0f;
    this->y = y;
    float dist = (float) rand() / (float) (RAND_MAX / (outer_radius - inner_radius)) + inner_radius;
    this->curr_angle = 50 * M_PI / 180;
    float cx = this->curr_following_x = dist * cos(this->curr_angle);
    float cz = this->curr_following_z = dist * sin(this->curr_angle);
    this->curr_following_dist = sqrt((cx - x) * (cx - x) + (cz - z) * (cz - z));
}

void OpponentCar::Display(float * view, float * projection){

    float t1 = cos(glfwGetTime());
    float t2 =  sin(glfwGetTime());
    float angle = atan((-this->curr_following_x + this->x) / (this->curr_following_z - this->z));
    cout << this->curr_following_dist << endl;
    GLfloat transform[] = {cos(angle), 0.0f, -sin(angle), this->x - dir_off * sin(angle),
                            0.0f, 1.0f, 0.0f, this->y,
                            sin(angle), 0.0f, cos(angle), this->z + dir_off * cos(angle),
                            0.0f, 0.0f, 0.0f, 1.0f};
    dir_off+=0.05f;
    if(dir_off >= this->curr_following_dist){
        this->curr_angle += 50 * M_PI / 180;
        float dist = (float) rand() / (float) (RAND_MAX / (outer_radius - inner_radius)) + inner_radius;
        this->x = this->curr_following_x;
        this->z = this->curr_following_z;
        float cx = this->curr_following_x = dist * cos(this->curr_angle);
        float cz = this->curr_following_z = dist * sin(this->curr_angle);
        this->dir_off = 0.0f;
        this->curr_following_dist = sqrt((cx - this->x) * (cx - this->x) + (cz - this->z) * (cz - this->z));
    }
    this->m->Display(this->model, transform, view, projection);
}
