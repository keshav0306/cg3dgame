#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>
#include <assimp/postprocess.h>

using namespace std;
#include "shader.h"
#include "mesh.h"
#include "model.h"

Model::Model(char * path, Shader shader){
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
    }
    string spath = string(path);
    directory = spath.substr(0, spath.find_last_of('/'));
    this->shader = shader;
    climbTheTree(scene->mRootNode, scene);
}


void Model::climbTheTree(aiNode * node, const aiScene * scene){
    for(int i=0;i<node->mNumMeshes;i++){
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(makeMesh(mesh, scene));
    }
    for(unsigned int i=0;i<node->mNumChildren;i++){
        climbTheTree(node->mChildren[i], scene);
    }
}

Mesh Model::makeMesh(aiMesh * mesh, const aiScene * scene){
    // need to transport vertices, indices and textures of the mesh to the mesh constructor
    vector <Vertex> vertices;
    vector <unsigned int> indices;
    vector <Texture> textures;

    // First fill the vertices in the vertex array

    for(unsigned int i=0;i<mesh->mNumVertices; i++){

        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        // cout << "Vertices : " << vector.x << " " << vector.y << " " << vector.z << endl;
        vertex.Position = vector;
        
        if(mesh->HasNormals()){
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if(mesh->mTextureCoords[0]){
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            
            // vector.x = mesh->mTangents[i].x;
            // vector.y = mesh->mTangents[i].y;
            // vector.z = mesh->mTangents[i].z;
            // vertex.Tangent = vector;
            
            // vector.x = mesh->mBitangents[i].x;
            // vector.y = mesh->mBitangents[i].y;
            // vector.z = mesh->mBitangents[i].z;
            // vertex.Bitangent = vector;
        }
        else{
            cout << "no tex coords for this mesh\n";
            vertex.TexCoords = glm::vec2(0.5f, 0.5f);
        }
        vertices.push_back(vertex);

    }

    // Fill the indices of the mesh's faces

    for(unsigned int i=0;i<mesh->mNumFaces;i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j=0;j<face.mNumIndices;j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    // Now time for the textures
    aiMaterial * mat = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> diffuseMaps = loadTextures(scene, mat, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    vector<Texture> specMaps = loadTextures(scene, mat, aiTextureType_SPECULAR, "texture_specular");
    // cout << specMaps.size() << endl;
    textures.insert(textures.end(), specMaps.begin(), specMaps.end());
    vector<Texture> normalMaps = loadTextures(scene, mat, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    vector<Texture> ambientMaps = loadTextures(scene, mat, aiTextureType_AMBIENT, "texture_ambient");
    textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
    cout << "size of textures : " << textures.size() << endl;
    return Mesh(vertices, indices, textures, shader);
}
    
vector<Texture> Model::loadTextures(const aiScene * scene, aiMaterial * mat, aiTextureType type, string typeName){
    vector <Texture> textures;
    cout << "count : " << mat->GetTextureCount(type) << typeName << endl;
    aiString tfile;
    // mat->Get(AI_MATKEY_TEXTURE(type, 0), tfile);
    mat->GetTexture(type, 0, &tfile);

    for(unsigned int i=0;i<mat->GetTextureCount(type);i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.nrComponents = 0;
        int width, height, nrComponents;

        // for glb files

        // const char * pntr = str.C_Str();
        // string strstar = string(pntr + 1);
        // int texture_index = stoi(strstar);
        // aiTexture * t = scene->mTextures[texture_index];
        // const unsigned char * mem = (unsigned char *)t->pcData;
        // // cout << "Tex File : " << strstar << endl;
        // unsigned char *data = stbi_load_from_memory(mem, t->mWidth, &width, &height, &nrComponents, 0);
        
        // for other files

        string filename = directory + '/' + string(str.C_Str());
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        unsigned int textureID;
        glGenTextures(1, &textureID);
        if(data){
            GLenum format;
            if(nrComponents == 1){
                format = GL_RED;
                texture.nrComponents = 1;
                cout << "red\n";
            }
            else if(nrComponents == 3){
                format = GL_RGB;
                texture.nrComponents = 3;
                cout << "rgb\n";
            }
            else if(nrComponents == 4){
                format = GL_RGBA;
                texture.nrComponents = 4;
                cout << "rgba";
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            stbi_image_free(data);
        }
        else{
            cout << "FAILED TO LOAD A TEXTURE\n";
            stbi_image_free(data);
        }
        texture.id = textureID;
        // cout << textureID << endl;
        texture.type = typeName;
        textures.push_back(texture);
    }
    return textures;
}

void Model::Display(GLfloat *model, GLfloat * transform, GLfloat * view, GLfloat * projection){
    // cout << model[0] << endl;
    for(int i=0;i<meshes.size();i++){
        meshes[i].display(model, transform, view, projection);
    }
}