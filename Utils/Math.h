#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED


const GLfloat PI = 3.14159265359f;


inline float degToRad(float deg)
{
    return float((deg * PI) / 180);
}

inline float radToDeg(float rad)
{
    return float((rad * 180 ) / PI);
}



#endif // MATH_H_INCLUDED
