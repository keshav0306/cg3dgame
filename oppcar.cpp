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
#include <tuple>

using namespace std;
#include "bs.h"
#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "cars.h"

extern vector<float> mult4v4(float * m, vector <float> v);

OpponentCar::OpponentCar(Model * m, float * model, float inner_radius, float outer_radius, float y, float scale, int index){
    // cout << "outer radius" << outer_radius << endl;
    this->m = m;
    this->model = model;
    this->inner_radius = inner_radius;
    this->outer_radius = outer_radius;
    float x = this->base_x = this->x = (inner_radius + outer_radius )/ 2;
    float z = this->base_z = this->z = 0.0f;
    this->y = y;
    // cout << "oppcar init : " << this->x << " " << this->y << " " << this->z << endl;
    float dist = (float) rand() / (float) (RAND_MAX / (outer_radius - inner_radius)) + inner_radius;
    this->curr_angle = 15 * M_PI / 180;
    float cx = this->curr_following_x = dist * cos(this->curr_angle);
    float cz = this->curr_following_z = dist * sin(this->curr_angle);
    this->curr_following_dist = sqrt((cx - x) * (cx - x) + (cz - z) * (cz - z));
    cout << x << " " << z << endl;
    this->dir_off = 0.0f;
    this->last_angle = 0.0f;
    this->index = index;
    bs.init(5, m->max_x * scale, m->min_y * scale, m->max_y * scale, 0, 0);

}

void OpponentCar::Display(float * view, float * projection){
    // cout << " before x and z : " << this->x << " " << this->z << endl;

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

    float change_angle = (angle - this->last_angle) *dir_off / curr_following_dist + last_angle;
    GLfloat transform[] = {cos(change_angle), 0.0f, -sin(change_angle), this->x - dir_off * sin(angle) * mult,
                            0.0f, 1.0f, 0.0f, this->y,
                            sin(change_angle), 0.0f, cos(change_angle), this->z + dir_off * cos(angle) * mult,
                            0.0f, 0.0f, 0.0f, 1.0f};

    this->base_x = this->x - dir_off * sin(angle) * mult;
    this->base_z = this->z + dir_off * cos(angle) * mult;

    if(dir_off >= this->curr_following_dist){
        this->last_angle = atan((-this->curr_following_x + this->x) / (this->curr_following_z - this->z));
        this->curr_angle += 15 * M_PI / 180;
        float dist = (float) rand() / (float) (RAND_MAX / (outer_radius - inner_radius)) + inner_radius;
        // cout << "inside if : " <<  this->x << " " << this->z << endl;
        this->x = this->x - dir_off * sin(angle) * mult;
        this->z = this->z + dir_off * cos(angle) * mult;
        // cout << this->x << " " << this->z << endl;
        float cx = this->curr_following_x = dist * cos(this->curr_angle);
        float cz = this->curr_following_z = dist * sin(this->curr_angle);
        // cout << "angle : " << angle << endl;
        // cout << "new cx and cz : " << this->curr_following_x << " " << this->curr_following_z << endl;
        this->dir_off = -0.05f;
        this->curr_following_dist = sqrt((cx - this->x) * (cx - this->x) + (cz - this->z) * (cz - this->z));
        // cout << "inside if bye : " <<  this->x << " " << this->z << endl;

    }
    // dir_off +=0.10f;
    // cout << "display func : " << this->dir_off << endl;
    this->m->Display(this->model, transform, view, projection);
    
}

vector<vector<float>> OpponentCar::get_the_bs(){
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
    float change_angle = (angle - this->last_angle) *dir_off / curr_following_dist + last_angle;
    GLfloat transform[] = {cos(change_angle), 0.0f, -sin(change_angle), this->x - dir_off * sin(angle) * mult,
                            0.0f, 1.0f, 0.0f, this->y,
                            sin(change_angle), 0.0f, cos(change_angle), this->z + dir_off * cos(angle) * mult,
                            0.0f, 0.0f, 0.0f, 1.0f};
    // cout <<  "dir_off is : " <<  this->dir_off << endl;
    // cout << "-------------------\n";
    vector <vector<float> > ret;
    for(int i=0;i<this->bs.n;i++){
        vector<float> v;
        v.push_back(get<0>(this->bs.sphere_centers[i]));
        v.push_back(get<1>(this->bs.sphere_centers[i]));
        v.push_back(get<2>(this->bs.sphere_centers[i]));
        v.push_back(1.0f);
        // vector <float> out = mult4v4(this->model, v);
        vector <float> out2 = mult4v4(transform, v);
        ret.push_back(out2);
        // cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << endl;
        // cout << out[0] << " " << out[1] << " " << out[2] << " " << out[3] << endl;
        // cout << out2[0] << " " << out2[1] << " " << out2[2] << " " << out2[3] << endl;
    }
    // cout << "-------------------\n";
    return ret;
}