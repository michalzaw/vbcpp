#include "PolygonGenerator.h"


namespace PolygonGenerator
{
	int getPreviousPointIndex(std::vector<glm::vec3>& points, std::vector<bool>& points2, int index)
	{
		int previousPointIndex = index;
		do
		{
			--previousPointIndex;

			if (previousPointIndex < 0)
			{
				previousPointIndex = points.size() - 1;
			}
		} while (!points2[previousPointIndex]);

		return previousPointIndex;
	}


	int getNextPointIndex(std::vector<glm::vec3>& points, std::vector<bool>& points2, int index)
	{
		int previousPointIndex = index;
		do
		{
			++previousPointIndex;
			previousPointIndex = previousPointIndex % points.size();
		} while (!points2[previousPointIndex]);

		return previousPointIndex;
	}


	// https://stackoverflow.com/questions/471962/how-do-i-efficiently-determine-if-a-polygon-is-convex-non-convex-or-complex
	bool isConvexPoint(std::vector<glm::vec3>& points, std::vector<bool>& points2, int index)
	{
		int i1 = getPreviousPointIndex(points, points2, index);
		int i2 = index;
		int i3 = getNextPointIndex(points, points2, index);

		glm::vec3 p1 = points[i1];
		glm::vec3 p2 = points[i2];
		glm::vec3 p3 = points[i3];

		float dx1 = p3.x - p2.x;
		float dx2 = p1.x - p2.x;
		float dz1 = p3.z - p2.z;
		float dz2 = p1.z - p2.z;

		return dx1 * dz2 - dz1 * dx2 > 0;
	}


	// https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
	float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
	{
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}


	bool isTriangleContainsVertex(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 v)
	{
		float d1, d2, d3;
		bool has_neg, has_pos;

		d1 = sign(v, p1, p2);
		d2 = sign(v, p2, p3);
		d3 = sign(v, p3, p1);

		return !(((d1 < 0) || (d2 < 0) || (d3 < 0)) && ((d1 > 0) || (d2 > 0) || (d3 > 0)));
	}


	bool isTriangleContainsNoConcaveVertex(std::vector<glm::vec3>& points, std::vector<bool>& points2, int index)
	{
		int i1 = getPreviousPointIndex(points, points2, index);
		int i2 = index;
		int i3 = getNextPointIndex(points, points2, index);

		glm::vec3 p1 = points[i1];
		glm::vec3 p2 = points[i2];
		glm::vec3 p3 = points[i3];

		for (int i = 0; i < points.size(); ++i)
		{
			if (i == i1 || i == i2 || i == i3)
			{
				continue;
			}

			if (!isConvexPoint(points, points2, i))
			{
				if (isTriangleContainsVertex(glm::vec2(p1.x, p1.z), glm::vec2(p2.x, p2.z), glm::vec2(p3.x, p3.z), glm::vec2(points[i].x, points[i].z)))
				{
					return false;
				}
			}
		}

		return true;
	}


	// http://cgm.cs.mcgill.ca/~godfried/teaching/cg-projects/97/Ian/cutting_ears.html
	// http://cgm.cs.mcgill.ca/~godfried/teaching/cg-projects/97/Ian/algorithm1.html
	int findEar(std::vector<glm::vec3>& points, std::vector<bool>& points2)
	{
		for (int i = 0; i < points.size(); ++i)
		{
			bool isConvexVertex = isConvexPoint(points, points2, i);
			LOG_INFO(LOG_VARIABLE(i) + ": " + LOG_VARIABLE(isConvexVertex));
		}

		int i = 0;
		//bool earNotFound = true;
		//while (earNotFound)
		for (int i = 0; i < points.size(); ++i)
		{
			if (points2[i])
			{
				if (isConvexPoint(points, points2, i))
				{
					if (isTriangleContainsNoConcaveVertex(points, points2, i))
					{
						return i;
					}
				}
			}
		}

		return -1;
	}
}
