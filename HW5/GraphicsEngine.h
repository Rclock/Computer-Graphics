#ifndef GRAPHICSENGINE_H_INCLUDED
#define GRAPHICSENGINE_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
\date      3/15/2018

*/

/**
\class GraphicsEngine

\brief The GraphicsEngine class is an extension of sf::RenderWindow which
handles all of the graphics rendering in the program.

*/

class GraphicsEngine : public sf::RenderWindow
{
private:
    int numBoxes;    ///< Number of boxes to draw on the screen.
    GLenum mode;     ///< Mode, either point, line or fill.
    int sscount;     ///< Screenshot count to be appended to the screenshot filename.
    Box box;        ///< Array of box objects.

    GLuint projLoc;            ///< Location of the projection matrix in the shader program.
    GLuint optionLoc;           ///< Location of the uniform int option in shader for UI
    GLuint timeLoc;
    GLfloat ScreenBounds[4];   ///< Bounds of the screen.
    GLuint program;
    float elptime;
    sf::Clock animclock;        ///< Shader program ID

public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();

    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);
    Box getBox();
    void printOpenGLErrors();
    void setProjectionMatrix();
    void print_GLM_Matrix(glm::mat4 m);
    void setImage(int option);///< Used to send a uniform int to the shaders
};

#endif // GRAPHICSENGINE_H_INCLUDED
