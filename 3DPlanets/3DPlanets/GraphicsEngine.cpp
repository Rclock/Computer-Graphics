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

The constructor also sets things up for the cube map, all of the textures, and some lighting things.

*/

///> Used for formatting the X Y Z in freetype
int numDigits(int n)
{
    int digits = 0;

    while (n)
    {
        n /= 10;
        digits++;
    }

    return digits;
}
GraphicsEngine::GraphicsEngine(std::string title, GLint MajorVer, GLint MinorVer, int width, int height) :
    sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default,
                     sf::ContextSettings(24, 8, 4, MajorVer, MinorVer, sf::ContextSettings::Core))
{


    //  Load the shaders

    objmodel.Load("Models/Spaceship/", "untitled3.obj");


    float objectScale = 2;
    objmodel.setTextureRatio(0.0);
    program = LoadShadersFromFile("VertexShaderLightingTexture.glsl", "PhongMultipleLightsAndTexture.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    CMprogram = LoadShadersFromFile("VertexShaderCubeMap.glsl", "FragmentCubeMap.glsl");

    if (!CMprogram)
    {
        std::cerr << "Could not load Cube Map Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Turn on the shader & get location of transformation matrix.
    //objmodel.LoadPrograms();
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
    sphcamera.setPosition(140, 30, 20);

    glEnable(GL_DEPTH_TEST);

    if (SetVS)
        setVerticalSyncEnabled(true);
    else
        setFramerateLimit(0);

    //mat = Materials::redPlastic;
    mat = Materials::ruby;
    //mat = Materials::whitePlastic;

    for (int i = 0; i < 7; i++)
    {
        lt[i].setLight(true,
                       30.0, 30.0, 30.0, 1.0,
                       -1.0, -1.0, -1.0,
                       0.0, 0.0, 0.0, 1.0,
                       0.70, 0.7, 0.70, 1.0,
                       0.70, 0.7, 0.70, 1.0,
                       180.0, 0.0,
                       1.0, 0.0, 0.0
                      );
        if (i == 3)
        {
            lt[i].setLight(true,
                           0.0, 0.0, 0.0, 1.0,
                           0.0, 0.0, 1.0,//down x axis light
                           0.0, 0.0, 0.0, 1.0,
                           1.0, .55, 0.0, 1.0,
                           1.0, 0.0, 0.0, 1.0,
                           180.0, 0.0,
                           1.0, 0.007, 0.0
                          );

        }

        if (i == 4)
        {
            lt[i].setLight(true,
                           0.0, 0.0, 0.0, 1.0,
                           0.0, 0.0, -1.0,//down z axis light
                           0.0, 0.0, 0.0, 1.0,
                           1.0, .55, 0.0, 1.0,
                           1.0, 0.0, 0.0, 1.0,
                           180.0, 0.0,
                           1.0, 0.007, 0.0
                          );

        }
        if (i == 5)
        {
            lt[i].setLight(true,
                           0.0, 0.0, 0.0, 1.0,
                           -1.0, 0.0, 0.0,//up x axis light
                           0.0, 0.0, 0.0, 1.0,
                           1.0, .55, 0.0, 1.0,
                           1.0, 0.0, 0.0, 1.0,
                           180.0, 0.0,
                           1.0, 0.007, 0.0
                          );

        }
        if (i == 6)
        {
            lt[i].setLight(true,
                           0.0, 0.0, 0.0, 1.0,
                           1.0, 0.0, 0.0,
                           0.0, 0.0, 0.0, 1.0,
                           1.0, .55, 0.0, 1.0,
                           1.0, 0.0, 0.0, 1.0,
                           180.0, 0.0,
                           1.0, 0.007, 0.0
                          );

        }
        /* if (i == 7)
         {
             lt[i].setLight(true,
                            0.0, 0.0, 0.0, 1.0,
                            0.0, 1.0, 0.0,//up y axis light
                            0.0, 0.0, 0.0, 1.0,
                            1.0, .5, 0.0, 1.0,
                            1.0, .5, 0.0, 1.0,
                            180.0, 0.0,
                            1.0, 0.007, 0.0
                           );

         }
          if (i == 8)
         {
             lt[i].setLight(true,
                            0.0, 0.0, 0.0, 1.0,
                            0.0, -1.0, 0.0,//down y axis light
                            0.0, 0.0, 0.0, 1.0,
                            1.0, .5, 0.0, 1.0,
                            1.0, .5, 0.0, 1.0,
                            180.0, 0.0,
                            1.0, 0.007, 0.0
                           );

         }*/

    }
    LoadLights(lt, "Lt", 7);
    loadMaterial(mat);
    glUniform1i(glGetUniformLocation(program, "numLights"), 7);

    glm::vec4 GlobalAmbient(.5, .5, .5, 1);
    //objmodel.setGlobalAmbient(GlobalAmbient);

    glUniform4fv(glGetUniformLocation(program, "GlobalAmbient"), 1, glm::value_ptr(GlobalAmbient));

    lightobj.createSphereOBJ(0.25, 7, 7);
    lightobj.LoadDataToGraphicsCard(0, 1, 2, 3);
    lightobj.setColor(1, 1, 0);
    lightobj.setDrawBorder(GL_TRUE);

    CMSphere.createSphereOBJ(150, 20, 20);
    CMSphere.LoadDataToGraphicsCard(0, 1, 2, 3);


    Planet.createSphereOBJ(2, 55, 55);

    Planet.LoadDataToGraphicsCard(0, 1, 2, 3);

    LtPos[0].setTheta(45);
    LtPos[0].setPsi(45);
    LtPos[0].setR(5000);

    LtPos[1].setTheta(100);
    LtPos[1].setPsi(-45);
    LtPos[1].setR(5000);

    LtPos[2].setTheta(-100);
    LtPos[2].setPsi(60);
    LtPos[2].setR(5000);

    LtPos[3].setTheta(0);
    LtPos[3].setPsi(0);
    LtPos[3].setR(0);

    LtPos[4].setTheta(0);
    LtPos[4].setPsi(0);
    LtPos[4].setR(0);

    LtPos[5].setTheta(0);
    LtPos[5].setPsi(0);
    LtPos[5].setR(0);

    LtPos[6].setTheta(0);
    LtPos[6].setPsi(0);
    LtPos[6].setR(0);







    sf::Image texture;
    std::string filename;

    glGenTextures(12, texID);

    for (int i = 0; i < 12; i++)
    {
        if (i == 0)
            filename = "";
        else if (i == 1)
            filename = "Textures/PurpleGas.png";
        else if (i == 2)
            filename = "Textures/Asteroid.jpg";
        else if (i == 3)
            filename = "Textures/Sun.jpg";//sun
        else if (i == 4)
            filename = "Textures/BlueGas.png";
        else if (i == 5)
            filename = "Textures/Eerie.jpg";
        else if (i == 6)
            filename = "Textures/Ice.jpg";
        else if (i == 7)
            filename = "Textures/Alien.png";
        else if (i == 8)
            filename = "Textures/Water.jpg";
        else if (i == 9)
            filename = "Textures/Fline.jpg";
        else if (i == 10)
            filename = "Textures/Pline.jpeg";
        else if (i == 11)
            filename = "Textures/Grass.png";

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

    glUseProgram(CMprogram);

    //  Load in Cube Map
    glActiveTexture(GL_TEXTURE0+10);
    glUniform1i(glGetUniformLocation(CMprogram, "cmtex"), 10);

    // Generate a new cube map texture and bind to it
    glGenTextures (1, &CubeMapTexId);
    glBindTexture (GL_TEXTURE_CUBE_MAP, CubeMapTexId);

    // Setup some parameters for texture filters and mipmapping
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    //
    if (!texture.loadFromFile("Textures/Starfield002.jpg"))
        std::cerr << "Could not load texture: Starfield002.jpg" << std::endl;

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    //

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    animclock.restart();
    animclock2.restart();
    orbitclock.restart();
    // Make it the active window for OpenGL calls, resize to set projection matrix.
    setActive();
    glClearColor(0, 0, 0, 1);

    soundclock.restart();
    if (!music.openFromFile("SpaceShipAmplified.wav"))
        std::cout << "Couldn't open wav" << std::endl;// error

    if (!music2.openFromFile("Asteroid.wav"))
        std::cout << "Couldn't open wav" << std::endl;// error

    asteroidclock.restart();
    extraclock.restart();


    upscaleTight = 1.75;
    viewscaleTight = 2.2;
    upscaleTrail = 4;
    viewscaleTrail = 15;
//    rotation = true;
    asteroidtime = 10 * (double)rand() / (double)RAND_MAX + 5;
    asteroid.createSphereOBJ(2, 5, 5);
    asteroid.LoadDataToGraphicsCard(0, 1, 2, 3);
    x = 2000 * (double)rand() / (double)RAND_MAX - 1000;
    y = 2000 * (double)rand() / (double)RAND_MAX - 1000;
    z = 2000 * (double)rand() / (double)RAND_MAX - 1000;

    resize();

    text.loadFont("fonts/Px437_TandyNew_Mono.ttf");
    //text.loadFont("fonts/times.ttf");
    //text.loadFont("fonts/segoesc.ttf");
    text.setFontSize(23);
    text.setColor(0, 0, 1, .95);
    text.setScreenSize(getSize().x, getSize().y);
    music.setLoop(true);
    music2.setVolume(20);

}


/**
\brief Destructor

Currently empty, no allocated memory to clear.

*/

GraphicsEngine::~GraphicsEngine() {}

/**
\brief The function responsible for drawing to the OpenGL frame buffer.

The display function draws all of the planets, the spaceship in it's current orientation, all of the lights, performs all transformations,
plays sounds, and puts on proper textures. It also takes care of updating all of these things. It also prints all of the freetype.

*/

void GraphicsEngine::display()
{

    glPolygonMode(GL_FRONT_AND_BACK, mode);//M key wasn't working so I added this here.
    turnLightsOn("Lt", lightsNo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    elptime = animclock.getElapsedTime().asSeconds();
    elptime2 = animclock2.getElapsedTime().asSeconds();

    ///> Draw the space background
    drawCubeMap();


    // Set view matrix via current camera.
    //glm::mat4 view(1.0);
    glm::mat4 RotateOuter(1.0);

    if (CameraNumber == 1)
        view = sphcamera.lookAt();


    glUseProgram(program);

    for (int i = 0; i < lightsNo; i++)
    {
        lt[i].setPosition(glm::vec4(LtPos[i].getPosition(), 1.0));
        lt[i].setSpotDirection(-LtPos[i].getPosition());
        lt[i].setOn(true);
    }
    LoadLights(lt, "Lt", lightsNo);

    for (int i = 0; i < lightsNo; i++)
        objmodel.setLight(i, lt[i]);

    objmodel.setNumberOfLights(lightsNo);


    ///> Update orientation of ship and user's view of it.
    updateCameraShipEyePV();//updates which camera, the ship's position and viewing vector



    if (orbit)
    {
        float t = orbitclock.getElapsedTime().asSeconds();
        t = t*7;
        objmodel.setModelMatrix(glm::rotate(glm::mat4(1.0), t*degf, glm::vec3(0, 1, 0))*objmodel.shipypr.getWorldTransform()*glm::rotate(glm::mat4(1.0), -90*degf, glm::vec3(0, 1, 0)));
    }
    else//this line moves the ship to it's correct orientation by communicating with it's YPR camera
        objmodel.setModelMatrix(objmodel.shipypr.getWorldTransform()*glm::rotate(glm::mat4(1.0), -90*degf, glm::vec3(0, 1, 0)));

    ///>Draws the ship.
    objmodel.draw();

    turnTexturesOff("useTexture", 12);

    ///> Draws all of the planets and their transformations/textures.
    drawPlanets();

    ///> Draws the asteroid and deals with it's random position and time before it expires
    float asteroidelp = asteroidclock.getElapsedTime().asSeconds();
    if (asteroidelp > asteroidtime)
    {
        asteroidtime = 20 * (double)rand() / (double)RAND_MAX + 20;
        asteroidclock.restart();
        x = 1000 * (double)rand() / (double)RAND_MAX - 500;
        y = 1000* (double)rand() / (double)RAND_MAX - 500;
        z = 1000 * (double)rand() / (double)RAND_MAX - 500;
        music2.play();

    }

    asteroidelp = asteroidelp - 35.0;
    astscale = glm::scale(glm::mat4(1.0), glm::vec3(100, 100, 100));
    astrot = glm::rotate(glm::mat4(1.0), 100*degf*asteroidelp, glm::vec3(0,1,0));
    asttrans = glm::translate(glm::mat4(1.0), glm::vec3(-1*((x*asteroidelp)+x), -1*((y*asteroidelp)+y), -1*((z*asteroidelp)+z)));
    //std::cout << "Asteroid location: " << x*asteroidelp << ", " << y*asteroidelp << ", " << z*asteroidelp << std::endl;
    astmodel = asttrans*astscale*astrot;
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(astmodel));
    glm::mat3 nMast(astmodel);

    // Create the Normal Transformation matrix N = (M^(-1))^T, where M is the
    // upper left 3 X 3 from model matrix.
    glm::mat3 normalMatrixast = glm::transpose(glm::inverse(nMast));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrixast));


    asttex = glm::translate(glm::mat4(1.0), glm::vec3(elptime*3, 0, 0));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(asttex));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*astmodel));
    turnTextureOn("useTexture", 2);
    asteroid.draw();
    turnTextureOff("useTexture", 2);


    ///> Draw the axes
    turnLightsOff("Lt", lightsNo);
    glm::mat4 axesscale = glm::scale(glm::mat4(1.0), glm::vec3(25000, 25000, 25000));

    // Load matrix product to shader.
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*axesscale));
    if (drawAxes)
        coords.draw();

    turnLightsOn("Lt", lightsNo);


    turnTexturesOff("useTexture", 12);

    for (int i = 0; i < lightsNo; i++)
    {
        glm::vec3 ltpos;
        ltpos.x = lt[i].getPosition().x;
        ltpos.y = lt[i].getPosition().y;
        ltpos.z = lt[i].getPosition().z;

        glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*(glm::translate(glm::mat4(1.0), ltpos))));
        //lightobj.draw();
    }




    ///> Print all of the freetype that is required with current settings.
    printFreeType();
    turnLightsOff("Lt", lightsNo);

    glUseProgram(program);
    sf::RenderWindow::display();
    printOpenGLErrors();
    resize();

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
    projection = glm::perspective(50.0f*degf, (float)getSize().x/getSize().y, 0.01f, 90000.0f);
}

