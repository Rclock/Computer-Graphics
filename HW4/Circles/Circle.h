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
struct VertexData
{
    GLfloat color[3];    ///< 3-D color array for (r, g, b) colors.
    GLfloat position[4]; ///< 4-D position array for (x, y, z, w) coordinates.
};
class Circle
{
private:
    GLfloat radius;
    GLfloat noCircles;        ///< Width of the box.
    ///< Height of the box.
    GLfloat centerX;      ///< X coordinate of the center of the box.
    GLfloat centerY;      ///< Y coordinate of the center of the box.
    ///< Array holding the (r,g,b) color values for each of the 4 vertices, [r1, g1, b1, r2, g2, b2, ...]

    VertexData vertices[700];
    GLuint CircleVAO;        ///< Vertex Array Object ID.
    GLuint ArrayBuffer;   ///< Vertex and Color Information Array Buffer ID.
    GLuint CircleEBO;
    GLuint Buffer;       ///< Indices Array Object ID.


    GLint vPosition;      ///< Array Buffer position index.
    GLint vColor;

    GLfloat sb[4];      ///< Array Buffer color index.

    float starttime = 0;
    bool bounce = false;
    GLfloat speed[2];  ///< All of these four are used to scale and translate the circles. Record keeping of position/speed.
    GLfloat position[2];  ///< see above
    GLfloat scalingFactor; ///< see above

    GLfloat initialSpeed[2];///< see above



public:///< Doxygen for these can be found in their cpp file's doxygen.
    //Box(GLfloat cx = 0, GLfloat cy = 0, GLfloat w = 1, GLfloat h = 1);
    Circle();
    ~Circle();
    void setRadius(GLfloat r);///<Set box width
    void setCenter(GLfloat cx, GLfloat cy);


    GLfloat getRadius();
    void getCenter(GLfloat* c);
    void getPosition(GLfloat *c);

    GLint updatePosition(float time, GLfloat bounds[]);
    void checkBounce();
    void draw();
    GLfloat getScaleF();
    GLfloat previousTransX = 0;
    GLfloat previousTransY = 0;
    glm::vec4 colorVec;



};

#endif // BOX_H_INCLUDED
