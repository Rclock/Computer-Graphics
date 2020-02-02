#include "GraphicsEngine.h"

/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Ryan Clocker
\version   1.0
\date      3/27/2019

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
    //  Load the shaders
    GLuint program = LoadShadersFromFile("VertexShaderBasic3D.glsl", "PassThroughFrag.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Turn on the shader & get location of transformation matrix.
    glUseProgram(program);
    PVMLoc = glGetUniformLocation(program, "PVM");

    // Initialize some data.
    mode = GL_FILL;
    sscount = 1;
    CameraNumber = 1;
    drawAxes = GL_TRUE;
    drawManyBoxes = GL_TRUE;
    drawBoxes = GL_TRUE;

    // Set position of spherical camera
    sphcamera.setPosition(30, 30, 20);

    // Enable depth and cull face.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    if (SetVS)
    {
        setVerticalSyncEnabled(true);
        setFramerateLimit(200);
    }
    else
    {
        setVerticalSyncEnabled(false);
        setFramerateLimit(0);
    }

    // Make it the active window for OpenGL calls, resize to set projection matrix.
    setActive();
    glClearColor(0, 0, 0, 1);

    animclock.restart();


    ScaleMat = glm::mat4(1.0);

    resize();
}

/**
\brief Destructor

Currently empty, no allocated memory to clear.

*/

GraphicsEngine::~GraphicsEngine() {}

/**
\brief The function responsible for drawing to the OpenGL frame buffer.

This function does the majority of the work, taking care of all animating and drawing of the boxes.
It also takes care of setting cameras and axes. Lots of transformations are being drawn here.

*/

