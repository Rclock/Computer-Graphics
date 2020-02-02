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

    GLfloat velocity[2];
    GLfloat velocityRot;
    GLfloat velocityOuter;
    GLfloat initialOuterX;
    GLfloat initialOuterY;
    GLfloat initialRot;
    GLfloat position[2];
    GLfloat scaleFactor;
    GLfloat stoptime;
    GLfloat stoptime2;
    int shootingStar;
    int trailing;
    GLfloat centerX, centerY;
    glm::vec4 colorVec;

public:
    Star(int sun, int starno, int shooting);
    void flipRotation(float time);
    void flipDirection(float time);
    void updateDirection(float time, bool flip, float time3);
    void updateRotation(float time, bool flip);
    GLfloat getST();
    GLfloat getST2();
    GLfloat getPosX();
    GLfloat getPosY();
    GLfloat getScale();
    GLfloat getRot();
    GLfloat getOuter();

    GLfloat getVelocityX();
    GLfloat getVelocityY();
    GLfloat getInitialOuterX();
    GLfloat getInitialOuterY();
    GLfloat getCenterX();
    GLfloat getCenterY();

    int getShooting();
    void setInitialOuterX(float num);
    void setInitialOuterY(float num);
    void setInitialRot(float num);
    void setVelocityX(float num);
    void setVelocityY(float num);
    void setCenterX(float num);
    void setCenterY(float num);
    void setST(float num);
    void setST2(float num);
    void setPosX(float num);
    void setPosY(float num);
    void setScale(float num);
    void setVRot(float num);
    void setVOuter(float num);
    void setvInit(float num);
    void setvOuter(float num);
    ~Star();

    void draw();

};
#endif // BOX_H_INCLUDED