/**
\brief Sets the size of the rendering window.

\param width --- the width in pixels of the new size.

\param height --- the height in pixels of the new size.

*/

void GraphicsEngine::setSize(unsigned int width, unsigned int height)
{
    sf::RenderWindow::setSize(sf::Vector2u(width, height));
    resize();
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
    music.pause();
    sound = false;
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
\brief Turns the ship camera on.

*/

void GraphicsEngine::setYPRCameraOn(int camera)
{
    if (sound == false)
    {
        music.play();
        sound = true;
    }
    else
    {
        if (camera == 4)
            music.setVolume(50);
        else
            music.setVolume(100);

    }

    CameraNumber = camera;

}

/**
\brief Sets the boolean to draw the axes or not.

\param b --- Draws the axes if true and not if false.

*/

void GraphicsEngine::setDrawAxes()
{
    if (drawAxes)
        drawAxes = false;
    else
        drawAxes = true;
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
 \brief
 freezes the planets where they currently are. Resumes when called after that.
 */
void GraphicsEngine::stopRotation()
{
    if (rotation)
    {
        rotation = false;
        stoptime = animclock.getElapsedTime().asSeconds()+stoptime;

    }
    else
    {
        animclock.restart();
        rotation=true;
    }
}
/**
 \brief
 Returns pointer to the ship's camera that it is controlled by.
 */
YPRCamera* GraphicsEngine::getShipYPR()
{
    return &objmodel.shipypr;
}
/**
 \brief
 Puts the ship in orbit.
 */
void GraphicsEngine::setOrbit()
{
    if (orbit)
    {
        orbitclock.restart();
        orbit=false;
    }
    else
    {
        orbitclock.restart();
        orbit=true;
    }
}
/**
\brief
Adds or subtracts velocity to the ship. Makes sure it does not go over 100%.
\param amt --- The amount to add.
*/
void GraphicsEngine::addShipVelocity(float amt)
{
    shipVelocity += amt;
    if (amt > 0 && shipVelocity <= 100)
        viewscaleTrail+= .01;
    else if (amt < 0 && shipVelocity >= -100)
        viewscaleTrail-= .01;

    if (shipVelocity >= 100)//keep it under or equal to 100% power
        shipVelocity = 100;

    if (shipVelocity <= -100)
        shipVelocity = -100;


}
/**
 \brief
 Stops the ship no matter it's current velocity. Useful for when you are going way too fast to stop infront of something you are closer to than you thought.

 */
void GraphicsEngine::stopShip()
{
    viewscaleTrail = 15;
    viewscaleTight = 2.25;
    shipVelocity=0;// shipVelocity;
    speedclock.restart();
}
/**
 \brief
 Changes whether or not you want to see the ship's flight info or not.
 */
void GraphicsEngine::setInfo()
{
    if (info)
        info = false;
    else
        info = true;
}
/**
 \brief
 Prints all of the freetype for the whole program including ship flight info and controls menu. Works off of the booleans that are set for those.
 Takes care of formatting the X Y Z letters for the ship's position and flight direction so they stay lined up with the numbers.
*/
void GraphicsEngine::printFreeType()
{
    glm::vec3 xyz = objmodel.shipypr.getPosition();
    if (info && CameraNumber > 1)
    {
        if(!controls)
        {


            char words1[64];

            sprintf(words1, "Power:%.2f%c", shipVelocity, '%');

            text.setFontSize(16);
            text.setColor(1.0, 0.0, 0.0, .9);
            text.draw(words1, 10, 450, 0);

            glm::vec3 yprdegrees = objmodel.shipypr.getYPRdegrees();

            text.setFontSize(17);
            sprintf(words1, "%s", "Ship Orientation");
            text.setColor(1, 1, 1, .9);
            text.draw(words1, 530, 475, 0);

            sprintf(words1, "%s", "              Ship Activity");

            text.setFontSize(16);
            text.draw(words1, 10, 475, 0);

            text.setColor(0, 0, 1, .9);
            sprintf(words1, "Yaw: %.1f %s", yprdegrees.x, "Degrees");
            text.draw(words1, 520, 455, 0);
            sprintf(words1, "Pitch: %.1f %s", yprdegrees.y, "Degrees");
            text.draw(words1, 520, 435, 0);
            sprintf(words1, "Roll: %.1f %s", yprdegrees.z, "Degrees");
            text.draw(words1, 520, 415, 0);

            text.setFontSize(16);

            glm::vec3 flydirec = objmodel.shipypr.getView();

            text.setFontSize(16);

            sprintf(words1, "%s", "X");
            float offset = 5;

            int digitsx = numDigits((int)flydirec.x);
            int digits = digitsx;
            if (flydirec.x < 0)
                digits++;

            //std::cout << "Dig: "<< digits << std::endl;
            text.draw(words1, 180+(digits*offset), 435, 0);


            //begin working on reformatting Y and Z letters above their values.
            offset = 5;
            sprintf(words1, "%s", "Y");
            digitsx = numDigits((int)flydirec.x);
            int digitsy = numDigits((int)flydirec.y);
            digits = digitsx + digitsy;
            if (flydirec.x < 0)
                digits++;
            if (flydirec.y < 0)
                digits++;

            //std::cout << "Dig: "<< digits << std::endl;
            text.draw(words1, 240+(digits*offset), 435, 0);

            digits = 0;
            digitsx = 0;
            digitsy = 0;

            offset = 4;
            sprintf(words1, "%s", "Z");
            digitsx = numDigits((int)flydirec.x);
            digitsy = numDigits((int)flydirec.y);
            int digitsz = numDigits((int)flydirec.z);
            digits = digitsx + digitsy + digitsz;

            if (flydirec.x < 0)
                digits++;
            if (flydirec.y < 0)
                digits++;
            if (flydirec.z < 0)
                digits++;

            //std::cout << "Dig: "<< digits << std::endl;
            text.draw(words1, 295+(digits*offset), 435, 0);


            sprintf(words1, "Flight Direction:[%.1f] [%.1f] [%.1f]", flydirec.x, flydirec.y, flydirec.z);
            text.draw(words1, 10, 420, 0);

            //draw the X Y and Z, seperately so you can reposition them as those values grow



            sprintf(words1, "%s", "X");
            offset = 2;

            digitsx = numDigits((int)xyz.x);
            digits = digitsx-2;
            if (xyz.x < 0)
                digits++;

            //std::cout << "Dig: "<< digits << std::endl;
            text.draw(words1, 180+(digits*offset), 395, 0);


            //begin working on reformatting Y and Z letters above their values.
            offset = 4;
            sprintf(words1, "%s", "Y");
            digitsx = numDigits((int)xyz.x);
            digitsy = numDigits((int)xyz.y);
            digits = digitsx + digitsy-4;
            if (xyz.x < 0)
                digits++;
            if (xyz.y < 0)
                digits++;

            //std::cout << "Dig: "<< digits << std::endl;
            text.draw(words1, 245+(digits*offset), 395, 0);

            digits = 0;
            digitsx = 0;
            digitsy = 0;

            offset = 5;
            sprintf(words1, "%s", "Z");
            digitsx = numDigits((int)xyz.x);
            digitsy = numDigits((int)xyz.y);
            digitsz = numDigits((int)xyz.z);
            digits = digitsx + digitsy + digitsz - 6;

            if (xyz.x < 0)
                digits++;
            if (xyz.y < 0)
                digits++;
            if (xyz.z < 0)
                digits++;
            if (xyz.x > 1000 && xyz.y > 100)
                digits+=2;

            //std::cout << "Dig: "<< digits << std::endl;
            text.draw(words1, 300+(digits*offset), 395, 0);



            sprintf(words1, "Ship Position:  [%.1f] [%.1f] [%.1f]", xyz.x, xyz.y, xyz.z);
            text.setFontSize(16);
            text.draw(words1, 10, 380, 0);
        }
        else
        {
            char words2[64];
            sprintf(words2, "%s", "Controls");

            text.setFontSize(23);
            text.draw(words2, 295, 480, 0);

            sprintf(words2, "%s", "A: Show Axes");

            text.setFontSize(20);
            text.draw(words2, 10, 450, 0);

            sprintf(words2, "%s", "C: Show Controls");

            text.setFontSize(20);
            text.draw(words2, 10, 435, 0);

            sprintf(words2, "%s", "F: Freeze Planets");

            text.setFontSize(20);
            text.draw(words2, 10, 420, 0);

            sprintf(words2, "%s", "I: Display Ship Info");

            text.setFontSize(20);
            text.draw(words2, 10, 405, 0);

            sprintf(words2, "%s", "L: Turn Sun Lights On/Off");

            text.setFontSize(20);
            text.draw(words2, 10, 390, 0);

            sprintf(words2, "%s", "M: Wireframe mode");

            text.setFontSize(20);
            text.draw(words2, 10, 375, 0);

            sprintf(words2, "%s", "O: Orbit Ship around Sun");

            text.setFontSize(20);
            text.draw(words2, 10, 360, 0);

            sprintf(words2, "%s", "S: Stop Ship");

            text.setFontSize(20);
            text.draw(words2, 10, 345, 0);

            sprintf(words2, "%s", "T: Cycle Textures (Fly around to find where)");

            text.setFontSize(20);
            text.draw(words2, 10, 330, 0);

            sprintf(words2, "%s", "Ship Controls");

            text.setFontSize(20);
            text.draw(words2, 10, 300, 0);

            sprintf(words2, "%s", "Ctrl + Up: Move Forward");

            text.setFontSize(20);
            text.draw(words2, 10, 285, 0);

            sprintf(words2, "%s", "Ctrl + Down: Move Backward");

            text.setFontSize(20);
            text.draw(words2, 10, 270, 0);


            sprintf(words2, "%s", "Ctrl + Left/Right: Roll Ship");

            text.setFontSize(20);
            text.draw(words2, 10, 255, 0);

            sprintf(words2, "%s", "Left/Right: Add/Decrease Yaw");

            text.setFontSize(20);
            text.draw(words2, 10, 240, 0);

            sprintf(words2, "%s", "Up/Down: Add/Decrease Pitch");

            text.setFontSize(20);
            text.draw(words2, 10, 225, 0);

            sprintf(words2, "%s", "Shift+Up/Down: Strafe Up/Down");

            text.setFontSize(20);
            text.draw(words2, 10, 210, 0);

            sprintf(words2, "%s", "Shift+Left/Right: Strafe Left/Right");

            text.setFontSize(20);
            text.draw(words2, 10, 195, 0);


            sprintf(words2, "%s", "0: Spherical Camera");

            text.setFontSize(20);
            text.draw(words2, 470, 70, 0);

            sprintf(words2, "%s", "1: Flight Camera");

            text.setFontSize(20);
            text.draw(words2, 470, 55, 0);

            sprintf(words2, "%s", "2: Free Flight Camera");

            text.setFontSize(20);
            text.draw(words2, 470, 40, 0);

            sprintf(words2, "%s", "3: Forward Camera");

            text.setFontSize(20);
            text.draw(words2, 470, 25, 0);

        }
    }
    else
    {
        if (controls)
        {
            char words2[64];
            sprintf(words2, "%s", "Controls");

            text.setFontSize(23);
            text.draw(words2, 295, 480, 0);

            sprintf(words2, "%s", "A: Show Axes");

            text.setFontSize(20);
            text.draw(words2, 10, 450, 0);

            sprintf(words2, "%s", "C: Show Controls");

            text.setFontSize(20);
            text.draw(words2, 10, 435, 0);

            sprintf(words2, "%s", "F: Freeze Planets");

            text.setFontSize(20);
            text.draw(words2, 10, 420, 0);

            sprintf(words2, "%s", "I: Display Ship Info");

            text.setFontSize(20);
            text.draw(words2, 10, 405, 0);

            sprintf(words2, "%s", "L: Turn Sun Lights On/Off");

            text.setFontSize(20);
            text.draw(words2, 10, 390, 0);

            sprintf(words2, "%s", "M: Wireframe mode");

            text.setFontSize(20);
            text.draw(words2, 10, 375, 0);

            sprintf(words2, "%s", "O: Orbit Ship around Sun");

            text.setFontSize(20);
            text.draw(words2, 10, 360, 0);

            sprintf(words2, "%s", "S: Stop Ship");

            text.setFontSize(20);
            text.draw(words2, 10, 345, 0);

            sprintf(words2, "%s", "T: Cycle Textures (Fly around to find where)");

            text.setFontSize(20);
            text.draw(words2, 10, 330, 0);

            sprintf(words2, "%s", "Ship Controls");

            text.setFontSize(20);
            text.draw(words2, 10, 300, 0);

            sprintf(words2, "%s", "Ctrl + Up: Move Forward");

            text.setFontSize(20);
            text.draw(words2, 10, 285, 0);

            sprintf(words2, "%s", "Ctrl + Down: Move Backward");

            text.setFontSize(20);
            text.draw(words2, 10, 270, 0);


            sprintf(words2, "%s", "Ctrl + Left/Right: Roll Ship");

            text.setFontSize(20);
            text.draw(words2, 10, 255, 0);

            sprintf(words2, "%s", "Left/Right: Add/Decrease Yaw");

            text.setFontSize(20);
            text.draw(words2, 10, 240, 0);

            sprintf(words2, "%s", "Up/Down: Add/Decrease Pitch");

            text.setFontSize(20);
            text.draw(words2, 10, 225, 0);

            sprintf(words2, "%s", "Shift+Up/Down: Strafe Up/Down");

            text.setFontSize(20);
            text.draw(words2, 10, 210, 0);

            sprintf(words2, "%s", "Shift+Left/Right: Strafe Left/Right");

            text.setFontSize(20);
            text.draw(words2, 10, 195, 0);

            sprintf(words2, "%s", "1: Spherical Camera");

            text.setFontSize(20);
            text.draw(words2, 470, 70, 0);

            sprintf(words2, "%s", "2: Flight Camera");

            text.setFontSize(20);
            text.draw(words2, 470, 55, 0);

            sprintf(words2, "%s", "3: Free Flight Camera");

            text.setFontSize(20);
            text.draw(words2, 470, 40, 0);

            sprintf(words2, "%s", "4: Forward Camera");

            text.setFontSize(20);
            text.draw(words2, 470, 25, 0);

        }
        char words2[64];

        if (CameraNumber == 1)

        {

            sprintf(words2, "%s", "Spherical Camera");

            text.setFontSize(20);
            text.draw(words2, 10, 10, 0);
        }

    }

    if (CameraNumber == 2)
    {
        char words2[64];

        sprintf(words2, "%s", "Flight Camera");

        text.setFontSize(20);
        text.draw(words2, 10, 10, 0);
    }

    if (CameraNumber == 3)
    {
        char words2[64];

        sprintf(words2, "%s", "Free Flight Camera");

        text.setFontSize(20);
        text.draw(words2, 10, 10, 0);
    }
    if (CameraNumber == 4)
    {
        char words2[64];

        sprintf(words2, "%s", "Forward Camera");

        text.setFontSize(20);
        text.draw(words2, 10, 10, 0);
    }

    //-3500, 500, -2000 planet 1, -3100 planet 2, -2700 planet 3 on the x
    if ((xyz.x >= -4500 && xyz.x <= -1700) && (xyz.y >= -500 && xyz.y <= 1500) && (xyz.z <= -1000 && xyz.z >= -3000))
    {
        char words2[64];
        sprintf(words2, "%s", "Press T to cycle planets");

        text.setFontSize(20);
        text.setColor(0.0, 1.0, 0.0, 1);
        text.draw(words2, 420, 15, 0);
    }
}

/**

 \brief
 Draws the cube map space background.
 */
void GraphicsEngine::drawCubeMap()
{
    glUseProgram(CMprogram);
    glm::mat4 cubescale=glm::scale(glm::mat4(1.0), glm::vec3(200, 200, 200));


    cubemodel = cubescale;

    textrans = glm::scale(textrans, glm::vec3(200, 200, 200));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(cubemodel));

    glm::mat3 nMcube(cubemodel);

    glm::mat3 normalMatrixcube = glm::transpose(glm::inverse(nMcube));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrixcube));

    glUniformMatrix4fv(glGetUniformLocation(CMprogram, "PVM"),
                       1, GL_FALSE, glm::value_ptr(projection*view*cubemodel));

    CMSphere.draw();

}
/**

 \brief
 Moves the ship forward based on the percentage of it's max velocity it has been given. Updates cameras and up and view vectors, depends on which
 camera you are using. Takes care of distance between your camera and the ship.

 */
