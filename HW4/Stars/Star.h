#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <iostream>

#include "ProgramDefines.h"
//#include "GraphicsEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
\file Box.h

\brief Header file for Box.cpp

\author    Ryan Clocker
\version   1.0
\date     2/26/2019

*/

/**
\class Box

\brief The Box class holds position and color data for a multicolored rectangle.  It also has
facilities for loading the data to the graphics card and invoking draw commands on the data.

*/

class Star
{

private:
    int points; ///< The number of points on the star.
    GLuint VAO; ///< Vertex Array Object ID.
    GLuint ArrayBuffer; ///< Vertex and Color Information Array Buffer ID.
    GLint vPosition; ///< Array Buffer position index.
    GLint vColor; ///< Array Buffer color index.

public:
    Star(int numpoints = 5, GLfloat outerRadius = 1, GLfloat innerRadius = 0.35,
         GLfloat ptred = 1, GLfloat ptgreen = 0, GLfloat ptblue = 0,
         GLfloat ctred = 1, GLfloat ctgreen = 1, GLfloat ctblue = 1);
    ~Star();

    void draw();

};
#endif // BOX_H_INCLUDED
