#include "GraphicsEngine.h"

/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Ryan Clocker
\version   1.0
\date      4/12/2019

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
//    pot.LoadDataToGraphicsCard(0, 2);

    //  Load the shaders
    program = LoadShadersFromFile("VertexShaderLighting.glsl", "PhongMultipleLights.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Turn on the shader & get location of transformation matrix.
    glUseProgram(program);
    PVMLoc = glGetUniformLocation(program, "PVM");
    ModelLoc = glGetUniformLocation(program, "Model");
    NormalLoc = glGetUniformLocation(program, "NormalMatrix");

    // Initialize some data.
    mode = GL_FILL;
    sscount = 1;
    CameraNumber = 1;
    drawAxes = GL_TRUE;

    // Set position of spherical camera
    sphcamera.setPosition(30, 30, 20);

    glEnable(GL_DEPTH_TEST);

    if (SetVS)
    {
        setVerticalSyncEnabled(true);
        setFramerateLimit(60);
    }
    else
    {
        setVerticalSyncEnabled(false);
        setFramerateLimit(0);
    }

    mat = Materials::bluePlastic;

    for (int i = 0; i < 3; i++)
        lt[i].setLight(true,
                       30.0, 30.0, 30.0, 1.0,
                       -1.0, -1.0, -1.0,
                       0.0, 0.0, 0.0, 1.0,
                       0.70, 0.70, 0.70, 1.0,
                       0.70, 0.70, 0.70, 1.0,
                       180.0, 0.0,
                       1.0, 0.0, 0.0
                      );

    LoadLights(lt, "Lt", 3);
    loadMaterial(mat);
    glUniform1i(glGetUniformLocation(program, "numLights"), 3);

    glm::vec4 GlobalAmbient(0.25, 0.25, 0.25, 1);
    glUniform4fv(glGetUniformLocation(program, "GlobalAmbient"), 1, glm::value_ptr(GlobalAmbient));

    lightobj.createSphereOBJ(0.25, 7, 7);
    lightobj.LoadDataToGraphicsCard(0, 1, 2);
    lightobj.setColor(1, 1, 0);
    lightobj.setDrawBorder(GL_TRUE);

    plane.createTessellatedWallOBJ(20, 20, 5, 5);
    plane.LoadDataToGraphicsCard(0, 1, 2);

    for (int i = 0; i < 6; i++)
    {
        objects.push_back(new Models());
        if ( i != 5)
        {
            objects[i]->createTorusOBJ(.5, 1, 25, 25);
            objects[i]->LoadDataToGraphicsCard(0, 1, 2);
        }
        else
        {
            objects[i]->createSphereOBJ(.4, 25, 25);
            objects[i]->LoadDataToGraphicsCard(0, 1, 2);
        }


    }


    LtPos[0].setR(20);
    LtPos[0].setTheta(45);
    LtPos[0].setPsi(45);


    LtPos[1].setR(20);
    LtPos[1].setTheta(100);
    LtPos[1].setPsi(-45);

    LtPos[2].setR(20);
    LtPos[2].setTheta(-100);
    LtPos[2].setPsi(60);



    // Make it the active window for OpenGL calls, resize to set projection matrix.
    setActive();
    resize();
    glClearColor(0, 0, 0, 1);
    factor = speedfactor;

    animclock.restart();


}

/**
\brief Destructor

Currently empty, no allocated memory to clear.

*/

GraphicsEngine::~GraphicsEngine() {}

/**
\brief The function responsible for drawing to the OpenGL frame buffer.

This function clears the screen and calls the draw functions of the box and circle.

*/

