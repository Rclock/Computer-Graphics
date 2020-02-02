#include "GraphicsEngine.h"

/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Ryan Clocker
\version   1.0
\date      2//22/2019

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
    //  Load the shaders
    GLuint program = LoadShadersFromFile("PassThroughVert.glsl", "PassThroughFrag.glsl");

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


}

/**
\brief Destructor

Currently empty, no allocated memory to clear.

*/

GraphicsEngine::~GraphicsEngine() {}

/**
\brief The function responsible for drawing to the OpenGL frame buffer.

This function clears the screen and calls the draw functions of the box and yellow outline.

*/

void GraphicsEngine::display()
{
    // Clear the screen (Frame Buffer)
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the objects.
for (int i = 0; i < boxes.size(); i++)
{
    boxes[i]->draw();//draw every box

    if (boxes[i]->selected)
    {
    boxes[i]->drawYellow();//if its selected, draw yellow border
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
Dynamically allocates memory and creates a new box there at the end of the vector.
*/
void GraphicsEngine::createBoxes(GLfloat cx, GLfloat cy, GLfloat w, GLfloat h)
{
boxes.push_back(new Box(cx, cy, w, h));
}



/**
\brief

This function compares the given mouse screen coordinates to every box on the screen, returns true on the first one you are in.

\param x --- Converted mouse position x coordinate
\param y --- Converted mouse position y coordinate

*/
bool GraphicsEngine::compareToAllBoxes(GLfloat x, GLfloat y)
{
for (int i = 0; i < boxes.size(); i++)
    {
    if (boxes[i]->compareToBox(x, y))
    {
    return true;
    }

    //calls the box's function
    }
    return false;
}
/**
 \brief
This function is similar to the regular compareToAllBoxes function, except for selected boxes only. If you are over a selected box
with the mouse, this will return true.

\param x --- Covnerted mouse x position
\param y --- Converted mouse y position

*/
bool GraphicsEngine::compareToAllSelectedBoxes(GLfloat x, GLfloat y)
{
for (int i = 0; i < boxes.size(); i++)
{
    if (boxes[i]->selected)
    {
        if (boxes[i]->compareToBox(x, y))
        {
        return true;
        }

    }

}
return false;
}
/**
 \brief

This function compares given mouse coordinates and finds which box you are over. It will delete that box.

\param x --- converted mouse x coordinate
\param y --- converted mouse y coordinate
*/
void GraphicsEngine::compareToAllBoxesDelete(GLfloat x, GLfloat y)
{
    for (int i = 0; i < boxes.size(); i++)
    {
    if (boxes[i]->compareToBox(x, y))
    {
    delete boxes[i];
    boxes.erase(boxes.begin()+i);//delete spot in vector AND delete object there!!!!!
    }
    //calls the box's function
    }
}
/**
 \brief

Checks if the given mouse coordinates are over a given box, will change the box's color to red if true. Calls a box function to do this.
Will revert back to normal colors also.

 \param x --- mouse x position converetd
 \param y --- mouse y position converted
*/
void GraphicsEngine::checkRed(GLfloat x, GLfloat y)
{

for (int i = 0; i < boxes.size(); i++)
    {
    if (boxes[i]->compareToBox(x, y))
    {
    boxes[i]->changeToRed(true);

    }
    else//revert box to original colors
    {
         boxes[i]->changeToRed(false);
    }

    }
}

/**
 \brief Sets up differences to drag boxes. Uses mouse coordinates and arithmetic to determine what to change to the box centers, calls a box function
to use the differences to drag the boxes.

 \param x --- converted mouse x
 \param y --- converted mouse y
*/
void GraphicsEngine::compareToAllBoxesDrag(GLfloat x, GLfloat y)
{

    for (int i = 0; i < boxes.size(); i++)
    {
    if (boxes[i]->compareToBox(x, y))
    {
        float bc[2];
        boxes[i]->getCenter(bc);//change here???
        float xnew = bc[0] + (x - clickOrigin.x);
        float ynew = bc[1] + (y - clickOrigin.y);
        clickOrigin.x = x;
        clickOrigin.y = y;
        boxes[i]->setCenter(xnew, ynew);
        boxes[i]->setYellowCenter(xnew, ynew);
        std::cout << "Trying to move the box. Center of box:" << bc[0] << " " << bc[1] << "\n";

    }

    //calls the box's function
    }


}
/**
 \brief
Drags selected boxes. Loops through all selected boxes and makes differences, calls box function to drag that box.

 \param x --- converted mouse x
 \param y --- converted mouse y

*/
void GraphicsEngine::dragSelectedBoxes(GLfloat x, GLfloat y)
{
float differencex, differencey;


for (int i = 0; i < boxes.size(); i++)
    {

    if (boxes[i]->selected)
    {

    differencex = x - clickOrigin.x;
    differencey = y - clickOrigin.y;
    cout << "passing difference x " << differencex << " and differencey " << differencey << "to function\n";
    boxes[i]->dragSelectedBox(differencex, differencey);
    }

    //calls the box's function
    }
    clickOrigin.x = x;
    clickOrigin.y = y;


}
/**
 \brief

Selects a box. Calls a box function to do this. Basically sets a boolean to true.

 \param x --- converted mouse x
 \param y --- converted mouse y
*/
void GraphicsEngine::selectBox(GLfloat x, GLfloat y)
{
for (int i = 0; i < boxes.size(); i++)
{
    if (boxes[i]->compareToBox(x, y))
    {
    //boxes[i]->changeToRed(true);
    boxes[i]->select();

    }
}
}
/**
 \brief

selects all of the boxes in the vector. Calls a box function to do so in each iteration of loop. No parameters.
*/
 void GraphicsEngine::selectAll()
 {
    for (int i = 0; i < boxes.size(); i++)
    {
    boxes[i]->select();
    }

 }
 /**
 \brief
 Deselects all boxes in the vector that are selected. Loops to the vector size and calls a box function.
 */
  void GraphicsEngine::deselectAll()
 {
    for (int i = 0; i < boxes.size(); i++)
    {
    boxes[i]->deselect();
    }

 }
 /**
 \brief
 Moves all selected boxes to the left in the state processor. Called when the left arrow key is down. Checks boolean selected and
 performs arithmetic, changes centers.

 */
 void GraphicsEngine::moveSelectedLeft()
 {
 GLfloat bc[2];
    for (int i = 0; i < boxes.size(); i++)
    {
            if (boxes[i]->selected)
            {
            boxes[i]->getCenter(bc);
            float xnew = bc[0] - .01;
            float ynew = bc[1];
            boxes[i]->setCenter(xnew, ynew);
            boxes[i]->setYellowCenter(xnew, ynew);
            }
    }

 }
 /**
 \brief
 Moves all selected boxes to the right in the state processor. Called when the right arrow key is down. Checks boolean selected and
 performs arithmetic, changes centers.

 */
 void GraphicsEngine::moveSelectedRight()
 {
 GLfloat bc[2];
    for (int i = 0; i < boxes.size(); i++)
    {
            if (boxes[i]->selected)
            {
            boxes[i]->getCenter(bc);
            float xnew = bc[0] + .01;
            float ynew = bc[1];
            boxes[i]->setCenter(xnew, ynew);
            boxes[i]->setYellowCenter(xnew, ynew);
            }
    }

 }
 /**
 \brief
 Moves all selected boxes up in the state processor. Called when the up arrow key is down. Checks boolean selected and
 performs arithmetic, changes centers.

 */
 void GraphicsEngine::moveSelectedUp()
 {
 GLfloat bc[2];
    for (int i = 0; i < boxes.size(); i++)
    {
            if (boxes[i]->selected)
            {
            boxes[i]->getCenter(bc);
            float xnew = bc[0];
            float ynew = bc[1] + .01;
            boxes[i]->setCenter(xnew, ynew);
            boxes[i]->setYellowCenter(xnew, ynew);
            }
    }

 }
 /**
 \brief
 Moves all selected boxes down in the state processor. Called when the down arrow key is down. Checks boolean selected and
 performs arithmetic, changes centers.

 */
  void GraphicsEngine::moveSelectedDown()
 {
 GLfloat bc[2];
    for (int i = 0; i < boxes.size(); i++)
    {
            if (boxes[i]->selected)
            {
            boxes[i]->getCenter(bc);
            float xnew = bc[0];
            float ynew = bc[1] - .01;
            boxes[i]->setCenter(xnew, ynew);
            boxes[i]->setYellowCenter(xnew, ynew);
            }
    }

 }
 /**
 \brief
 Decreases width of all selected boxes. Called in the state processor. Checks boolean selected and does arithmetic
 */
 void GraphicsEngine::decreaseWidth()
 {
 for (int i = 0; i < boxes.size(); i++)
    {
        if (boxes[i]->selected)
        {
        GLfloat w;
        w = boxes[i]->getWidth();
        boxes[i]->setWidth(w - 0.01);
        GLfloat y;
        y= boxes[i]->getYellowWidth();
        boxes[i]->setYellowWidth(y - .01);
        }
    }
}
/**
 \brief
 Increases width of all selected boxes. Called in the state processor. Checks boolean selected and does arithmetic
 */
void GraphicsEngine::increaseWidth()
 {
 for (int i = 0; i < boxes.size(); i++)
    {
        if (boxes[i]->selected)
        {
        GLfloat w;
        w = boxes[i]->getWidth();
        boxes[i]->setWidth(w + 0.01);
        GLfloat y;
        y= boxes[i]->getYellowWidth();
        boxes[i]->setYellowWidth(y + .01);
        }
    }
}
/**
 \brief
 Increases height of all selected boxes. Called in the state processor. Checks boolean selected and does arithmetic
 */
void GraphicsEngine::increaseHeight()
 {
 for (int i = 0; i < boxes.size(); i++)
    {
        if (boxes[i]->selected)
        {
        GLfloat h;
        h = boxes[i]->getHeight();
        boxes[i]->setHeight(h + 0.01);
        GLfloat y;
        y= boxes[i]->getYellowHeight();
        boxes[i]->setYellowHeight(y + .01);
        }
    }
}
/**
 \brief
 Decreases height of all selected boxes. Called in the state processor. Checks boolean selected and does arithmetic
 */
void GraphicsEngine::decreaseHeight()
 {
 for (int i = 0; i < boxes.size(); i++)
    {
        if (boxes[i]->selected)
        {
        GLfloat h;
        h = boxes[i]->getHeight();
        boxes[i]->setHeight(h - 0.01);
        GLfloat y;
        y= boxes[i]->getYellowHeight();
        boxes[i]->setYellowHeight(y - .01);
        }
    }
}

