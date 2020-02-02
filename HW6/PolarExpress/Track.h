#ifndef TRACK_H_INCLUDED
#define TRACK_H_INCLUDED


#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>

#include "ProgramDefines.h"



/**
\file Track.h
\brief Header file for Track.cpp

\author    Ryan Clocker
\version   1.0
\date      3/27/2019

*/
struct VertexData
{
    GLfloat color[3];    ///< 3-D color array for (r, g, b) colors.
    GLfloat position[4]; ///< 4-D position array for (x, y, z, w) coordinates.
};
class Track
{

private:
    GLuint NumVertices = 500;

    GLint vPosition;    ///< Vertex data position, for the shader.
    GLint vColor;       ///< Color data position, for the shader.

// Arrays to hold buffer "addresses"
    GLuint VAO;       ///< Array of Vertex Array Object addresses.
    GLuint Buffer;    ///< Array of Buffer addresses.

    GLenum mode;



public:
    Track(float radius);
    void draw();

    ~Track();


protected:


};

#endif // TRACK_H
