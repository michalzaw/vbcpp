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

inline float mapRange(float a1, float a2, float b1, float b2, float value)
{
    return b1 + (value - a1) * (b2 - b1) / (a2 - a1);
}


#endif // MATH_H_INCLUDED
