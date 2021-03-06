#ifndef YPRCAMERA_H
#define YPRCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ProgramDefines.h"

/**
\file YPRCamera.h

\brief Header file for YPRCamera.cpp

\author    Don Spickler
\version   1.2
\date      1/14/2018

*/

/**
\class YPRCamera

\brief Creates a Yaw-Pitch-Roll camera.

*/

class YPRCamera
{
private:
    glm::vec3 Position; ///< Camera position.
    glm::vec3 View;     ///< Direction of the line of sight.
    glm::vec3 Up;       ///< Direction of up.

    float pitchdegree = 0;
    float yawdegree = 0;
    float rolldegree = 0;

public:
    YPRCamera();
    void PositionCamera(float X, float Y, float Z,
                        float vX, float vY, float vZ,
                        float upX, float upY, float upZ);
    void PositionCamera(glm::vec3 pos, glm::vec3 view, glm::vec3 up);

    void setPosition(float X, float Y, float Z);
    void setPosition(glm::vec3 pos);
    void setView(float X, float Y, float Z);
    void setView(glm::vec3 view);
    void setUp(float X, float Y, float Z);
    void setUp(glm::vec3 up);
    void setUpViaY();

    glm::vec3 getPosition();
    glm::vec3 getView();
    glm::vec3 getUp();
    glm::mat4 lookAt();
    glm::vec3 getYPRdegrees();

    void addPitch(float num);
    void addYaw(float num);
    void addRoll(float num);

    void moveForward(float num);
    void moveRight(float num);
    void moveUp(float num);

    glm::mat4 getWorldTransform();
};

#endif