void GraphicsEngine::updateCameraShipEyePV()
{

    glm::vec3 eye;
    if (CameraNumber == 1)
        eye = sphcamera.getPosition();


    float now = speedclock.getElapsedTime().asSeconds();
    static float lasttime = now;

    objmodel.shipypr.moveForward(shipVelocity/100*shipMaxVel);//*(now - lasttime));


    if (CameraNumber == 2)//regular trailing camera
    {
        //upscale = 4;
        //viewscale = 15;

        glm::vec3 v = objmodel.shipypr.getView();
        glm::vec3 u = objmodel.shipypr.getUp();//left off here

        glm::vec3 sv = glm::vec3(viewscaleTrail*v.x, viewscaleTrail*v.y, viewscaleTrail*v.z);
        glm::vec3 su = glm::vec3(upscaleTrail*u.x, upscaleTrail*u.y, upscaleTrail*u.z);

        YPRCamera behindship;
        behindship.setPosition(objmodel.shipypr.getPosition() - sv + su);
        behindship.setView(v);
        behindship.setUp(u);
        view = behindship.lookAt();

        glm::vec3 eye1 = objmodel.shipypr.getPosition();

        eye = objmodel.shipypr.getPosition();
    }

    else if (CameraNumber == 3)//free flight camera
    {
        //upscale = 4;
        //viewscale = 15;

        glm::vec3 v = objmodel.shipypr.getView();
        glm::vec3 u = objmodel.shipypr.getUp();//left off here

        glm::vec3 sv = glm::vec3(viewscaleTrail*v.x, viewscaleTrail*v.y, viewscaleTrail*v.z);
        glm::vec3 su = glm::vec3(upscaleTrail*u.x, upscaleTrail*u.y, upscaleTrail*u.z);

        YPRCamera behindship;
        behindship.setPosition(objmodel.shipypr.getPosition() - sv + su);
        behindship.setView(v);
        glm::vec3 h(0, 2, -4);
        behindship.setUp(u+h);
        view = behindship.lookAt();


        eye = objmodel.shipypr.getPosition();
    }

    else if (CameraNumber == 4)//Tilt camera
    {
        //upscale = 1.75;
        //viewscale = 2.2;

        glm::vec3 v = objmodel.shipypr.getView();
        glm::vec3 u = objmodel.shipypr.getUp();//left off here

        glm::vec3 sv = glm::vec3(viewscaleTight*v.x, viewscaleTight*v.y, viewscaleTight*v.z);
        glm::vec3 su = glm::vec3(upscaleTight*u.x, upscaleTight*u.y, upscaleTight*u.z);

        YPRCamera behindship;
        behindship.setPosition(objmodel.shipypr.getPosition() - sv + su);
        behindship.setView(v);
        glm::vec3 h(0, .75, -1.2);
        behindship.setUp(u + h);
        view = behindship.lookAt();


        eye = objmodel.shipypr.getPosition();
    }

    glUniform3fv(glGetUniformLocation(program, "eye"), 1, glm::value_ptr(eye));


    objmodel.setEye(eye);
    objmodel.setProjectionMatrix(projection);
    objmodel.setViewMatrix(view);

}
/**
 \brief

 Draws all of the planets and their transformations. This takes care of both the planets that rotate around the sun and the planets that are freely out in space.

 */