void GraphicsEngine::display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    for (int i = 0; i < 3; i++)
    {
        lt[i].setPosition(glm::vec4(LtPos[i].getPosition(), 1.0));
        lt[i].setSpotDirection(-LtPos[i].getPosition());
        // lt[i].setSpecular(1.0, 1.0, 1.0, 1.0);
    }
    LoadLights(lt, "Lt", 3);

    // Set view matrix via current camera.
    glm::mat4 view(1.0);
    if (CameraNumber == 1)
        view = sphcamera.lookAt();
    else if (CameraNumber == 2)
        view = yprcamera.lookAt();

    glm::vec3 eye;
    if (CameraNumber == 1)
        eye = sphcamera.getPosition();
    else if (CameraNumber == 2)
        eye = yprcamera.getPosition();

    glUniform3fv(glGetUniformLocation(program, "eye"), 1, glm::value_ptr(eye));

    turnLightsOff("Lt", 3);

    turnLightsOn("Lt", 3);
    mat = Materials::emerald;
    loadMaterial(mat);
    model=glm::mat4(1.0);
    model=glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    glm::mat3 nM(model);
    std::cout << "Model matrix upper 3x3" << std::endl;
    print_GLM_Matrix(nM);
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
    std::cout << "Normal matrix upper 3x3" << std::endl;
    print_GLM_Matrix(normalMatrix);
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));


    plane.draw();
    float time  = animclock.getElapsedTime().asSeconds();
    float elptime;

    for (int j = 0; j < 4; j++)//draws the towers
    {
        RotateOuter = glm::rotate(glm::mat4(1.0), 90*degf*j, glm::vec3(0, 1, 0));
        // cool oneRotateCounter = glm::rotate(glm::mat4(1.0), 90*degf*j, glm::vec3(0, 1, 0));

        for (int i = 0; i < 6; i++)
        {
            if (i == 5)
            {
                mat = Materials::silver;
                loadMaterial(mat);

                factor = speedfactor;
                if (i == 5 && time > 5/factor && time < 6/factor)
                {
                    elptime = animclock.getElapsedTime().asSeconds() - 5/factor;
                    mat.setEmission(fabs(sin(3*(elptime)*factor)), 0.0, 0.0, 1.0);
                }
                loadMaterial(mat);

                ScaleMat = glm::scale(glm::mat4(1.0), glm::vec3(3.5, 3.5, 3.5));
                RotateMat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
                TranslateMat = glm::translate(glm::mat4(1.0), glm::vec3(5, 6, 0));
                model = RotateOuter*TranslateMat*RotateMat*ScaleMat;
                glm::mat3 nM(model);
                glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
                glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
                objects[i]->draw();

                if (time > 6/speedfactor)
                {
                    animclock.restart();

                }
            }
            else
            {


                std::cout << "Time: " << time << std::endl;
                mat = Materials::bluePlastic;
                loadMaterial(mat);
                factor = speedfactor;

                if (i == 4 && time > 0/factor && time < 1/factor)
                {
                    elptime = animclock.getElapsedTime().asSeconds();

                    mat.setEmission(fabs(sin(3*(elptime)*factor)), 0.0, 0.0, 1.0);
                    //mat.setSpecular(animclock.getElapsedTime().asSeconds(), 0.6, 0.8, 1.0);
                }
                loadMaterial(mat);
                factor = speedfactor;

                if (i == 3 && time > 1/factor && time < 2/factor)
                {
                    elptime = animclock.getElapsedTime().asSeconds() - 1/factor;
                    mat.setEmission(fabs(sin(3*(elptime)*factor)), 0.0, 0.0, 1.0);
                    //mat.setSpecular(animclock.getElapsedTime().asSeconds(), 0.6, 0.8, 1.0);
                }
                loadMaterial(mat);
                factor = speedfactor;

                if (i == 2 && time > 2/factor && time < 3/factor)
                {
                    elptime = animclock.getElapsedTime().asSeconds() - 2/factor;
                    mat.setEmission(fabs(sin(3*(elptime)*factor)), 0.0, 0.0, 1.0);
                    //mat.setSpecular(animclock.getElapsedTime().asSeconds(), 0.6, 0.8, 1.0);
                }
                loadMaterial(mat);
                factor = speedfactor;

                if (i == 1 && time > 3/factor && time < 4/factor)
                {
                    elptime = animclock.getElapsedTime().asSeconds() - 3/factor;
                    mat.setEmission(fabs(sin(3*(elptime)*factor)), 0.0, 0.0, 1.0);
                    //mat.setSpecular(animclock.getElapsedTime().asSeconds(), 0.6, 0.8, 1.0);
                }
                loadMaterial(mat);
                factor = speedfactor;

                if (i == 0 && time > 4/factor && time < 5/factor)
                {
                    elptime = animclock.getElapsedTime().asSeconds() - 4/factor;
                    mat.setEmission(fabs(sin(3*(elptime)*factor)), 0.0, 0.0, 1.0);
                    //mat.setSpecular(animclock.getElapsedTime().asSeconds(), 0.6, 0.8, 1.0);
                }
                loadMaterial(mat);

                ScaleMat = glm::scale(glm::mat4(1.0), glm::vec3(2 + (i*.3), 2 + (i*.3), 2+(i*.3)));
                RotateMat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
                TranslateMat = glm::translate(glm::mat4(1.0), glm::vec3(5, 5-i, 0));
                model = RotateOuter*TranslateMat*RotateMat*ScaleMat;
                glm::mat3 nM(model);
                glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
                glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
                glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
                glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
                objects[i]->draw();
            }
        }
        //RotateOuter = glm::rotate(glm::mat4(1.0), 90*degf*j, glm::vec3(1, 0, 0));
        //glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*RotateOuter*model));
    }





    turnLightsOff("Lt", 3);

    for (int i = 0; i < 3; i++)
    {
        glm::vec3 ltpos;
        ltpos.x = lt[i].getPosition().x;
        ltpos.y = lt[i].getPosition().y;
        ltpos.z = lt[i].getPosition().z;

        //glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*(glm::translate(glm::mat4(1.0), ltpos))));
        //lightobj.draw();
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
\brief Returns a pointer to the spherical camera.

*/

SphericalCamera* GraphicsEngine::getSphericalCamera()
{
    return &sphcamera;
}

SphericalCamera* GraphicsEngine::getLtPos()
{
    return &LtPos[0];
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
matrices are column major. This is for the 3 X 3 matrices.

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
\brief Prints the glm matrix to the console window.  Remember that glm
matrices are column major. This is for the 3 X 3 matrices.

\param m --- the glm matrix to be displayed.

*/

void GraphicsEngine::print_GLM_Matrix(glm::mat3 m)
{
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
            printf("%7.2f", m[c][r]);

        std::cout << std::endl;
    }
    std::cout << std::endl;
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
\brief Turns the light on

*/

void GraphicsEngine::turnLightOn()
{
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "Lt.on"), true);
}

