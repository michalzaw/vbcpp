#include "PoissonDiscSampler.h"


PoissonDiscSampler::PoissonDiscSampler(float width, float height, float minDistance, int k/* = 30*/)
    : _width(width), _height(height), _minDistance(minDistance), _k(k),
    _cellSize(minDistance / std::sqrt(2.0f)),
    _grid((int)(_width / _cellSize) + 1, std::vector<int>((int)(_height / _cellSize) + 1, -1)),
    _randomGenerator((unsigned int)std::time(0)), _distAngle(0.0f, 2.0f * PI), _distRadius(_minDistance, 2 * _minDistance)
{
    _pointValidationFunction = [this](const glm::vec2& point) {
        if (point.x < 0 || point.y < 0 || point.x >= _width || point.y >= _height)
        {
            return false;
        }
        return true;
    };
}


float PoissonDiscSampler::generateRandomFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(_randomGenerator);
}


int PoissonDiscSampler::generateRandomInt(int minValue, int maxValue)
{
    std::uniform_int_distribution<> distribution(minValue, maxValue);

    return distribution(_randomGenerator);
}


glm::vec2 PoissonDiscSampler::generateRandomPointAround(const glm::vec2& p)
{
    float angle = _distAngle(_randomGenerator);
    float r = _distRadius(_randomGenerator);

    return glm::vec2(p.x + r * std::cos(angle), p.y + r * std::sin(angle));
}


void PoissonDiscSampler::placeInGrid(const glm::vec2& p, int index)
{
    int gridX = (int) (p.x / _cellSize);
    int gridY = (int) (p.y / _cellSize);
    _grid[gridX][gridY] = index;
}


bool PoissonDiscSampler::isValid(const glm::vec2& p, const std::vector<glm::vec2>& points) {
    if (!_pointValidationFunction(p))
    {
        return false;
    }

    int gridX = (int) (p.x / _cellSize);
    int gridY = (int) (p.y / _cellSize);

    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            int currentGridX = gridX + i;
            int currentGridY = gridY + j;
            if (currentGridX >= 0 && currentGridY >= 0 && currentGridX < _grid.size() && currentGridY < _grid[0].size())
            {
                int index = _grid[currentGridX][currentGridY];
                if (index != -1)
                {
                    if (glm::distance(p, points[index]) < _minDistance) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}


void PoissonDiscSampler::generate(std::vector<glm::vec2>& outPoints)
{
    outPoints.clear();

    std::vector<glm::vec2> activePoints;


    for (int i = 0; i < _k; ++i)
    {
        glm::vec2 initial(generateRandomFloat(0, _width), generateRandomFloat(0, _height));
        if (_pointValidationFunction(initial))
        {
            outPoints.push_back(initial);
            activePoints.push_back(initial);
            placeInGrid(initial, outPoints.size() - 1);

            break;
        }
    }

    while (!activePoints.empty())
    {
        int centerPointIndex = generateRandomInt(0, activePoints.size() - 1);
        glm::vec2 center = activePoints[centerPointIndex];
        bool found = false;

        for (int i = 0; i < _k; ++i)
        {
            glm::vec2 newPoint = generateRandomPointAround(center);
            if (isValid(newPoint, outPoints))
            {
                outPoints.push_back(newPoint);
                activePoints.push_back(newPoint);
                placeInGrid(newPoint, outPoints.size() - 1);
                found = true;
                break;
            }
        }

        if (!found)
        {
            activePoints.erase(activePoints.begin() + centerPointIndex);
        }
    }
}
