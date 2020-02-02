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

#include "LoadShaders.h"
#include "Box.h"
#include "ProgramDefines.h"

/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Ryan Clocker
\version   1.0
\date      2/25/2017

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
    Box box;         ///< Simple box object.

public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();

    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);
    Box* getBox();
};

#endif // GRAPHICSENGINE_H_INCLUDED