/**
\brief Turns the light off

*/

void GraphicsEngine::turnLightOff()
{
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "Lt.on"), false);
}

/**
\brief Loads the light structure to the shader light structure.

\param Lt --- Light structure to load.

*/

void GraphicsEngine::loadLight(Light Lt)
{
    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "Lt.on"), Lt.getOn());
    glUniform4fv(glGetUniformLocation(program, "Lt.position"), 1, glm::value_ptr(Lt.getPosition()));
    glUniform4fv(glGetUniformLocation(program, "Lt.ambient"), 1, glm::value_ptr(Lt.getAmbient()));
    glUniform4fv(glGetUniformLocation(program, "Lt.diffuse"), 1, glm::value_ptr(Lt.getDiffuse()));
    glUniform4fv(glGetUniformLocation(program, "Lt.specular"), 1, glm::value_ptr(Lt.getSpecular()));
    glUniform3fv(glGetUniformLocation(program, "Lt.spotDirection"), 1, glm::value_ptr(Lt.getSpotDirection()));
    glUniform3fv(glGetUniformLocation(program, "Lt.attenuation"), 1, glm::value_ptr(Lt.getAttenuation()));
    glUniform1f(glGetUniformLocation(program, "Lt.spotCutoff"), Lt.getSpotCutoff());
    glUniform1f(glGetUniformLocation(program, "Lt.spotExponent"), Lt.getSpotExponent());
}

