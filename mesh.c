#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "mesh.h"

using namespace std;


Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> texture){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = texture;
    makeTheBuffs();
}

Mesh::void addShader(char * vs_path, char * fs_path){
    this->shader.makeProgram(vs_path, fs_path);
}

Mesh::void makeTheBuffs(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // fill the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    // tex coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    // tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Tangent));
    glEnableVertexAttribArray(3);
    // bitangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Bitangent));
    glEnableVertexAttribArray(4);

    // file the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

Mesh::void display(GLfloat *model, GLfloat * transform, GLfloat * view){
    glBindVertexArray(VAO);
        int comp_loc = glGetUniformLocation(this->shader.prog_id, "comp");
    for(int i=0;i<textures.size();i++){
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
        glUniform1i(comp_loc, textures[i].nrComponents);
        glUniform1f(glGetUniformLocation(this->shader.prog_id, "val"), val);
    }

    if(textures.size() == 0){
        glUniform1i(comp_loc, 0);
    }

    int m_loc = glGetUniformLocation(this->shader.prog_id, "model");
    int t_loc = glGetUniformLocation(this->shader.prog_id, "transform");
    int v_loc = glGetUniformLocation(this->shader.prog_id, "view");

    glUniformMatrix4fv(m_loc, 1, GL_TRUE, model);
    glUniformMatrix4fv(t_loc, 1, GL_TRUE, transform);
    glUniformMatrix4fv(v_loc, 1, GL_TRUE, view);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
