class BoundingSpheres{
    public:
    BoundingSpheres();
    BoundingSpheres(int n, float radius, float zmin, float zmax, float x, float y);
    void init(int n, float radius, float zmin, float zmax, float x, float y);
    vector<tuple<float, float, float> > sphere_centers;
    float radius;
    int n;
};
