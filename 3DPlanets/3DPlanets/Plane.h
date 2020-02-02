#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "ProgramDefines.h"
#include "Shape.h"

/**
\file Plane.h
\brief Header file for Plane.cpp

\author    Don Spickler
\version   1.2
\date      1/24/2018

*/

/**
\class Plane

\brief The Plane class creates a plane [-1, 1] X [-1, 1] with four vertices,
each with normals, colors and texture coordinates.

*/

class Plane : public Shape
{
private:
    GLint vPosition;   ///< Shader position of the vertex data.
    GLint vColor;      ///< Shader position of the vertex color.
    GLint vNormal;     ///< Shader position of the vertex normal.
    GLint vTex;        ///< Shader position of the texture coordinate.

    GLfloat red;    ///< Red component of the object color.
    GLfloat green;  ///< Green component of the object color.
    GLfloat blue;   ///< Blue component of the object color.

    GLuint vboptr;  ///< ID for the VBO.
    GLuint bufptr;  ///< ID for the data buffer.
    GLuint eboptr;  ///< ID for the index buffer.

public:
    Plane();
    ~Plane();

    void setColor(GLfloat r, GLfloat g, GLfloat b);

    void LoadDataToGraphicsCard();
    void LoadDataToGraphicsCard(GLint v, GLint c, GLint n, GLint t);

    void draw();
};


#endif // PLANE_H_INCLUDED
