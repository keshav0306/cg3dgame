#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <tuple>
#include <string>
#include <vector>
#include <stdlib.h>
#include <glm/glm.hpp>

using namespace std;
#include "bs.h"

BoundingSpheres::BoundingSpheres(){
    ;
}

BoundingSpheres::BoundingSpheres(int n, float radius, float zmin, float zmax, float x, float y){
    init(n, radius, zmin, zmax, x, y);
}

void BoundingSpheres::init(int n, float radius, float zmin, float zmax, float x, float y){
    // always call with n >= 2
    this->radius = radius;
    assert(n >= 2);
    tuple<float, float, float> cf, cl;
    this->n = n;
    cf = make_tuple(x, y, zmin + radius);
    cl = make_tuple(x, y, zmax - radius);
    this->sphere_centers.push_back(cf);
    float length = zmax - zmin;
    for(int i=0;i<n-2;i++){
        tuple<float, float, float> t = make_tuple(x, y, zmin + radius + (i + 1) * (length - 2*radius) / (n - 1));
        this->sphere_centers.push_back(t);
    }
    this->sphere_centers.push_back(cl);
    cout << "-----bs.cpp-----\n";
    for(int i=0;i<n;i++){
        cout << get<2>(sphere_centers[i]) << endl;
    }
    cout << "----------\n";

}