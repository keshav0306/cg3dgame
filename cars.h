class Car{
    public: 
    float health;
    float fuel;
    float score;
    float time;
    float mileage;
    Model * m;
    float * model;
    float x, y, z;
    float base_x, base_y, base_z;
};

class OpponentCar : public Car{
    public:
    OpponentCar(Model * m, float * model, float x, float y, float z);
    void Display(float * view, float * projection);
    float curr_following_x;
    float curr_following_z;
    float inner_radius;
    float outer_radius;
    float dir_off;
    float curr_following_dist;
    float curr_angle;
    float last_angle;
};

class QueenCar : public Car{
    public:
    QueenCar(Model * m, float * model, float x, float y, float z);
    QueenCar(){
        ;
    };
    void init(Model * m, float * model, float x, float y, float z);
    void Display(float * view, float * projection);
    float player_angle;
    float dir_off;
};