/**
\brief Loads the material structure to the shader material structure.

\param Mat --- Material structure to load.

*/

void GraphicsEngine::loadMaterial(Material Mat)
{
    glUseProgram(program);

    glUniform4fv(glGetUniformLocation(program, "Mat.ambient"), 1, glm::value_ptr(Mat.getAmbient()));
    glUniform4fv(glGetUniformLocation(program, "Mat.diffuse"), 1, glm::value_ptr(Mat.getDiffuse()));
    glUniform4fv(glGetUniformLocation(program, "Mat.specular"), 1, glm::value_ptr(Mat.getSpecular()));
    glUniform4fv(glGetUniformLocation(program, "Mat.emission"), 1, glm::value_ptr(Mat.getEmission()));
    glUniform1f(glGetUniformLocation(program, "Mat.shininess"), Mat.getShininess());
}

/**
\brief Loads a single light into a light array in the shader.

\param Lt --- Light to load.

\param name --- The name of the array in the shader.

\param i --- The index of the light to load.

*/

void GraphicsEngine::LoadLight(Light Lt, std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), Lt.getOn());

    sprintf(locID, "%s[%d].%s", arrayname, i, "position");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getPosition()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "ambient");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getAmbient()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "diffuse");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getDiffuse()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "specular");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getSpecular()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotDirection");
    glUniform3fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getSpotDirection()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "attenuation");
    glUniform3fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getAttenuation()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotCutoff");
    glUniform1f(glGetUniformLocation(program, locID), Lt.getSpotCutoff());

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotExponent");
    glUniform1f(glGetUniformLocation(program, locID), Lt.getSpotExponent());
}

/**
\brief Loads the first num entries of a light array to the a light array in the shader.

\param Lt --- Light array to load.

\param name --- The name of the array in the shader.

\param i --- The number of lights to load.

*/

void GraphicsEngine::LoadLights(Light Lt[], std::string name, int num)
{
    for (int i = 0; i < num; i++)
        LoadLight(Lt[i], name.c_str(), i);
}

/**
\brief Turns the light at index i on.

\param name --- The name of the array in the shader.

\param i --- The index of the light to turn on.

*/

void GraphicsEngine::turnLightOn(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), true);
}

/**
\brief Turns the light at index i off.

\param name --- The name of the array in the shader.

\param i --- The index of the light to turn off.

*/

void GraphicsEngine::turnLightOff(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), false);
}

/**
\brief Turns the first num lights on.

\param name --- The name of the array in the shader.

\param num --- Number of lights to turn on.

*/

void GraphicsEngine::turnLightsOn(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnLightOn(name.c_str(), i);
}

/**
\brief Turns the first num lights off.

\param name --- The name of the array in the shader.

\param num --- Number of lights to turn on.

*/

void GraphicsEngine::turnLightsOff(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnLightOff(name.c_str(), i);
}

/**
\brief Function to increase speed. Slight fudge factor to match the need for a fudge factor in the decrease.

*/
void GraphicsEngine::increaseSpeed()
{

    if (speedfactor >= .58 && speedfactor <= 1)
        speedfactor += .01;
    else if (speedfactor > 1 && speedfactor < 6)
        speedfactor+= .1;
}
/**
\brief Function to decrease speed. Slight fudge factor.

*/
void GraphicsEngine::decreaseSpeed()
{

    if (speedfactor > .6 && speedfactor <= 1)
        speedfactor -= .01;
    else if (speedfactor > 1)
        speedfactor-= .1;


}
