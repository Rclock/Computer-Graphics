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
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LoadShaders.h"
#include "ProgramDefines.h"
#include "SphericalCamera.h"
#include "YPRCamera.h"
#include "Axes.h"
#include "teapot.h"
#include "Material.h"
#include "Light.h"
#include "MaterialPresets.h"
#include "Models.h"
#include "ObjModel.h"
#include "TextRendererTTF.h"
#include <vector>

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
    GLenum mode;    ///< Mode, either point, line or fill.
    int sscount;    ///< Screenshot count to be appended to the screenshot filename.
    Axes coords;    ///< Axes Object
    teapot pot;     ///< Teapot Object

    GLuint ModelLoc;       ///< Location ID of the Model matrix in the shader.
    GLuint NormalLoc;      ///< Location ID of the Normal matrix in the shader.
    GLuint PVMLoc;         ///< Location ID of the PVM matrix in the shader.
    GLuint texTransLoc;    ///< Location ID of the texture transformation matrix in the shader.
    GLuint program;        ///< ID of the shader program.
    GLuint CMprogram;      ///< ID of the cube map shader program.

    SphericalCamera sphcamera;   ///< Spherical Camera
    YPRCamera yprcamera;         ///< Yaw-Pitch-Roll Camera
    int CameraNumber;            ///< Camera number 1 = spherical, 2 = yaw-pitch-roll.
    SphericalCamera LtPos[10];   ///< Spherical "Camera" to control position of the light.

    Material mat;         ///< Material of the current object.
    Light lt[10];         ///< Light object.
    GLuint texID[10];      ///< Texture IDs.
    GLuint CubeMapTexId;  ///< Cube Map Texture ID.

    Models lightobj;  ///< Used for sphere at light source.
    Models Planet;      ///< Model Object
    Models CMSphere;  ///< Sphere Object for Cube Map

    glm::mat4 projection;  ///< Projection Matrix
    glm::mat4 model;       ///< Model Matrix
    glm::mat4 textrans;    ///< Texture transformation matrix.
    glm::mat4 scalemat, rotatemat, transmat, cubemodel;
    glm::mat4 RotateOuter = glm::mat4(1.0);
    GLboolean drawAxes;    ///< Boolean for axes being drawn.
    ObjModel objmodel;    ///< Object wrapper for Wavefront obj file data.

    sf::Clock animclock, animclock2, orbitclock, soundclock, speedclock, moontime, asteroidclock, extraclock;
    float asteroidtime = 0;
    bool rs = false;
    float orbitstop;
    bool rotation = true;
    bool orbit = false;
    bool sound = false;
    bool info = true;
    float shipMaxVel = 7;
    float shipVelocity = 0;
    char words[300];
    int v = 1;

    bool controls = false;


    sf::Music music, music2;

    void printOpenGLErrors();
    void print_GLM_Matrix(glm::mat4 m);
    void print_GLM_Matrix(glm::mat3 m);
    glm::mat4 astmodel = glm::mat4(1.0);
    glm::mat4 asttex = glm::mat4(1.0);
    glm::mat4 astscale, astrot, asttrans;

    glm::mat4 view;
    float stoptime = 0;
    float elptime = 0;
    float elptime2 = 0;

    TextRendererTTF text;
    Models asteroid;
    float x, y, z;

    int lightsNo = 7;

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
    void setDrawAxes();

    GLboolean isSphericalCameraOn();
    void setSphericalCameraOn();
    GLboolean isYPRCameraOn();
    void setYPRCameraOn(int camera);

    void loadLight(Light Lt);
    void loadMaterial(Material Mat);

    void turnLightOn();
    void turnLightOff();
    void turnLightOn(std::string name, int i);
    void turnLightOff(std::string name, int i);
    void turnLightsOn(std::string name, int num);
    void turnLightsOff(std::string name, int num);

    void turnTexturesOff(std::string name, int num);
    void turnTexturesOn(std::string name, int num);
    void turnTextureOff(std::string name, int i);
    void turnTextureOn(std::string name, int i);

    void toggleDrawAxes();

    void LoadLights(Light Lt[], std::string name, int num);
    void LoadLight(Light Lt, std::string name, int i);
    void stopRotation();
    void setOrbit();
    YPRCamera* getShipYPR();



    SphericalCamera* getLtPos();
    SphericalCamera* getSphericalCamera();
    YPRCamera* getYPRCamera();

    void addShipVelocity(float amt);
    void setInfo();
    void stopShip();
    float upscaleTrail, viewscaleTrail, upscaleTight, viewscaleTight;


    void printFreeType();
    void drawAsteroid();
    void drawCubeMap();
    void updateCameraShipEyePV();
    void drawPlanets();
    void changeTexture();
    void setVS();
    void setSunLights();
    void showControls();



};

#endif // GRAPHICSENGINE_H_INCLUDED
