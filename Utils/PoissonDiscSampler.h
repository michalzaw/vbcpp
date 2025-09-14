#ifndef POISSONDISCSAMPLER_H_INCLUDED
#define POISSONDISCSAMPLER_H_INCLUDED


#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>
#include <functional>

#include <glm/glm.hpp>

#include "Math.h"


class PoissonDiscSampler
{
    private:
        float _width;
        float _height;
        float _minDistance;
        int _k;

        float _cellSize;

        std::vector<std::vector<int>> _grid;

        std::mt19937 _randomGenerator;
        std::uniform_real_distribution<float> _distAngle;
        std::uniform_real_distribution<float> _distRadius;

        std::function<bool(const glm::vec2&)> _pointValidationFunction;

        float generateRandomFloat(float min, float max);
        int generateRandomInt(int minValue, int maxValue);
        glm::vec2 generateRandomPointAround(const glm::vec2& p);

        void placeInGrid(const glm::vec2& p, int index);
        bool isValid(const glm::vec2& p, const std::vector<glm::vec2>& points);

    public:
        PoissonDiscSampler(float width, float height, float radius, int k = 30);

        void setPointValidationFunction(const std::function<bool(const glm::vec2&)>& pointValidationFunction) { _pointValidationFunction = pointValidationFunction; }

        void generate(std::vector<glm::vec2>& outPoints);

};


#endif // POISSONDISCSAMPLER_H_INCLUDED