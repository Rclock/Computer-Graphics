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

\author    Ryan Clocker
\version   1.0
\date     2/26/2019

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
    GLfloat height;  ///< Height of the box.
    GLfloat centerX;      ///< X coordinate of the center of the box.
    GLfloat centerY;      ///< Y coordinate of the center of the box.
    GLfloat colors[12];   ///< Array holding the (r,g,b) color values for each of the 4 vertices, [r1, g1, b1, r2, g2, b2, ...]

    GLuint BoxVAO;        ///< Vertex Array Object ID.
    GLuint ArrayBuffer;   ///< Vertex and Color Information Array Buffer ID.
    GLuint BoxEBO;        ///< Indices Array Object ID.



    GLfloat colorsYellow[12]; ///<Array holding rgb yellow colors for box outline
    GLfloat yellowWidth;   ///< Width of yellow outline
    GLfloat yellowHeight;   ///< Height of yellow outline
    GLfloat YellowCenterX, YellowCenterY; ///< centers of yellow outline
    GLuint YellowVAO;  ///< New VAO for yellow outline
    GLuint ArrayBufferYellow;   ///< Vertex and Color Information for Yellow Array Buffer ID.
    GLuint BoxEBOYellow; ///<New EBO for yellow outline


    GLint vPosition;      ///< Array Buffer position index.
    GLint vColor;         ///< Array Buffer color index.

    void LoadDataToGraphicsCard();
    void LoadYellow();

public:
    //Box(GLfloat cx = 0, GLfloat cy = 0, GLfloat w = 1, GLfloat h = 1);
    Box(GLfloat cx, GLfloat cy, GLfloat w, GLfloat h);
    ~Box();
    bool selected = false;///<Is the box selected
    bool blue = false;
    bool green = false;
    bool regular = false;
    bool white = false;
    GLfloat r, g, b;
    void changeToRed(bool revert);///<Change box to red
    void setWidth(GLfloat w);///<Set box width
    void setHeight(GLfloat h);///<Set height
    void setYellowWidth(GLfloat w);///<Set yellow outline width
    void setYellowHeight(GLfloat h);///<Set yellow outline height
    void setCenter(GLfloat cx, GLfloat cy);
    void setYellowCenter(GLfloat cx, GLfloat cy);///<set yellow outline center
    void setVertexColor(GLint v, GLfloat r, GLfloat g,GLfloat b);
    GLfloat getWidth();
    GLfloat getHeight();
    GLfloat getYellowWidth();
    GLfloat getYellowHeight();
    void getCenter(GLfloat* c);
    void getYellowCenter(GLfloat *c);
    bool compareToBox(float x, float y);///<Compares given coords to the box
    void select();///<Selects the box
    void deselect();///<Deselects the box
    void draw();
    void drawYellow();///<draws yellow outline
    void dragSelectedBox(float differencex, float differencey);///<Drags a selected box

};

#endif // BOX_H_INCLUDED
