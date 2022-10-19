#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED


const float PI = 3.14159265359f;


inline float degToRad(float deg)
{
    return float((deg * PI) / 180);
}

inline float radToDeg(float rad)
{
    return float((rad * 180 ) / PI);
}

inline float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}


#endif // MATH_H_INCLUDED
