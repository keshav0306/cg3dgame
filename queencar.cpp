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

QueenCar::QueenCar(Model * m, float * model, float x, float y, float z){
    init(m, model, x, y, z);
}

void QueenCar::Display(float * view, float * projection){

    GLfloat transform[] = {cos(this->player_angle), 0.0f, -sin(this->player_angle), this->x - this->dir_off * sin(this->player_angle),
                                0.0f, 1.0f, 0.0f, this->y,
                                sin(this->player_angle), 0.0f, cos(this->player_angle), this->z + this->dir_off * cos(this->player_angle),
                                0.0f, 0.0f, 0.0f, 1.0f};

    this->m->Display(this->model, transform, view, projection);

}

void QueenCar::init(Model * m, float * model, float x, float y, float z){
    this->m = m;
    this->model = (float *)malloc(sizeof(float) * 16);
    memcpy(this->model, model, 16 * sizeof(float));
    this->x = this->base_x = x;
    this->y = this->base_y = y;
    this->z = this->base_z = z;
    this->dir_off = 0.0f;
    this->player_angle = 0.0f;
}