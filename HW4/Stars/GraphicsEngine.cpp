#include "GraphicsEngine.h"
#include <math.h>
#include <cmath>
#include <vector>
/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Ryan Clocker
\version   1.0
\date      3/5/2019

*/

/**
\brief Constructor

\param title --- Title to be placed in the titlebar of the graphics window.
\param MajorVer --- The OpenGL major version that is requested.
\param MinorVer --- The OpenGL minor version that is requested.
\param width --- The width (in pixels) of the graphics window.
\param height --- The height (in pixels) of the graphics window.

Creates rendering window, enables GLEW, loads the shaders, and sets some initial
data settings.

*/


GraphicsEngine::GraphicsEngine(std::string title, GLint MajorVer, GLint MinorVer, int width, int height) :
    sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default,
                     sf::ContextSettings(24, 8, 4, MajorVer, MinorVer, sf::ContextSettings::Core))
{
    vPosition = 0;
    vColor = 1;
    //  Load the shaders
    GLuint program = LoadShadersFromFile("AspectRatioVert.glsl", "PassThroughFrag.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    mode = GL_FILL;
    sscount = 1;

    if (SetVS)
        setVerticalSyncEnabled(true);
    else
        setFramerateLimit(0);



    // Make it the active window for OpenGL calls
    setActive();

    glClearColor(0, 0, 0, 1);

    // Turn on the shader.
    glUseProgram(program);


    projLoc = glGetUniformLocation(program, "ProjectionMatrix");
    modelLoc = glGetUniformLocation(program, "ModelMatrix");

    colorLoc = glGetUniformLocation(program, "colors");
    setProjectionMatrix();

    animclock.restart();
    previoustime = animclock.getElapsedTime().asSeconds();

}

/**
 \brief Destructor

Currently empty, no allocated memory to clear.

*/

GraphicsEngine::~GraphicsEngine() {}

/**
 \brief The function responsible for drawing to the OpenGL frame buffer.

This function clears the screen and calls the draw functions of the stars.
This function also takes care of using the parallel vector of structs that holds data for each star.
All linear transformations are done here also and the model matrix is sent to the graphics engine.
Generating stars every tenth of a second is also taken care of here.

*/

void GraphicsEngine::display()
{

    GLint val;
    // Clear the screen (Frame Buffer)
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the objects.
    glLineWidth(1);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
    std::cout<< animclock.getElapsedTime().asSeconds();
    std::cout << std::endl;
    std::cout << Stars.size() << std::endl;

    if (animclock.getElapsedTime().asSeconds() - previoustime >= .1)
    {
        generateStar(animclock.getElapsedTime().asSeconds());
        previoustime = animclock.getElapsedTime().asSeconds();
    }


    for (int i = 0; i < Stars.size(); i++)
    {

        float elptime = animclock.getElapsedTime().asSeconds() - Data[i].starttime;

        glm::mat4 TranslateMat = glm::translate(glm::mat4(1.0), glm::vec3(Data[i].position[0], Data[i].position[1], 0));//translate by pos[] instead??
        glm::mat4 ScaleMat = glm::scale(glm::mat4(1.0), glm::vec3(Data[i].scaleFactor, Data[i].scaleFactor, 1));
        glm::mat4 RotateMat = glm::rotate(glm::mat4(1.0), Data[i].velocityRot*degf, glm::vec3(0, 0, 1));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TranslateMat*RotateMat*ScaleMat));


        Data[i].position[0] = Data[i].centerX + Data[i].velocity[0]*elptime;
        Data[i].position[1] = Data[i].centerY + Data[i].velocity[1]*elptime;


        Data[i].velocityRot = elptime*Data[i].initialRot;//problem

        // glUniform4fv(colorLoc, 1, glm::value_ptr(Data[i].colorVec));
        Stars[i]->draw();

    }
    //free memory and shrink vector for stars that are off the screen.
    for (int i = 0; i < Stars.size(); i++)
    {
        bool erasespot = false;
        if (Data[i].position[0] + Data[i].scaleFactor*1 <= ScreenBounds[0])
            erasespot = true;

        if (Data[i].position[0] - Data[i].scaleFactor*1 >= ScreenBounds[1])
            erasespot = true;

        if (Data[i].position[1] + Data[i].scaleFactor*1 <= ScreenBounds[2])
            erasespot = true;

        if (Data[i].position[1] - Data[i].scaleFactor*1 >= ScreenBounds[3])
            erasespot = true;

        if (erasespot)
        {
            delete Stars[i];//delete pointer first before deleting index slot in vector
            Stars.erase(Stars.begin() + i);

            Data.erase(Data.begin() + i);
        }
    }




    sf::RenderWindow::display();

}

/**
 \brief Changes the fill, line, and point mode being used.

*/

