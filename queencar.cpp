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

vector<float> mult4v4(float * m, vector <float> v){
    vector<float> v2;
    for(int i=0;i<4;i++){
        float sum = 0;
        for(int j=0;j<4;j++){
            sum += m[i * 4 + j] * v[j];
        }
        v2.push_back(sum);
    }
    return v2;
}

QueenCar::QueenCar(Model * m, float * model, float x, float y, float z, float scale){
    init(m, model, x, y, z, scale);
}

void QueenCar::Display(float * view, float * projection){

    GLfloat transform[] = {cos(this->player_angle), 0.0f, -sin(this->player_angle), this->x - this->dir_off * sin(this->player_angle),
                                0.0f, 1.0f, 0.0f, this->y,
                                sin(this->player_angle), 0.0f, cos(this->player_angle), this->z + this->dir_off * cos(this->player_angle),
                                0.0f, 0.0f, 0.0f, 1.0f};

    this->base_x = this->x - this->dir_off * sin(this->player_angle);
    this->base_z = this->z + this->dir_off * cos(this->player_angle);
    this->m->Display(this->model, transform, view, projection);

}

void QueenCar::init(Model * m, float * model, float x, float y, float z, float scale){
    this->m = m;
    this->model = (float *)malloc(sizeof(float) * 16);
    memcpy(this->model, model, 16 * sizeof(float));
    this->x = this->base_x = x;
    this->y = this->base_y = y;
    this->z = this->base_z = z;
    this->dir_off = 0.0f;
    this->player_angle = 0.0f;
    bs.init(5, m->max_x * scale, m->min_y * scale, m->max_y * scale, 0, 0);
}

vector<vector<float> > QueenCar::get_the_bs(){
    GLfloat transform[] = {cos(this->player_angle), 0.0f, -sin(this->player_angle), this->x - this->dir_off * sin(this->player_angle),
                                0.0f, 1.0f, 0.0f, this->y,
                                sin(this->player_angle), 0.0f, cos(this->player_angle), this->z + this->dir_off * cos(this->player_angle),
                                0.0f, 0.0f, 0.0f, 1.0f};

    // cout << "-------------------\n";
    vector<vector<float> > ret;
    for(int i=0;i<this->bs.n;i++){
        vector<float> v;
        v.push_back(get<0>(this->bs.sphere_centers[i]));
        v.push_back(get<1>(this->bs.sphere_centers[i]));
        v.push_back(get<2>(this->bs.sphere_centers[i]));
        v.push_back(1.0f);
        // vector <float> out = mult4v4(this->model, v);
        vector <float> out2 = mult4v4(transform, v);
        ret.push_back(out2);
        // cout << out[0] << " " << out[1] << " " << out[2] << " " << out[3] << endl;
    }
    // cout << "-------------------\n";
    return ret;
}