void GraphicsEngine::drawPlanets()
{
    if (rotation)
        RotateOuter = glm::rotate(glm::mat4(1.0), -45*degf*(stoptime+elptime), glm::vec3(0, 1, 0));
    else
        RotateOuter = glm::rotate(glm::mat4(1.0), -45*degf*stoptime, glm::vec3(0, 1, 0));
    //planet 1 on fire one
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(9, 9, 9));
    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(70, 0, 70));
    model = RotateOuter*transmat*rotatemat*scalemat;


    // Load model matrix.
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Extract upper left 3 X 3 from model matrix.
    glm::mat3 nM(model);

    // Create the Normal Transformation matrix N = (M^(-1))^T, where M is the
    // upper left 3 X 3 from model matrix.
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));



    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    glm::mat4 texTrans = glm::translate(glm::mat4(1.0), glm::vec3(.25*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));



    turnTextureOn("useTexture", 4);




    Planet.draw();
    turnTextureOff("useTexture", 4);//herre left off copying

    turnTextureOn("useTexture", 3);

    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(.05*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(16, 16, 16));
    model = scalemat*rotatemat;
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Extract upper left 3 X 3 from model matrix.
    nM = glm::mat3(model);

    // Create the Normal Transformation matrix N = (M^(-1))^T, where M is the
    // upper left 3 X 3 from model matrix.
    normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));



    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));


    mat = Materials::copper;
    loadMaterial(mat);
    Planet.draw();
    turnTextureOff("useTexture", 3);

    //2nd ice planet start
    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(-1*.20*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

    if (rotation)
        RotateOuter = glm::rotate(glm::mat4(1.0), -20*degf*(stoptime+elptime), glm::vec3(0, 1, 0));
    else
        RotateOuter = glm::rotate(glm::mat4(1.0), -20*degf*stoptime, glm::vec3(0, 1, 0));

    //ice planet
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(12, 12, 12));
    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(145, 0, 145));
    model = RotateOuter*transmat*rotatemat*scalemat;
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    std::cout<< glGetError() << std::endl;
    // Extract upper left 3 X 3 from model matrix.
    nM = glm::mat3(model);

    // Create the Normal Transformation matrix N = (M^(-1))^T, where M is the
    // upper left 3 X 3 from model matrix.
    normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));



    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    turnTextureOn("useTexture", 6);
    Planet.draw();
    turnTextureOff("useTexture", 6);

    turnTextureOn("useTexture", 5);

    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(-1*.15*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

    if (rotation)
        RotateOuter = glm::rotate(glm::mat4(1.0), 30*degf*(stoptime+elptime), glm::vec3(0, 1, 0));
    else
        RotateOuter = glm::rotate(glm::mat4(1.0), 30*degf*stoptime, glm::vec3(0, 1, 0));

    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(-110, 0, -110));
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(9, 9, 9));
    model = RotateOuter*transmat*rotatemat*scalemat;

    nM = glm::mat3(model);

    normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    Planet.draw();


    turnTextureOff("useTexture", 5);




    turnTextureOn("useTexture", 7);

    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(-1*.15*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

    if (rotation)
        RotateOuter = glm::rotate(glm::mat4(1.0), 30*degf*(stoptime+elptime), glm::vec3(0, 1, 0));
    else
        RotateOuter = glm::rotate(glm::mat4(1.0), 30*degf*stoptime, glm::vec3(0, 1, 0));

    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(180, 0, 180));
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(9, 9, 9));
    model = RotateOuter*transmat*rotatemat*scalemat;

    nM = glm::mat3(model);

    normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    Planet.draw();

    turnTextureOff("useTexture", 7);





    turnTextureOn("useTexture", 5);

    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(.3*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

    RotateOuter = glm::rotate(glm::mat4(1.0), 120*degf*elptime, glm::vec3(0, 2000, 4000));
    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(150, 2000, 4000));
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(60, 60, 60));
    model = RotateOuter*transmat*rotatemat*scalemat;

    nM = glm::mat3(model);

    normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    Planet.draw();



    turnTextureOff("useTexture", 5);

    turnTextureOn("useTexture", 5);

    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(.3*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));


    RotateOuter = glm::rotate(glm::mat4(1.0), 120*degf*elptime, glm::vec3(0, 2000, 4000));
    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(-150, 2000, 4000));
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(60, 60, 60));
    model = RotateOuter*transmat*rotatemat*scalemat;

    nM = glm::mat3(model);

    normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    Planet.draw();


    turnTextureOff("useTexture", 5);




    turnTextureOn("useTexture", 1);

    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(.3*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));


    RotateOuter = glm::rotate(glm::mat4(1.0), 120*degf*elptime, glm::vec3(0, 10, 0));
    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(0, -6000, -6000));
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(78, 78, 78));
    model = transmat*rotatemat*scalemat;

    nM = glm::mat3(model);

    normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    Planet.draw();


    turnTextureOff("useTexture", 1);

    turnTextureOn("useTexture", 6);

    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(.3*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));


    float etime = extraclock.getElapsedTime().asSeconds();
    RotateOuter = glm::rotate(glm::mat4(1.0), 40*degf*etime, glm::vec3(0, -6000, -6000));
    rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(0, -5750, -6100));
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(15, 15, 15));
    model = RotateOuter*transmat*rotatemat*scalemat;

    nM = glm::mat3(model);

    normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

    Planet.draw();


    turnTextureOff("useTexture", 6);

    turnTextureOn("useTexture", 6);

    texTrans = glm::translate(glm::mat4(1.0), glm::vec3(.3*elptime2, 0, 0));

    textrans = texTrans;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));


    float mt = moontime.getElapsedTime().asSeconds();

    if (rs)//stagger the second moon rotating around the far-off gas planet
    {


        RotateOuter = glm::rotate(glm::mat4(1.0), 40*degf*mt, glm::vec3(0, -6000, -6000));
        rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
        transmat = glm::translate(glm::mat4(1.0), glm::vec3(0, -5750, -6100));
        scalemat = glm::scale(glm::mat4(1.0), glm::vec3(15, 15, 15));
        model = RotateOuter*transmat*rotatemat*scalemat;

        nM = glm::mat3(model);

        normalMatrix = glm::transpose(glm::inverse(nM));
        glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
        Planet.draw();
    }
    if (elptime > 4.4 && !rs)//used to stagger it's translation to not happen until it's on the opposite side
    {
        rs = true;
        moontime.restart();
    }



    turnTextureOff("useTexture", 6);


    for (int i = 0; i < 3; i++)
    {
        turnTextureOn("useTexture", v);
        int ofst = 400*i;
        texTrans = glm::translate(glm::mat4(1.0), glm::vec3(.3*elptime2, 0, 0));

        textrans = texTrans;
        glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

        RotateOuter = glm::rotate(glm::mat4(1.0), 120*degf*elptime, glm::vec3(-3500, 500, -2000));
        rotatemat = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(1, 0, 0));
        transmat = glm::translate(glm::mat4(1.0), glm::vec3(-3500+ofst, 500, -2000));
        scalemat = glm::scale(glm::mat4(1.0), glm::vec3(80, 80, 80));
        model = transmat*rotatemat*scalemat;

        nM = glm::mat3(model);

        normalMatrix = glm::transpose(glm::inverse(nM));
        glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));

        Planet.draw();



        turnTextureOff("useTexture", v);
    }
}
/**

 \brief
 Cycles through the textures for the three planets that are in a row.

 */
void GraphicsEngine::changeTexture()
{
    v++;
    if (v == 12)
        v=1;
}

/**
 \brief
 Turns the sun's lights on or off/

 */
void GraphicsEngine::setSunLights()
{
    if (lightsNo == 7)
        lightsNo = 3;
    else
        lightsNo = 7;
}

/**
 \brief
 Shows the controls menu. Can be done in the ship or in the spherical camera.
 */
void GraphicsEngine::showControls()
{
    if (controls)
        controls = false;
    else
        controls = true;
}

