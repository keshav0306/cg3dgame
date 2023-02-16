class Model{
    public:
    // the list of all meshes in the model. the model is a tree of meshes with each node
    // having multiple children and each node having a pointer to a set of meshes

    vector<Mesh> meshes; 

    // the list of all textures in the file. 
    vector<Texture> textures;

    // not needed for glb files, since the textures come embedded in it
    // but still needed for other files
    string directory; 

    Shader shader;

    Model(){
        ;
    }
    Model(char * path, Shader shader);

    void climbTheTree(aiNode * node, const aiScene * scene);

    Mesh makeMesh(aiMesh * mesh, const aiScene * scene);
    
    vector<Texture> loadTextures(const aiScene * scene, aiMaterial * mat, aiTextureType type, string typeName);
    void Display(GLfloat *model, GLfloat * transform, GLfloat * view, GLfloat * projection);
    
};