void GraphicsEngine::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set view matrix via current camera.
    glm::mat4 view(1.0);
    if (CameraNumber == 1)
        view = sphcamera.lookAt();
    else if (CameraNumber == 2)
        view = yprcamera.lookAt();

    // Set axes scaling.
    glm::mat4 axesscale = glm::scale(glm::mat4(1.0), glm::vec3(10, 10, 10));
    // glm::mat4 ScaleMat;



    // Load matrix product to shader.
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*axesscale));

    if (drawAxes)
        coords.draw();


    if (drawBoxes)
    {
        if (drawManyBoxes)
        {
            for (int i = -10; i <= 10; i+=4)
                for (int j = -10; j <= 10; j+=4)
                    for (int k = -10; k <= 10; k+=4)
                    {
                        model = glm::mat4(1.0);

                        float elptime;

                        //glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                        if (transNo == 1)
                        {
                            elptime = animclock.getElapsedTime().asSeconds() + stoptime;
                            if (animate)
                            {


                                RotateMat = glm::rotate(glm::mat4(1.0), 60*degf*elptime, glm::vec3(0, 0, 1));
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                model = TranslateMat*RotateMat;


                                // Load matrix product to shader.
                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                            else
                            {

                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                RotateMat = glm::rotate(glm::mat4(1.0), 60*degf*stoptime, glm::vec3(0, 0, 1));
                                model = TranslateMat*RotateMat;
                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                        }
                        else if (transNo == 2)
                        {

                            float elptime = animclock.getElapsedTime().asSeconds() + stoptime;

                            if (animate)
                            {
                                RotateMat = glm::rotate(glm::mat4(1.0), 60*degf*elptime, glm::vec3(0, 0, 1));
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                model = RotateMat*TranslateMat;//flip order of this to get #2.

                                // Load matrix product to shader.
                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                            else
                            {
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                RotateMat = glm::rotate(glm::mat4(1.0), 60*degf*stoptime, glm::vec3(0, 0, 1));
                                model = RotateMat*TranslateMat;
                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                        }
                        else if (transNo == 3)
                        {

                            float  time = animclock.getElapsedTime().asSeconds() + stoptime;//- previoustime;

                            if (animate)
                                ScaleMat = glm::scale(glm::mat4(1.0), glm::vec3(2.75*fabs(sin(time)), 2.75*fabs(sin(time)), 2.75*fabs(sin(time))));


                            std::cout << "Time:" << time << std::endl;
                            TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                            model = TranslateMat*ScaleMat;

                            glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

                            //old method, worked too hard to delete it. will not work anymore
                            /* if (time > 3)
                             {
                                 backward = true;
                                 animclock.restart();

                             }*/
                            //glm::mat4 RotateMat = glm::rotate(glm::mat4(1.0), 60*degf*elptime, glm::vec3(0, 0, 1));
                            /*if (!backward)
                            {
                                ScaleMat = glm::scale(glm::mat4(1.0), glm::vec3(time, time, time));
                            }*/
                            //else
                            //{


                            /* if (time > 2.9)
                             {
                                 backward = false;
                                 animclock.restart();
                                 std::cout << "Switched from backward \n";
                             }*/
                            //}




                        }


                        else if (transNo == 4)//works
                        {


                            float time = animclock.getElapsedTime().asSeconds() + stoptime;//- previoustime;


                            if (animate)
                            {

                                ScaleMat =  glm::scale(glm::mat4(1.0), glm::vec3(fabs(sin(time)), fabs(sin(time)), fabs(sin(time))));
                            }

                            TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                            //ScaleMat =  glm::scale(glm::mat4(1.0), glm::vec3(abs(sin(stoptime)), abs(sin(stoptime)), abs(sin(stoptime))));




                            //TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                            model = model*ScaleMat*TranslateMat;
                            glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));



                        }
                        else if (transNo == 5)
                        {

                            float time = animclock.getElapsedTime().asSeconds() + stoptime;

                            if (animate)
                            {
                                TranslateMat = glm::translate(model, glm::vec3(i*fabs(sin(5*time/3)), j, k));
                            }
                            else
                            {
                                TranslateMat = glm::translate(model, glm::vec3(i*fabs(sin(5*stoptime/3)), j, k));
                            }

                            model = model*TranslateMat;
                            glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));


                            /*if (time > 2.0)
                            {
                                backward2 = true;
                                animclock.restart();

                            }


                            if (!backward2)
                            {
                             time = animclock.getElapsedTime().asSeconds();
                                //float time = animclock.getElapsedTime().asSeconds()

                                TranslateMat = glm::translate(model, glm::vec3(i*time, j, k));

                                model = TranslateMat;
                                 //glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                            else
                            {

                                time = animclock.getElapsedTime().asSeconds();
                                //time = time*5;
                                TranslateMat = glm::translate(model, glm::vec3(i-i*time, j, k));
                                model = model*TranslateMat;
                                //glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                                if (time > 2)
                                {
                                    backward2 = false;
                                    animclock.restart();

                                }
                            }*/


                        }
                        else if (transNo == 6)
                        {


                            float time = animclock.getElapsedTime().asSeconds() + stoptime;



                            if (animate)
                            {
                                TranslateMat = glm::translate(model, glm::vec3(i*fabs(sin(4*time/3)), j*fabs(sin(4*time/3)), k*fabs(sin(4*time/3))));
                            }
                            else
                            {
                                TranslateMat = glm::translate(model, glm::vec3(i*fabs(sin(4*stoptime/3)), j*fabs(sin(4*stoptime/3)), k*fabs(sin(4*stoptime/3))));
                            }



                            model = model*TranslateMat;
                            glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            /* if (time > 1)
                             {
                                 animclock.restart();
                                 if (!backward4)
                                     backward4 = true;
                                 else
                                     backward4 = false;
                             }
                             if (!backward4)
                             {
                                 TranslateMat = glm::translate(model, glm::vec3(i*time, j*time, k*time));
                                 //ScaleMat =  glm::scale(glm::mat4(1.0), glm::vec3(time, time, time));
                             }
                             else
                             {
                                 TranslateMat = glm::translate(model, glm::vec3(i-i*time, j-j*time, k-k*time));
                                 //ScaleMat =  glm::scale(glm::mat4(1.0), glm::vec3(2-time, 2-time, 2-time));
                             }*/


                        }
                        else if (transNo == 7)
                        {

                            elptime = animclock.getElapsedTime().asSeconds() + stoptime;
                            if (animate)
                            {

                                RotateMat = glm::rotate(glm::mat4(1.0), -60*degf*elptime, glm::vec3(0, 0, 1));
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                RotateCounter = glm::rotate(glm::mat4(1.0), 60*degf*elptime, glm::vec3(0, 0, 1));
                                model = RotateMat*TranslateMat*RotateCounter;//flip order of this to get #2.
                                // Load matrix product to shader.
                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                            else
                            {
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                RotateMat = glm::rotate(glm::mat4(1.0), -60*degf*stoptime, glm::vec3(0, 0, 1));
                                RotateCounter = glm::rotate(glm::mat4(1.0), 60*degf*stoptime, glm::vec3(0, 0, 1));
                                model = RotateMat*TranslateMat*RotateCounter;//flip order of this to get #2.
                                // Load matrix product to shader.
                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }

                        }
                        else if (transNo == 8)
                        {
                            // glm::mat4 TranslateMat;
                            // glm::mat4 RotateOuter(1.0);

                            elptime = animclock.getElapsedTime().asSeconds() + stoptime;
                            if (animate)
                            {
                                RotateCounter = glm::rotate(glm::mat4(1.0), 60*degf*elptime, glm::vec3(1, 1, 1));
                                RotateOuter = glm::rotate(glm::mat4(1.0), -60*degf*elptime, glm::vec3(1, 1, 1));
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                model = RotateOuter*TranslateMat*RotateCounter;
                                //}



                                // Load matrix product to shader.
                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                            else
                            {
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                RotateCounter = glm::rotate(glm::mat4(1.0), 60*degf*stoptime, glm::vec3(1, 1, 1));
                                RotateOuter = glm::rotate(glm::mat4(1.0), -60*degf*stoptime, glm::vec3(1, 1, 1));
                                model = RotateOuter*TranslateMat*RotateCounter;
                                //}

                                // Load matrix product to shader.
                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                        }
                        else if (transNo == 9)
                        {

                            elptime = animclock.getElapsedTime().asSeconds() + stoptime;
                            //}

                            if (animate)
                            {

                                // Load matrix product to shader.
                                //glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                                RotateOuter = glm::rotate(glm::mat4(1.0), -60*degf*elptime, glm::vec3(i, j, k));
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                model = TranslateMat*RotateOuter;

                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                            }
                            else
                            {
                                TranslateMat = glm::translate(model, glm::vec3(i, j, k));
                                RotateOuter = glm::rotate(glm::mat4(1.0), -60*degf*stoptime, glm::vec3(i, j, k));
                                model = TranslateMat*RotateOuter;

                                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

                            }

                            //cool code


                            //}
                        }
                        else if (transNo == 10)//looks cool in wireframe
                        {
                            elptime = animclock.getElapsedTime().asSeconds() + stoptime;
                            if (animate)
                            {

                                RotateOuter = glm::rotate(glm::mat4(1.0), 60*degf*elptime, glm::vec3(0, 0, 1));
                                TranslateMat = glm::translate(model, glm::vec3(i*15*degf, 15*degf*j, 15*degf*k));
                            }
                            else
                            {
                                RotateOuter = glm::rotate(glm::mat4(1.0), 60*degf*stoptime, glm::vec3(0, 0, 1));
                                TranslateMat = glm::translate(model, glm::vec3(i*15*degf, 15*degf*j, 15*degf*k));
                            }
                            model = RotateOuter*TranslateMat;//*RotateInner;//flip order of this to get #2.
                            // Load matrix product to shader.

                            glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                        }
                        else //comment out this else to make a cool one
                        {
                            model = glm::translate(model, glm::vec3(i, j, k));
                            glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                        }

                        box.draw();


                    }
        }
        else
        {
            // Load matrix product to shader.
            glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view));
            box.draw();
        }
    }

    sf::RenderWindow::display();
    printOpenGLErrors();
}