void GraphicsEngine::changeMode()
{
    if (mode == GL_FILL)
        mode = GL_LINE;
    else
        mode = GL_FILL;

    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

/**
 \brief Saves a screenshot of the current display to a file, ScreenShot###.png.

*/

void GraphicsEngine::screenshot()
{
    char ssfilename[100];
    sprintf(ssfilename, "ScreenShot%d.png", sscount);
    sf::Vector2u windowSize = getSize();
    sf::Texture texture;
    texture.create(windowSize.x, windowSize.y);
    texture.update(*this);
    sf::Image img = texture.copyToImage();
    img.saveToFile(ssfilename);
    sscount++;
}

/**
 \brief Handles the resizing events of the window.

*/

void GraphicsEngine::resize()
{
    glViewport(0, 0, getSize().x, getSize().y);
    setProjectionMatrix();
}

/**
\brief Sets the size of the rendering window.

\param width --- the width in pixels of the new size.

\param height --- the height in pixels of the new size.

*/

void GraphicsEngine::setSize(unsigned int width, unsigned int height)
{
    sf::RenderWindow::setSize(sf::Vector2u(width, height));
}
/**
 \brief
 This function generates one star with the random attributes discussed earlier (stored in the parallel data vector). Adds generated star
 into the graphics engine vector of stars to be drawn to the screen.

 \param time --- Current time from the animClock. Since each star has a different start time.
 */
void GraphicsEngine::generateStar(float time)
{

    starData newstar;

    newstar.starttime = time;
    srand(clock());
    newstar.position[0] = 2 * (double)rand()/RAND_MAX - 1;
    newstar.position[1] = 2 * (double)rand()/RAND_MAX - 1;
    newstar.centerX = newstar.position[0];
    newstar.centerY = newstar.position[1];
    //newstar.position[1] = 0;
    newstar.velocity[0] = 2 * (double)rand()/RAND_MAX - 1;
    newstar.velocity[1] = 2 * (double)rand()/RAND_MAX - 1;

    if (newstar.velocity[0] > .15)//limit speeds so theyre not so fast
        newstar.velocity[0] = .15;
    if (newstar.velocity[0] < -.15)
        newstar.velocity[0] = -.15;
    if (newstar.velocity[1] > .15)
        newstar.velocity[1] = .15;
    if (newstar.velocity[1] < -.15)
        newstar.velocity[1] = -.15;


    srand(clock());

    newstar.velocityRot =  rand() % 200 + 25;
    newstar.scaleFactor = (double)rand() / (double)RAND_MAX;
    if (newstar.scaleFactor < .1)//keep them within a reasonable size
        newstar.scaleFactor = .1;

    if (newstar.scaleFactor > .5)
        newstar.scaleFactor = .5;
    newstar.initialRot = newstar.velocityRot;

    //newstar.colorVec = glm::vec4((double)rand() / ((double)RAND_MAX), (double)rand() / ((double)RAND_MAX), (double)rand() / ((double)RAND_MAX), 1);
    Data.push_back(newstar);
    Stars.push_back(new Star());
}
/**
 \brief Prints the matrix passed to it. For now, program only prints out the projection matrix for the
asp ratio.
*/
void GraphicsEngine::print_GLM_Matrix(glm::mat4 m)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            printf("%7.2f", m[c][r]);

        std::cout << std::endl;
    }
    std::cout << std::endl;
}
/**

 \brief Takes care of the aspect ratio projection matrix, using screen bounds. This is mainly Dr. Spickler's work.

*/

void GraphicsEngine::setProjectionMatrix()
{
    glm::mat4 ProjectionMatrix = glm::mat4(1.0);

    if (getSize().x > getSize().y)
    {
        GLfloat aspratio = (GLfloat)getSize().x / getSize().y;
        ProjectionMatrix = glm::ortho(-aspratio, aspratio, -1.0f, 1.0f);
    }
    else
    {
        GLfloat aspratio = (GLfloat)getSize().y / getSize().x;
        ProjectionMatrix = glm::ortho(-1.0f, 1.0f, -aspratio, aspratio);
    }

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

    glm::mat4 inv = glm::inverse(ProjectionMatrix);
    glm::vec4 preImage = inv * glm::vec4(-1, 0, 0, 1);
    ScreenBounds[0] = preImage.x;
    preImage = inv * glm::vec4(1, 0, 0, 1);
    ScreenBounds[1] = preImage.x;
    preImage = inv * glm::vec4(0, -1, 0, 1);
    ScreenBounds[2] = preImage.y;
    preImage = inv * glm::vec4(0, 1, 0, 1);
    ScreenBounds[3] = preImage.y;

    std::cout << "[" << ScreenBounds[0] << ", " <<
              ScreenBounds[1] << "] X [" <<
              ScreenBounds[2] << ", " <<
              ScreenBounds[3] << "]" << std::endl;
    std::cout << std::endl;

    print_GLM_Matrix(ProjectionMatrix);
}

