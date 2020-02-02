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
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include "Star.h"
#include "Circle.h"

#include "LoadShaders.h"

#include "ProgramDefines.h"
#include "Plane.h"
#include "Models.h"
#include "TextRendererTTF.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Ryan Clocker
\version   1.0
\date      5/14/2019

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
    GLuint VAO;       ///< Array of Vertex Array Object addresses.
    GLuint Buffer;    ///< Array of Buffer addresses.   ///< Vector of circles
    std::vector<Star*> Stars;
    Circle planet;
    GLuint texTransLoc;
    GLuint texID[12];
    GLuint useTextureLoc;  ///< Location ID of the texture use flag in the shader.
    GLuint modelLoc;           ///< Location of the model matrix in the shader program.
    GLuint projLoc;                ///< Location of the projection matrix in the shader program.
    GLfloat ScreenBounds[4];   ///< Bounds of the screen.
    GLuint program;
    int check;
    int t = 3;
    int u = 4;
    int v = 5;


    GLint passColorLoc;
    GLint changecolorvalue;
    GLuint colorLoc;
    GLuint PVMLoc;

     glm::mat4 projection;  ///< Projection Matrix
    glm::mat4 model;
    glm::mat4 textrans;



    bool flip = false;//for flip of direction
    bool flip2 = false;

    glm::mat4 ModelMatrix;     ///< Model Matrix

    sf::Clock animclock, animclock2, animclock3, elpclock, starclock, texclock, sunclock;   ///< Clock for animation.
     Plane plane;
     //Models sphere;

     void printOpenGLErrors();

    float previoustime = 0;
    bool rotation = true;
    float stoptime = 0;
    float stoptex = 0;
    glm::mat4 scalemat, rotateouter, transmat, texscale, texrot, trick;
    glm::mat4 Identity = glm::mat4(1.0);

    sf::Music music;
    TextRendererTTF text;
    bool drawtext = false;

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
    void generateStar(int starno, int shooting);
    void generateCircle();
    void flipRotation();
    void flipDirection();
    void setProjectionMatrix();
    void print_GLM_Matrix(glm::mat4 m);



    void turnTexturesOff(std::string name, int num);
    void turnTexturesOn(std::string name, int num);
    void turnTextureOff(std::string name, int i);
    void turnTextureOn(std::string name, int i);

    void stopRotation();
    void cycleTextures();
    void freezePlanets();
    void updateShootingStar();
    void drawPlanets();
    void drawFreeType();

};

#endif // GRAPHICSENGINE_H_INCLUDED
