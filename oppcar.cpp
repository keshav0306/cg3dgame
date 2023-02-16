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
    this->curr_angle = 20 * M_PI / 180;
    float cx = this->curr_following_x = dist * cos(this->curr_angle);
    float cz = this->curr_following_z = dist * sin(this->curr_angle);
    this->curr_following_dist = sqrt((cx - x) * (cx - x) + (cz - z) * (cz - z));
    cout << x << " " << z << endl;
    this->last_angle = 0.0f;
}

void OpponentCar::Display(float * view, float * projection){

    float angle = atan((-this->curr_following_x + this->x) / (this->curr_following_z - this->z));

    int mult = 1;
    float dx = this->x - this->curr_following_x;
    float dz = this->curr_following_z - this->z;

    if(dx > 0 && dz > 0){
        mult = 1;
    }
    else if(dx > 0 && dz < 0){
        mult = -1;
    }
    else if(dx < 0 && dz > 0){
        mult = 1;
    }
    else{
        mult = -1;
    }

    // cout << "angle : " << angle << endl;
    // cout << this->curr_following_dist << endl;
    // cout << " ----- \n";
    // cout << " x and z : " << this->x << " " << this->z << endl;
    // cout << " dir_off : " << this->dir_off << endl;
    // cout << " aim x aim z : " << this->curr_following_x << " " << this->curr_following_z << endl;
    // cout << " aim dist : " << this->curr_following_dist << endl;
    // cout << " translated : " <<  this->x - dir_off * sin(angle) << " " << this->z + dir_off * cos(angle) << endl;
    // cout << " angle : " << angle << endl;
    // cout << " ----- \n";

    float change_angle = (angle - last_angle) *dir_off / curr_following_dist + last_angle;
    GLfloat transform[] = {cos(change_angle), 0.0f, -sin(change_angle), this->x - dir_off * sin(angle) * mult,
                            0.0f, 1.0f, 0.0f, this->y,
                            sin(change_angle), 0.0f, cos(change_angle), this->z + dir_off * cos(angle) * mult,
                            0.0f, 0.0f, 0.0f, 1.0f};

    if(dir_off >= this->curr_following_dist){
        this->last_angle = atan((-this->curr_following_x + this->x) / (this->curr_following_z - this->z));
        this->curr_angle += 20 * M_PI / 180;
        float dist = (float) rand() / (float) (RAND_MAX / (outer_radius - inner_radius)) + inner_radius;
        this->x = this->x - dir_off * sin(angle) * mult;
        this->z = this->z + dir_off * cos(angle) * mult;
        // cout << this->x << " " << this->z << endl;
        float cx = this->curr_following_x = dist * cos(this->curr_angle);
        float cz = this->curr_following_z = dist * sin(this->curr_angle);
        // cout << "angle : " << angle << endl;
        // cout << "new cx and cz : " << this->curr_following_x << " " << this->curr_following_z << endl;
        this->dir_off = -0.05f;
        this->curr_following_dist = sqrt((cx - this->x) * (cx - this->x) + (cz - this->z) * (cz - this->z));
    }
    dir_off +=0.10f;
    this->m->Display(this->model, transform, view, projection);
    
}