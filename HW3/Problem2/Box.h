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

/**
\file Box.h

\brief Header file for Box.cpp

\author    Don Spickler
\version   1.2
\date      Written: 1/11/2016  <BR> Revised: 1/12/2016

*/

/**
\class Box

\brief The Box class holds position and color data for a multicolored rectangle.  It also has
facilities for loading the data to the graphics card and invoking draw commands on the data.

*/

class Box
{
private:
    GLfloat width;        ///< Width of the box.
    GLfloat height;       ///< Height of the box.
    GLfloat centerX;      ///< X coordinate of the center of the box.
    GLfloat centerY;      ///< Y coordinate of the center of the box.
    GLfloat colors[12];   ///< Array holding the (r,g,b) color values for each of the 4 vertices, [r1, g1, b1, r2, g2, b2, ...]

    GLuint BoxVAO;        ///< Vertex Array Object ID.
    GLuint ArrayBuffer;   ///< Vertex and Color Information Array Buffer ID.
    GLuint BoxEBO;        ///< Indices Array Object ID.

    GLint vPosition;      ///< Array Buffer position index.
    GLint vColor;         ///< Array Buffer color index.

    void LoadDataToGraphicsCard();

public:
    Box(GLfloat cx = 0, GLfloat cy = 0, GLfloat w = 1, GLfloat h = 1);
    ~Box();

    void setWidth(GLfloat w);
    void setHeight(GLfloat h);
    void setCenter(GLfloat cx, GLfloat cy);
    void setVertexColor(GLint v, GLfloat r, GLfloat g,GLfloat b);
    GLfloat getWidth();
    GLfloat getHeight();
    void getCenter(GLfloat* c);
    bool compareToBox(float x, float y);
    void changeColor(bool red);
    bool blue = false;///<IS the box blue
    bool green = false;///<Is the box green
    bool white = false;///<Is the box white
    bool regular = true;///<Is the box regular initial colors


    void draw();
};

#endif // BOX_H_INCLUDED
