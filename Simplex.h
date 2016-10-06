#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <random>


class Simplex {
    public:
        Simplex(std::mt19937* generator, float scale, float min, float max);
        ~Simplex();
        float valSimplex(float x, float y);


    private:
        int fastfloor(const float x);
        float dot(const int* g, const float x, const float y);
        static const int grad3[12][3];
        std::mt19937* generator;

        std::vector<int> perm;
        float scale;
        float min;
        float max;
};


#endif
