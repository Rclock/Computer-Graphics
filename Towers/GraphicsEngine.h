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
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LoadShaders.h"
#include "ProgramDefines.h"
#include "SphericalCamera.h"
#include "YPRCamera.h"
#include "Axes.h"

#include "Material.h"
#include "Light.h"
#include "MaterialPresets.h"
#include "Models.h"
using namespace std;

/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Ryan Clocker
\version   1.0
\date      4/12/2019

*/

/**
\class GraphicsEngine

\brief The GraphicsEngine class is an extension of sf::RenderWindow which
handles all of the graphics rendering in the program.

*/

class GraphicsEngine : public sf::RenderWindow
{
private:
    GLenum mode;    ///< Mode, either point, line or fill.
    int sscount;    ///< Screenshot count to be appended to the screenshot filename.
    Axes coords;    ///< Axes Object
//    teapot pot;     ///< Teapot Object

    GLuint ModelLoc;   ///< Location ID of the Model matrix in the shader.
    GLuint NormalLoc;  ///< Location ID of the Normal matrix in the shader.
    GLuint PVMLoc;     ///< Location ID of the PVM matrix in the shader.
    GLuint program;    ///< ID of the shader program.

    SphericalCamera sphcamera;   ///< Spherical Camera
    YPRCamera yprcamera;         ///< Yaw-Pitch-Roll Camera
    int CameraNumber;            ///< Camera number 1 = spherical, 2 = yaw-pitch-roll.
    SphericalCamera LtPos[10];   ///< Spherical "Camera" to control position of the light.

    Material mat;   ///< Material of the current object.
    Light lt[3];   ///< Light object.

    Models lightobj;  ///< Used for sphere at light source.
    Models plane;     ///< Plane
    vector<Models*> objects;       ///< Model Object
    sf::Clock animclock;

    glm::mat4 projection;  ///< Projection Matrix
    glm::mat4 model;       ///< Model Matrix
    glm::mat4 TranslateMat;
    glm::mat4 RotateMat;
    glm::mat4 ScaleMat;
    glm::mat4 RotateOuter;
    glm::mat4 RotateCounter;
    GLboolean drawAxes;    ///< Boolean for axes being drawn.

    float speedfactor = 1.0;
    float starttime;
    bool changed = false;
    float factor;
    void printOpenGLErrors();
    void print_GLM_Matrix(glm::mat4 m);
    void print_GLM_Matrix(glm::mat3 m);

public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();

    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);
    GLfloat* getScreenBounds();
    void setDrawAxes(GLboolean b);

    GLboolean isSphericalCameraOn();
    void setSphericalCameraOn();
    GLboolean isYPRCameraOn();
    void setYPRCameraOn();

    void loadLight(Light Lt);
    void loadMaterial(Material Mat);

    void turnLightOn();
    void turnLightOff();
    void turnLightOn(std::string name, int i);
    void turnLightOff(std::string name, int i);
    void turnLightsOn(std::string name, int num);
    void turnLightsOff(std::string name, int num);

    void LoadLights(Light Lt[], std::string name, int num);
    void LoadLight(Light Lt, std::string name, int i);

    SphericalCamera* getLtPos();
    SphericalCamera* getSphericalCamera();
    YPRCamera* getYPRCamera();

    void increaseSpeed();
     void decreaseSpeed();
};

#endif // GRAPHICSENGINE_H_INCLUDED
