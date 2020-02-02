#include "GraphicsEngine.h"

/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Ryan Clocker
\version   1.0
\date      3/15/2018

*/

/**
\brief Constructor

\param title --- Title to be placed in the titlebar of the graphics window.
\param MajorVer --- The OpenGL major version that is requested.
\param MinorVer --- The OpenGL minor version that is requested.
\param width --- The width (in pixels) of the graphics window.
\param height --- The height (in pixels) of the graphics window.

Creates rendering window, loads the shaders, and sets some initial data settings.

*/

GraphicsEngine::GraphicsEngine(std::string title, GLint MajorVer, GLint MinorVer, int width, int height) :
    sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default,
                     sf::ContextSettings(24, 8, 4, MajorVer, MinorVer, sf::ContextSettings::Core))
{
    //  Set the number of boxes to draw.
    numBoxes = 1;

    glClearColor(0, 0, 0, 1);
    //glClearColor(1, 1, 1, 1);

    // Set the seed of the random number generator to the clock value.
    srand(clock());

    mode = GL_FILL;
    sscount = 1;

    if (SetVS)
        setVerticalSyncEnabled(true);
    else
        setFramerateLimit(0);

    program = LoadShadersFromFile("AspectRatioVert.glsl", "StrangeFrag.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Turn on the shader.
    glUseProgram(program);

    // Find the location of the projection matrix.
    projLoc = glGetUniformLocation(program, "Projection");
    //used for shader uniform option for image on screen
    optionLoc = glGetUniformLocation(program, "option");
    timeLoc = glGetUniformLocation(program, "time");
    setProjectionMatrix();

    animclock.restart();
    //elptime = animclock.getElapsedTime().asSeconds();


    // Make it the active window for OpenGL calls
    setActive();
    glClearColor(0, 0, 0, 1);
}

/**
\brief Destructor

Clear memory allocations

*/

GraphicsEngine::~GraphicsEngine()
{

}

/**
\brief The function responsible for drawing to the OpenGL frame buffer.

This function clears the screen and calls the draw functions of the box and circle.

*/

void GraphicsEngine::display()
{
    // Clear the screen (Frame Buffer)
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the objects.

    box.draw();
    glUniform1f(timeLoc, animclock.getElapsedTime().asSeconds());//sends time to graphics card

    sf::RenderWindow::display();

    printOpenGLErrors();
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
\brief Handles the resizing events of the window.

*/

void GraphicsEngine::resize()
{
    glViewport(0, 0, getSize().x, getSize().y);
    box.setWidth(getSize().x);
    box.setHeight(getSize().y);
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
\brief Returns a pointer to the box object.

*/

Box GraphicsEngine::getBox()
{


    return box;
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
\brief Prints all OpenGL errors to stderr.

*/

void GraphicsEngine::printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s\n", errString);
    }
}

/**
\brief Prints the glm matrix to the console window.  Remember that glm
matrices are column major.

\param m --- the glm matrix to be displayed.

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
\brief Calculates the aspect ratio projection matrix for the current screen size
and loads it into the shader.

The later portion of the method calculates the "screen coordinates" and displays
them along with the projection matrix to the console window.

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
/**
\brief This function takes care of sending the uniform int to the shader based on the user's input.
\param int option --- Passed in from UI to send to shader.

**/
void GraphicsEngine::setImage(int option)
{
//sends option chosen to the shader to act accordingly
    glUniform1i(optionLoc, option);
}
