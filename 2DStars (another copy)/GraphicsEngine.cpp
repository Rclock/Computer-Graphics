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
\date      5/14/2019

*/

/**
\brief Constructor

\param title --- Title to be placed in the titlebar of the graphics window.
\param MajorVer --- The OpenGL major version that is requested.
\param MinorVer --- The OpenGL minor version that is requested.
\param width --- The width (in pixels) of the graphics window.
\param height --- The height (in pixels) of the graphics window.

Creates rendering window, enables GLEW, loads the shaders, and sets some initial
data settings. Also sets texturing things up, sets up the sound effect, loads in a freetype font.

*/


GraphicsEngine::GraphicsEngine(std::string title, GLint MajorVer, GLint MinorVer, int width, int height) :
    sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default,
                     sf::ContextSettings(24, 8, 4, MajorVer, MinorVer, sf::ContextSettings::Core))
{

    //  Load the shaders
    program = LoadShadersFromFile("AspectRatioVert.glsl", "PassThroughFrag.glsl");

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





    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);

    // Turn on the shader.
    glUseProgram(program);

    projLoc = glGetUniformLocation(program, "ProjectionMatrix");
    modelLoc = glGetUniformLocation(program, "ModelMatrix");
    texTransLoc = glGetUniformLocation(program, "textrans");
    //colorLoc = glGetUniformLocation(program, "colors");
    setProjectionMatrix();


    sf::Image texture;
    std::string filename;

    glGenTextures(12, texID);

    for (int i = 0; i < 12; i++)
    {
        if (i == 0)
            filename = "Textures/Constellations.png";
        else if (i == 1)
            filename = "Textures/Constellations.png";
        else if (i == 2)
            filename = "Textures/Sun.jpg";
        else if (i == 3)
            filename = "Textures/Ice.jpg";
        else if (i == 4)
            filename = "Textures/Eerie.jpg";
        else if (i == 5)
            filename = "Textures/BlueGas.png";
        else if (i == 6)
            filename = "Textures/Fline.jpg";
        else if (i == 7)
            filename = "Textures/Aliens.png";
        else if (i == 8)
            filename = "Textures/Pgas.png";
        else if (i == 9)
            filename = "Textures/Pline.jpeg";
        else if (i == 10)
            filename = "Textures/Water.jpg";
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

    animclock.restart();
    animclock2.restart();
    texclock.restart();
    sunclock.restart();
    srand(clock());
    float numstars = rand() % 12 + 12;
    for (int i = 0; i < numstars; i++)
        //generateStar(i, 0);



    sphere.createSphereOBJ(.5, 50, 50);
    sphere.LoadDataToGraphicsCard(0, 1, 2);
    elpclock.restart();



    if (!music.openFromFile("ShootingStar.wav"))
    std::cout << "Couldn't open wav" << std::endl;

    text.loadFont("fonts/Px437_TandyNew_Mono.ttf");
    //text.loadFont("fonts/times.ttf");
    //text.loadFont("fonts/segoesc.ttf");
    text.setFontSize(23);
    text.setColor(0, 0, 1, 1);
    text.setScreenSize(getSize().x, getSize().y);



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

Draws the shooting stars, planets, and background. Also prints the freetype.

*/

void GraphicsEngine::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glUseProgram(program);

   ///> Wireframe would not work regularly as in all other programs, so added this line to fix it.
   glPolygonMode(GL_FRONT_AND_BACK, mode);

    srand(clock());


    turnTexturesOff("useTexture", 11);

    glLineWidth(1);

    ///> Update position of shooting star if there is one currently and draw it, play sound
    updateShootingStar();

    ///> Draw all planets and their transformations and textures at the current time
    drawPlanets();

    ///>Draw the background
    scalemat = glm::scale(Identity, glm::vec3(1, 1, 1));

    model = scalemat;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    textrans = glm::mat4(1.0);
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    turnTextureOn("useTexture", 1);

    plane.draw();

    turnTextureOff("useTexture", 1);

    turnTexturesOff("useTexture", 11);

    ///> draw all of the freetype, for shooting stars and texture cycling
    drawFreeType();

    glUseProgram(program);
    srand(clock());
    printOpenGLErrors();

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

 \param starno --- Was used back when I was using a middle star for the sun, is not being used anymore.
 \param shooting --- Whether or not it is a shooting star. In this program, only time the star class
 is used is for shooting stars. So it always will be.
 */
void GraphicsEngine::generateStar(int starno, int shooting)
{

        Stars.push_back(new Star(0, starno, shooting));

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
        ProjectionMatrix = glm::ortho(-aspratio, aspratio, -1.0f, 1.0f, -1.0f, 1.0f);
    }
    else
    {
        GLfloat aspratio = (GLfloat)getSize().y / getSize().x;
        ProjectionMatrix = glm::ortho(-1.0f, 1.0f, -aspratio, aspratio, -1.0f, 1.0f);
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


void GraphicsEngine::turnTexturesOff(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnTextureOff(name.c_str(), i);
}

void GraphicsEngine::turnTextureOff(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d]", arrayname, i);
    glUniform1i(glGetUniformLocation(program, locID), false);
}

void GraphicsEngine::turnTexturesOn(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnTextureOn(name.c_str(), i);
}

void GraphicsEngine::turnTextureOn(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d]", arrayname, i);
    glUniform1i(glGetUniformLocation(program, locID), true);
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
 \brief
    This function cycles through different textures on the planets. turnTextureOn is called with
    these variables.
    */
void GraphicsEngine::cycleTextures()
{
    t++; u++; v++;
    if (t >= 12)
        t = 3;
    if (u >= 12)
        u = 3;
    if (v >= 12)
        v = 3;
}
/**
    \brief
    This function simply freeze the planets in their current position, the textures on the
    planets keep spinning. So the planets stop rotating around the sun, but keep rotating on their
    own axis.
    */
void GraphicsEngine::freezePlanets()
{
     if (rotation)
    {
        rotation = false;
        stoptime = elpclock.getElapsedTime().asSeconds()+stoptime;
        //stoptex = texclock.getElapsedTime().asSeconds()+stoptex;

    }
    else
    {
        //texclock.restart();
        elpclock.restart();
        rotation=true;
    }
}
/**
 \brief
 This function is responsible for drawing the shooting stars and determining where they are,
 and playing the sound effect. The shooting star is really two stars with different numbers of points
 overlayed on top of each other. One star has 7 points and one has 15.


 */
void GraphicsEngine::updateShootingStar()
{
    GLfloat randomtime = rand() % 10 + 6;
    GLint val;
    static int s = 0;



    if (animclock3.getElapsedTime().asSeconds() > randomtime)
    {
        if (s == 0)
        {
            generateStar(Stars.size() + 1, 1);
            generateStar(Stars.size() + 1, 2);
             Stars[Stars.size()-1]->setInitialOuterX(Stars[Stars.size()-2]->getInitialOuterX());
            Stars[Stars.size()-1]->setInitialOuterY(Stars[Stars.size()-2]->getInitialOuterY());
             Stars[Stars.size()-1]->setCenterX(Stars[Stars.size()-2]->getCenterX());
            Stars[Stars.size()-1]->setCenterY(Stars[Stars.size()-2]->getCenterY());
           // generateStar(Stars.size() + 1, 3);


        }
        else
        {
            Stars.pop_back();
            Stars.pop_back();
            //Stars.pop_back();

            generateStar(Stars.size() + 1, 1);
            generateStar(Stars.size() + 1, 2);
            Stars[Stars.size()-1]->setInitialOuterX(Stars[Stars.size()-2]->getInitialOuterX());
            Stars[Stars.size()-1]->setInitialOuterY(Stars[Stars.size()-2]->getInitialOuterY());
            Stars[Stars.size()-1]->setCenterX(Stars[Stars.size()-2]->getCenterX());
            Stars[Stars.size()-1]->setCenterY(Stars[Stars.size()-2]->getCenterY());

        }
        starclock.restart();
        music.play();
        drawtext = true;

        s++;
        animclock3.restart();
        randomtime = rand() % 15 + 1;

    }



    for (int i = 0; i < Stars.size(); i++)
    {

        float elptime = animclock.getElapsedTime().asSeconds();
        float elptime2 = animclock2.getElapsedTime().asSeconds();


        Stars[i]->updateRotation(elptime2, false);
        Stars[i]->updateDirection(elptime, false, animclock3.getElapsedTime().asSeconds());

            glm::mat4 TranslateMat = glm::translate(glm::mat4(1.0), glm::vec3(Stars[i]->getPosX(), Stars[i]->getPosY(), 0));//translate by pos[] instead??
            glm::mat4 ScaleMat = glm::scale(glm::mat4(1.0), glm::vec3(Stars[i]->getScale(), Stars[i]->getScale(), 1));
            glm::mat4 RotateMat = glm::rotate(glm::mat4(1.0), Stars[i]->getRot()*degf, glm::vec3(0, 0, 1));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TranslateMat*RotateMat*ScaleMat));
            Stars[i]->draw();
    }
}

