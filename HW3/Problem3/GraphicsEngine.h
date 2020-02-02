#ifndef GRAPHICSENGINE_H_INCLUDED
#define GRAPHICSENGINE_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
using namespace std;

#include "LoadShaders.h"
#include "Box.h"
#include "ProgramDefines.h"

/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Ryan Clocker
\version   1.0
\date      2/26/2019

*/

/**
\class GraphicsEngine

\brief The GraphicsEngine class is an extension of sf::RenderWindow which
handles all of the graphics rendering in the program.

*/

class GraphicsEngine : public sf::RenderWindow
{
private:
    GLenum mode;     ///< Mode, either point, line or fill.
    int sscount;     ///< Screenshot count to be appended to the screenshot filename.
    vector<Box*> boxes;

public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();

    sf::Vector2f clickOrigin;
    void selectBox(GLfloat x, GLfloat y);///<Select a single box
    void checkRed(GLfloat x, GLfloat y);///<Check or make a box red
    void createBoxes(GLfloat cx, GLfloat cy, GLfloat w, GLfloat h);///<Create new boxes

    void compareToAllBoxesDelete(GLfloat x, GLfloat y);///<Delete a given box in those coords
    void compareToAllBoxesDrag(GLfloat x, GLfloat y);///<Drag a given box in those coords
    void dragSelectedBoxes(GLfloat x, GLfloat y);///<Drag all selected boxes
    void selectAll();///<Select every box
    void deselectAll();///<Deselect every box

    //make a void delete boxes, delete spot in vector and delete actual object. use new command to create new boxes
    bool compareToAllBoxes(GLfloat x, GLfloat y);///<Compares mouse coords to all boxes
    bool compareToAllSelectedBoxes(GLfloat x, GLfloat y);///<Compares mouse coords to all selected boxes
    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);



    void moveSelectedLeft();///<These move selected boxes in the 4 directions
    void moveSelectedRight();
    void moveSelectedUp();
    void moveSelectedDown();


    //increasing/decreasing H/w functions.
    void increaseWidth();///<These mess with widths and heights, of the selected boxes.
    void decreaseWidth();
    void increaseHeight();
    void decreaseHeight();


};

#endif // GRAPHICSENGINE_H_INCLUDED
