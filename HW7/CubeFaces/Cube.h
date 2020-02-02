#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glew.h>
#endif // __APPLE__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ProgramDefines.h"
#include "Shape.h"

/**
\file Cube.h

\brief Header file for Cube.cpp

\author    Don Spickler
\version   1.2
\date      1/18/2018

*/

/**
\class Cube

\brief The cube class draws a cube of side lengths 1 centered at the origin.

*/

class Cube //: public Shape
{
private:
    GLuint vboptr1;   ///< ID for faces VBO.
    GLuint eboptr1;   ///< ID for faces index array.
    GLuint bufptr1;   ///< ID for faces array buffer.

    GLuint vboptr2;   ///< ID for faces VBO.
    GLuint eboptr2;   ///< ID for faces index array.
    GLuint bufptr2;   ///< ID for faces array buffer.

    GLuint vboptr3;   ///< ID for faces VBO.
    GLuint eboptr3;   ///< ID for faces index array.
    GLuint bufptr3;   ///< ID for faces array buffer.

    GLuint vboptr4;   ///< ID for faces VBO.
    GLuint eboptr4;   ///< ID for faces index array.
    GLuint bufptr4;   ///< ID for faces array buffer.

    GLuint vboptr5;   ///< ID for faces VBO.
    GLuint eboptr5;   ///< ID for faces index array.
    GLuint bufptr5;   ///< ID for faces array buffer.

    GLuint vboptr6;   ///< ID for faces VBO.
    GLuint eboptr6;   ///< ID for faces index array.
    GLuint bufptr6;   ///< ID for faces array buffer.

    void LoadDataToGraphicsCard();

public:
    Cube();
    ~Cube();

    void draw(GLint whichface);
};

#endif // CUBE_H_INCLUDED
