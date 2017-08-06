#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED


#include <glm/glm.hpp>


class AABB
{
    private:
        glm::vec3 _minCoords;
        glm::vec3 _maxCoords;

    public:
        AABB()
        {
            _minCoords = _maxCoords = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        AABB(glm::vec3 min, glm::vec3 max)
        {
            setSize(min, max);
        }

        void setSize(glm::vec3 min, glm::vec3 max)
        {
            _minCoords = min;
            _maxCoords = max;
        }

        glm::vec3 getMinCoords()
        {
            return _minCoords;
        }

        glm::vec3 getMaxCoords()
        {
            return _maxCoords;
        }

        glm::vec3 getCenterPosition()
        {
            return glm::vec3((_maxCoords.x + _minCoords.x) / 2.0f,
                             (_maxCoords.y + _minCoords.y) / 2.0f,
                             (_maxCoords.z + _minCoords.z) / 2.0f);
        }

};


#endif // AABB_H_INCLUDED
