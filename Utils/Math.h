#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED


const GLfloat PI = 3.14159265359f;


inline float DegToRad(float deg)
{
    return float((deg * PI) / 180);
}

inline float RadToDeg(float rad)
{
    return float((rad * 180 ) / PI);
}



#endif // MATH_H_INCLUDED
