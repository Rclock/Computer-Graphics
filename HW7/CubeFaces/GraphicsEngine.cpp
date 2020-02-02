#include "GraphicsEngine.h"

/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Ryan Clocker
\version   1.0
\date      4/26/2019

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
    pot.LoadDataToGraphicsCard(0, 2, 3);

    //  Load the shaders
    program = LoadShadersFromFile("VertexShaderLightingTexture.glsl", "PhongMultipleLightsAndTexture.glsl");

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
    texTransLoc = glGetUniformLocation(program, "textrans");

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

    glm::vec4 GlobalAmbient(0.2, 0.2, 0.2, 1);
    glUniform4fv(glGetUniformLocation(program, "GlobalAmbient"), 1, glm::value_ptr(GlobalAmbient));

    lightobj.createSphereOBJ(0.25, 7, 7);
    lightobj.LoadDataToGraphicsCard(0, 1, 2, 3);
    lightobj.setColor(1, 1, 0);
    lightobj.setDrawBorder(GL_TRUE);


    LtPos[0].setTheta(45);
    LtPos[0].setPsi(45);
    LtPos[0].setR(50);

    LtPos[1].setTheta(-45);
    LtPos[1].setPsi(100);
    LtPos[1].setR(50);

    LtPos[2].setTheta(-100);
    LtPos[2].setPsi(60);
    LtPos[2].setR(50);

    model = glm::scale(glm::mat4(1.0), glm::vec3(6, 6, 6));


    // Load model matrix.
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Extract upper left 3 X 3 from model matrix.
    glm::mat3 nM(model);

    // Create the Normal Transformation matrix N = (M^(-1))^T, where M is the
    // upper left 3 X 3 from model matrix.
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

//  Texture transformation matrix experiments
    textrans = glm::mat4(1.0);


    sf::Image texture;
    std::string filename;

    glGenTextures(6, texID);

    for (int i = 0; i < 6; i++)
    {
        if (i == 0)
            filename = "Textures/PE.png";
        else if (i == 1)
            filename = "Textures/AE.png";
        else if (i == 2)
            filename = "Textures/RD.png";
        else if (i == 3)
            filename = "Textures/SK.png";
        else if (i == 4)
            filename = "Textures/JV.png";
        else if (i == 5)
            filename = "Textures/AT.png";

        if (!texture.loadFromFile(filename))
            std::cerr << "Could not load texture: " << filename << std::endl;

        char arrayname[10];
        sprintf(arrayname, "tex[%d]", i);

        //  Link the texture to the shader.
        GLuint tex1_uniform_loc = glGetUniformLocation(program, arrayname);
        glUniform1i(tex1_uniform_loc, i);

        //  Load the texture into texture memory.
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, texID[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // Make it the active window for OpenGL calls, resize to set projection matrix.
    setActive();
    glClearColor(0, 0, 0, 1);
    resize();
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
    textrans = glm::mat4(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    for (int i = 0; i < 3; i++)
    {
        lt[i].setPosition(glm::vec4(LtPos[i].getPosition(), 1.0));
        lt[i].setSpotDirection(-LtPos[i].getPosition());
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
    turnTexturesOff("useTexture", 6);


    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));


    //begin drawing the faces of the cube one by one
    turnLightsOn("Lt", 3);


    turnTextureOn("useTexture", 1);

    //keep this face upright
    textrans = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

    if (texturesOff)
        turnTexturesOff("useTexture", 6);
    if (lightsoff)
        turnLightsOff("Lt", 3);

        //draw first VBO
    cube.draw(1);
    turnTextureOff("useTexture", 1);

    turnTextureOn("useTexture", 2);
    textrans = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    if (texturesOff)
        turnTexturesOff("useTexture", 6);
    if (lightsoff)
        turnLightsOff("Lt", 3);
    cube.draw(2);
    turnTextureOff("useTexture", 2);

    turnTextureOn("useTexture", 3);
    textrans = glm::rotate(glm::mat4(1.0), 180*degf, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    if (texturesOff)
        turnTexturesOff("useTexture", 6);
    if (lightsoff)
        turnLightsOff("Lt", 3);
    cube.draw(3);
    turnTextureOff("useTexture", 3);

    textrans = glm::mat4(1.0);
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

    turnTextureOn("useTexture", 4);
    if (texturesOff)
        turnTexturesOff("useTexture", 6);
    if (lightsoff)
        turnLightsOff("Lt", 3);
    cube.draw(4);
    turnTextureOff("useTexture", 4);


    textrans = glm::mat4(1.0);
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    turnTextureOn("useTexture", 5);
    if (texturesOff)
        turnTexturesOff("useTexture", 6);
    if (lightsoff)
        turnLightsOff("Lt", 3);
    cube.draw(5);
    turnTextureOff("useTexture", 5);

    textrans = glm::mat4(1.0);
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    turnTextureOn("useTexture", 6);
    if (texturesOff)
        turnTexturesOff("useTexture", 6);
    if (lightsoff)
        turnLightsOff("Lt", 3);
    cube.draw(6);
    turnTextureOff("useTexture", 6);



    turnLightsOff("Lt", 3);
    turnTexturesOff("useTexture", 6);

    for (int i = 0; i < 3; i++)
    {
        glm::vec3 ltpos;
        ltpos.x = lt[i].getPosition().x;
        ltpos.y = lt[i].getPosition().y;
        ltpos.z = lt[i].getPosition().z;

        glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*(glm::translate(glm::mat4(1.0), ltpos))));
        lightobj.draw();
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

\param num --- Number of lights to turn off.

*/

void GraphicsEngine::turnLightsOff(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnLightOff(name.c_str(), i);
}

/**
\brief Toggles the boolean to draw the axes or not.

*/

void GraphicsEngine::toggleDrawAxes()
{
    drawAxes = !drawAxes;
}

/**
\brief Turns the first num textures off.

\param name --- The name of the boolean texture array in the shader.

\param num --- Number of textures to turn off.

*/

void GraphicsEngine::turnTexturesOff(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnTextureOff(name.c_str(), i);
}

/**
\brief Turns the ith texture off.

\param name --- The name of the boolean texture array in the shader.

\param i --- Number of the texture to turn off.

*/

void GraphicsEngine::turnTextureOff(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d]", arrayname, i);
    glUniform1i(glGetUniformLocation(program, locID), false);
}

/**
\brief Turns the first num textures on.

\param name --- The name of the boolean texture array in the shader.

\param num --- Number of textures to turn on.

*/

void GraphicsEngine::turnTexturesOn(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnTextureOn(name.c_str(), i);
}

/**
\brief Turns the ith texture on.

\param name --- The name of the boolean texture array in the shader.

\param i --- Number of the texture to turn on.

*/


void GraphicsEngine::turnTextureOn(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d]", arrayname, i);
    glUniform1i(glGetUniformLocation(program, locID), true);
}


/**
\brief Turns the texture on if it's off and off if it's on.
*/
void GraphicsEngine::textureOff()
{
    if (!texturesOff)
        texturesOff = true;
    else
        texturesOff = false;
}
/**
\brief Turns the lighting on if it's off and off if it's on.
*/
void GraphicsEngine::lightOff()
{
    if (!lightsoff)
        lightsoff = true;
    else
        lightsoff = false;
}
