#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

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
\file Sphere.h
\brief Header file for Sphere.cpp

\author    Don Spickler
\version   1.2
\date      1/24/2018

*/

/**
\class Sphere

\brief The Sphere class creates a sphere of radius 1, centered at the origin,
with normals, colors, and texture coordinates.

*/

class Sphere : public Shape
{
private:
    GLint vPosition;   ///< Shader position of the vertex data.
    GLint vColor;      ///< Shader position of the vertex color.
    GLint vNormal;     ///< Shader position of the vertex normal.
    GLint vTex;        ///< Shader position of the texture coordinate.

    GLfloat red;       ///< Red component of the object color.
    GLfloat green;     ///< Green component of the object color.
    GLfloat blue;      ///< Blue component of the object color.

    GLuint vboptr;     ///< ID for the VBO.
    GLuint bufptr;     ///< ID for the data buffer.
    GLuint eboptr;     ///< ID for the index buffer.

    GLint lon;         ///< Number of longitudinal divisions.
    GLint lat;         ///< Number of latitude divisions.

    GLboolean reload;  ///< Flag to reload the data to the graphics card.

public:
    Sphere();
    ~Sphere();

    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void resetArraySize(GLint ilon, GLint ilat);

    void LoadDataToGraphicsCard();
    void LoadDataToGraphicsCard(GLint v, GLint c, GLint n, GLint t);

    void draw();
};


#endif // SPHERE_H_INCLUDED
