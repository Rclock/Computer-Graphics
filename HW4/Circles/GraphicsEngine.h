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
#include "Circle.h"

#include "LoadShaders.h"

#include "ProgramDefines.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    GLuint NumVertices;
    int noCircles;
    GLint vPosition;    ///< Vertex data position, for the shader.
    GLint vColor;       ///< Color data position, for the shader.

// Arrays to hold buffer "addresses"
    GLuint VAO;       ///< Vertex Array Object
    GLuint Buffer;    ///< Array of Buffer addresses.
    std::vector<Circle*> Circles; ///< Vector of circles


    GLuint modelLoc;           ///< Location of the model matrix in the shader program.
    GLuint projLoc;                ///< Location of the projection matrix in the shader program.
    GLfloat ScreenBounds[4];   ///< Bounds of the screen.
    GLuint program;


    // GLint passColorLoc;   ///< Location for the new color matrix to pass to shader
    //GLint changecolorvalue;
    GLuint colorLoc; ///< Location of new color matrix to pass to shader. On graphics card.


    glm::mat4 ModelMatrix;     ///< Model Matrix

    sf::Clock animclock;   ///< Clock for animation.
public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();
    void init();
    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);
    void createCircles();
    int numCircles;
    void setProjectionMatrix();
    void print_GLM_Matrix(glm::mat4 m);


};

#endif // GRAPHICSENGINE_H_INCLUDED
