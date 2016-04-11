#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED


#include <glm/glm.hpp>

//#include <LinearMath/btVector3.h>


class Vector3
{
	public:
		float x;
		float y;
		float z;

	public:
		Vector3()
			: x(0.0f), y(0.0f), z(0.0f)
		{}

		Vector3(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z)
		{}

		Vector3(const glm::vec3& vec)
			: x(vec.x), y(vec.y), z(vec.z)
		{}

		//Vector3(const btVector3& vec)
		//	: x(vec[0]), y(vec[1]), z(vec[2])
		//{}

		glm::vec3 GLMvec() const
		{
			return glm::vec3(x, y, z);
		}

		operator glm::vec3()
		{
			return glm::vec3(x, y, z);
		}

		//operator btVector3()
		//{
		//	return btVector3(x, y, z);
		//}

		Vector3& operator+=(Vector3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;

			return *this;
		}

		Vector3& operator+=(glm::vec3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;

			return *this;
		}

		/*Vector3& operator+=(btVector3& vec)
		{
			x += vec[0];
			y += vec[1];
			z += vec[2];

			return *this;
		}*/

		Vector3& operator-=(Vector3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;

			return *this;
		}

		Vector3& operator-=(glm::vec3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;

			return *this;
		}

		/*Vector3& operator-=(btVector3& vec)
		{
			x -= vec[0];
			y -= vec[1];
			z -= vec[2];

			return *this;
		}*/

		Vector3& operator*=(float Scalar)
		{
			x *= Scalar;
			y *= Scalar;
			z *= Scalar;

			return *this;
		}

		Vector3& operator/=(float Scalar)
		{
			x /= Scalar;
			y /= Scalar;
			z /= Scalar;

			return *this;
		}

		Vector3 operator-() const
		{
			return Vector3(-x, -y, -z);
		}

};


inline Vector3 operator+(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

inline Vector3 operator-(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

inline Vector3 operator*(float Scalar, const Vector3& vec)
{
	return Vector3(Scalar * vec.x, Scalar * vec.y, Scalar * vec.z);
}

inline Vector3 operator*(const Vector3& vec, float Scalar)
{
	return Vector3(Scalar * vec.x, Scalar * vec.y, Scalar * vec.z);
}

// dot
inline float operator*(const Vector3& vec1, const Vector3& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

inline Vector3 operator/(const Vector3& vec, float Scalar)
{
	return Vector3(vec.x / Scalar, vec.y / Scalar, vec.z / Scalar);
}

inline bool operator==(const Vector3& vec1, const Vector3& vec2)
{
    return vec1.GLMvec() == vec2.GLMvec();
}

inline bool operator!=(const Vector3& vec1, const Vector3& vec2)
{
    return vec1.GLMvec() != vec2.GLMvec();
}


#endif // VECTOR3_H_INCLUDED