/**
\brief Changes the fill and line mode being used.

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

Sets the viewport to the entire screen and recalculates the projection matrix.
*/

void GraphicsEngine::resize()
{
    glViewport(0, 0, getSize().x, getSize().y);
    projection = glm::perspective(50.0f*degf, (float)getSize().x/getSize().y, 0.01f, 500.0f);
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

Cube* GraphicsEngine::getBox()
{
    return &box;
}

/**
\brief Returns a pointer to the spherical camera.

*/

SphericalCamera* GraphicsEngine::getSphericalCamera()
{
    return &sphcamera;
}

/**
\brief Returns a pointer to the yaw-pitch-roll camera.

*/

YPRCamera* GraphicsEngine::getYPRCamera()
{
    return &yprcamera;
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
\brief Sets the boolean to draw a grid of boxes if true and a single box if false.

\param b --- Draws a grid of boxes if true and a single box if false.

*/

void GraphicsEngine::setDrawManyBoxes(GLboolean b)
{
    drawManyBoxes = b;
}

/**
\brief Sets the boolean to draw any boxes or not.

\param b --- Draws boxes if true and not if false.

*/

void GraphicsEngine::setDrawBoxes(GLboolean b)
{
    drawBoxes = b;
}

/**
\brief Returns true if the spherical camera is currently in use.

\return True if the spherical camera is currently in use and false otherwise.

*/

GLboolean GraphicsEngine::isSphericalCameraOn()
{
    return CameraNumber == 1;
}

/**
\brief Turns the spherical camera on.

*/

void GraphicsEngine::setSphericalCameraOn()
{
    CameraNumber = 1;
}

/**
\brief Returns true if the yaw-pitch-roll camera is currently in use.

\return True if the yaw-pitch-roll camera is currently in use and false otherwise.

*/

GLboolean GraphicsEngine::isYPRCameraOn()
{
    return CameraNumber == 2;
}

/**
\brief Turns the YPR camera on.

*/

void GraphicsEngine::setYPRCameraOn()
{
    CameraNumber = 2;
}

/**
 \brief Sets the boolean to draw the axes or not.

\param b --- Draws the axes if true and not if false.

*/

void GraphicsEngine::setDrawAxes(GLboolean b)
{
    drawAxes = b;
}
/**
\brief Sets the current transformation number from the UI.

\param trans --- Number of the new animation to be done.

*/
void GraphicsEngine::setTransNo(int trans)
{
    transNo = trans;
}
/**
 \brief Takes care of pressing S to turn the animations on and off. Works by resetting anim clock
if animations are turned back on and setting stop time when animations are turned off. Animations rely on
these two things often.


*/
void GraphicsEngine::anim()
{

    if (!animate)
    {
        animclock.restart();
        animate=true;
    }
    else
    {
        stoptime = stoptime + animclock.getElapsedTime().asSeconds();
        animate = false;
    }
}