/**

 \brief
 This function draws all of the planets and their textures including the sun. All transformations
 and texturing take place here.
 */
void GraphicsEngine::drawPlanets()
{
    float elptex = texclock.getElapsedTime().asSeconds();
    float elptime = elpclock.getElapsedTime().asSeconds();
    float suntime = sunclock.getElapsedTime().asSeconds();



    model = glm::mat4(1.0);
    scalemat = glm::scale(glm::mat4(1.0), glm::vec3(.5, .5, 1));
    transmat = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
    model = transmat*scalemat;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    texrot = glm::translate(Identity, glm::vec3(1*suntime/30, 0, 0));
    texscale = glm::scale(Identity, glm::vec3(.5, .5, 1));
    textrans = texrot*texscale;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));


    turnTextureOn("useTexture", 2);
    glUseProgram(program);

    planet.draw();


    turnTextureOff("useTexture", 2);

    if (rotation)
        rotateouter = glm::rotate(Identity, -35*degf*(elptime+stoptime), glm::vec3(0, 0, 1));
    else
        rotateouter = glm::rotate(Identity, -35*degf*stoptime, glm::vec3(0, 0, 1));

    trick = glm::rotate(Identity, 35*degf*(elptime+stoptime), glm::vec3(0, 0, 1));
    if (!rotation)
        trick = glm::rotate(Identity, 35*degf*stoptime, glm::vec3(0, 0, 1));

    transmat = glm::translate(Identity, glm::vec3(-.65, .65, 0));
    scalemat = glm::scale(Identity, glm::vec3(.25, .25, 1));

    model = rotateouter*transmat*trick*scalemat;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    texscale = glm::scale(Identity, glm::vec3(.25, .25, 1));
    texrot = glm::translate(Identity, glm::vec3(1*elptex/7, 0, 0));
    textrans = texrot*texscale;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

    turnTextureOn("useTexture", t);

    planet.draw();

    //plane.draw();

    turnTextureOff("useTexture", t);

     if (rotation)
        rotateouter = glm::rotate(Identity, 55*degf*(elptime+stoptime), glm::vec3(0, 0, 1));
    else
        rotateouter = glm::rotate(Identity, 55*degf*stoptime, glm::vec3(0, 0, 1));

    //rotateouter = glm::rotate(Identity, 55*degf*elptime, glm::vec3(0, 0, 1));
    trick = glm::rotate(Identity, -55*degf*(elptime+stoptime), glm::vec3(0, 0, 1));

     if (!rotation)
        trick = glm::rotate(Identity, -55*degf*stoptime, glm::vec3(0, 0, 1));

    transmat = glm::translate(Identity, glm::vec3(-.4, -.4, 0));
    scalemat = glm::scale(Identity, glm::vec3(.4, .4, 1));

    model = rotateouter*transmat*trick*scalemat;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    texscale = glm::scale(Identity, glm::vec3(.35, .35, 1));
    texrot = glm::translate(Identity, glm::vec3(-1*elptex/12, 0, 0));
    textrans = texrot*texscale;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    turnTextureOn("useTexture", u);

    planet.draw();

    turnTextureOff("useTexture", u);


    if (rotation)
        rotateouter = glm::rotate(Identity, 55*degf*(elptime+stoptime), glm::vec3(0, 0, 1));
    else
        rotateouter = glm::rotate(Identity, 55*degf*stoptime, glm::vec3(0, 0, 1));


    trick = glm::rotate(Identity, -55*degf*(elptime+stoptime), glm::vec3(0, 0, 1));
    if (!rotation)
        trick = glm::rotate(Identity, -55*degf*stoptime, glm::vec3(0, 0, 1));

    transmat = glm::translate(Identity, glm::vec3(0, .55, 0));
    scalemat = glm::scale(Identity, glm::vec3(.35, .35, 1));

    model = rotateouter*transmat*trick*scalemat;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    texscale = glm::scale(Identity, glm::vec3(.35, .35, 1));
    texrot = glm::translate(Identity, glm::vec3(1*(elptex)/10, 0, 0));
    textrans = texrot*texscale;
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    turnTextureOn("useTexture", v);

    planet.draw();

    turnTextureOff("useTexture", v);

}

/**

 \brief
 There is some freetype in the program, this function draws it to the screen. Some text is always drawn,
 some text is only drawn when a shooting star appears.
 */
void GraphicsEngine::drawFreeType()
{
     char words1[64];

      sprintf(words1, "%s", "Press T to cycle textures");

    text.setFontSize(22);
    text.setColor(1.0, .7, 0.0, .9);
    text.draw(words1, 375, 15, 0);

    if (drawtext)
    {
        if (starclock.getElapsedTime().asSeconds() > 3.0)
            drawtext = false;



    sprintf(words1, "%s", "Shooting Star");

    text.setFontSize(22);
    text.setColor(1.0, 0.0, 0.0, .9);
    text.draw(words1, 15, 15, 0);
    }
}
