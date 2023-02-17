struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture{
    string type;
    string path;
    unsigned int id;
    int nrComponents;
};

class Mesh{
    public:
    vector <Vertex> vertices;
    vector <unsigned int> indices;
    vector <Texture> textures;
    unsigned int VAO, VBO, EBO;
    Shader shader;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> texture, Shader shader);

    void makeTheBuffs();

    void display(GLfloat *model, GLfloat * transform, GLfloat * view, GLfloat * projection);
};