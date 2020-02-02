#include "GraphicsEngine.h"
#include <math.h>
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
    createCircles();
    animclock.restart();
}

/**
 \brief Destructor

Currently empty, no allocated memory to clear.

*/

GraphicsEngine::~GraphicsEngine() {}

/**
 \brief The function responsible for drawing to the OpenGL frame buffer.

This function is responsible for drawing the circles based on their positions. A lot of work is done here, color vec4 is sent
to the fragment shader, translation and scaling is done here and sent to the shaders, etc.

*/

void GraphicsEngine::display()
{
    GLint val;
    // Clear the screen (Frame Buffer)
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the objects.
    glLineWidth(1);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

    for (int i = 0; i < Circles.size(); i++)
    {
        GLfloat scale = Circles[i]->getScaleF();

        GLfloat pos[2];

        Circles[i]->getPosition(pos);

        glm::mat4 TranslateMat = glm::translate(glm::mat4(1.0), glm::vec3(pos[0], pos[1], 0));//translate by pos[] instead??
        glm::mat4 ScaleMat = glm::scale(glm::mat4(1.0), glm::vec3(scale, scale, 0));


        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TranslateMat*ScaleMat));

        Circles[i]->updatePosition(animclock.getElapsedTime().asSeconds(), ScreenBounds);



        glUniform4fv(colorLoc, 1, glm::value_ptr(Circles[i]->colorVec));//passes to vertex shader if bounces

        Circles[i]->draw();




    }


    std::cout << "Circles vector size: " << Circles.size() << std::endl;

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

 \brief Creates random number of circles and puts in vector.
 */
void GraphicsEngine::createCircles()
{
    srand(clock());
    numCircles = rand() % 50 + 10;
    for (int i = 0; i < numCircles; i++)
        Circles.push_back(new Circle());
}

/**

\brief Prints a given matrix, Dr. Spickler's work.
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
\brief Sets projection matrix to later be multiplied to the model matrix for aspect ratio.
Dr Spickler's work.
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

