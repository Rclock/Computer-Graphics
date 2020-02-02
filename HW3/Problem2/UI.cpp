#include "UI.h"
#include <iostream>
#include <stdlib.h>
/**
\file UI.cpp
\brief User interface processor for the program.

\author    Ryan Clocker
\version   1.0
\date      2.26.2019

*/

/**
\brief Constructor

\param graph --- Pointer to the GraphicsEngine that this interface processor is attached.

Simply stores the pointer of the GraphicsEngine.

*/

UI::UI(GraphicsEngine* graph)
{
    ge = graph;
}


/**
\brief Destructor

No implementation needed at this point.

*/

UI::~UI() {}

/**
 \brief The method handles the SFML event processing and calls the keyboard state processor
method.

Processes mouse released, mouse pressed, and mouse moved events. Also some key pressed events and some state processing.

*/

void UI::processEvents()
{
    // Process user events
    sf::Event event;
    while (ge->pollEvent(event))
    {
        // Close Window or Escape Key Pressed: exit
        if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            ge->close();

        // Key is pressed.
        if (event.type == sf::Event::KeyPressed)
            keyPressed(event.key);

        // Window is resized.
        if (event.type == sf::Event::Resized)
            ge->resize();
        if (event.type == sf::Event::MouseButtonPressed)
        {
            processMouseButtonPressed(event.mouseButton);
        }
        if (event.type == sf::Event::MouseMoved)//process mouse moved event
        {
            processMouseMoved(event.mouseMove);
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            processMouseButtonReleased(event.mouseButton);
        }


    // Process the state of the keyboard outside of event firing,
    keyboardStateProcessing();

    }
}

/**
 \brief The function handles the keyboard input events from the user.

\param keyevent --- The SFML key code for the key pressed.

\remark

If no modifier keys are pressed:

- M: Toggles between fill mode and line mode to draw the triangles.
- R: Resets the window size to 700 X 500.
- A: Resets the window size to 600 X 600.
- F10: Saves a screen shot of the graphics window to a png file.

If the control key is down:

- R: Resets the box size.

If the alt key is down:

- B: Turns box solid blue.
- G: Turns box solid green.
- W: Turns box solid white.
- M: Turns box to original colors.
*/

void UI::keyPressed(sf::Event::KeyEvent keyevent)
{
    int key = keyevent.code;
    bool altdown = keyevent.alt;
    bool ctrldown = keyevent.control;
    bool shiftdown = keyevent.shift;

    if (ctrldown)
    {
        // Reset the box size.
        if (key == sf::Keyboard::R)
        {
            ge->getBox()->setWidth(1);
            ge->getBox()->setHeight(1);
        }
    }
     else if (altdown)
    {

        if (key == sf::Keyboard::B)//turns 4 corner vertices solid blue
        {//r, g, b
        ge->getBox()->green = false;
        ge->getBox()->blue = true;
        ge->getBox()->white = false;
        ge->getBox()->regular = false;
        ge->getBox()->setVertexColor(0, 0, 0, 1);//vertex 1 (top left)
        ge->getBox()->setVertexColor(1, 0, 0, 1);//vertex 2
        ge->getBox()->setVertexColor(2, 0, 0, 1);//vertex 3
        ge->getBox()->setVertexColor(3, 0, 0, 1);//vertex 4
        }
         if (key == sf::Keyboard::G)//turns all 4 corner vertices green
        {
        ge->getBox()->green = true;
        ge->getBox()->blue = false;
        ge->getBox()->white = false;
        ge->getBox()->regular = false;
        ge->getBox()->setVertexColor(0, 0, 1, 0);//top left vertex
        ge->getBox()->setVertexColor(1, 0, 1, 0);//(r, g, b)
        ge->getBox()->setVertexColor(2, 0, 1, 0);
        ge->getBox()->setVertexColor(3, 0, 1, 0);
        }
         if (key == sf::Keyboard::W)//turns all 4 corner vertices white
        {//r, g, b
        ge->getBox()->green = false;
        ge->getBox()->blue = false;
        ge->getBox()->white = true;
        ge->getBox()->regular = false;
         ge->getBox()->setVertexColor(0, 1, 1, 1);//top left vertex
        ge->getBox()->setVertexColor(1, 1, 1, 1);
        ge->getBox()->setVertexColor(2, 1, 1, 1);
        ge->getBox()->setVertexColor(3, 1, 1, 1);
        }
         if (key == sf::Keyboard::M)//turn colors back to red/green/white/blue
        {//r, g, b
        ge->getBox()->green = false;
        ge->getBox()->blue = false;
        ge->getBox()->white = false;
        ge->getBox()->regular = true;
        ge->getBox()->setVertexColor(0, 1, 0, 0);//top left vertex red
        ge->getBox()->setVertexColor(1, 0, 1, 0);//top right vertex green
        ge->getBox()->setVertexColor(2, 0, 0, 1);//bottom right vertex blue
        ge->getBox()->setVertexColor(3, 1, 1, 1);//bottom left vertex white
        }
    }

    else
    {
        switch (key)
        {
        // F10 takes a screenshot.
        case sf::Keyboard::F10:
            ge->screenshot();
            break;

        // If the M key is pressed toggle the display mode between Fill and Line.
        case sf::Keyboard::M:
            ge->changeMode();
            break;

        // Resets the screen size to 700 X 500.
        case sf::Keyboard::R:
            ge->setSize(700, 500);
            break;

        // Sets the screen size to 600 X 600.
        case sf::Keyboard::A:
            ge->setSize(600, 600);
            break;

        default:
            break;
        }
    }
}

/**
 \brief The method processes the keyboard state.

\remark

If no modifier keys are pressed:

- Left: Moves the box to the left.
- Right: Moves the box to the right.
- Up: Moves the box up.
- Down: Moves the box down.

If the control key is down:

- Left: Decreases the width of the box.
- Right: Increases the width of the box.
- Up: Decreases the height of the box.
- Down: Increases the height of the box.


*/

void UI::keyboardStateProcessing()
{
    GLfloat BoxCenter[2];
    float w, h;
    sf::Vector2i wp;
    sf::Vector2u ws;


    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);


    if (ctrldown)
    {
        // Change the box dimensions.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            w = ge->getBox()->getWidth();
            ge->getBox()->setWidth(w - 0.01);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            w = ge->getBox()->getWidth();
            ge->getBox()->setWidth(w + 0.01);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            h = ge->getBox()->getHeight();
            ge->getBox()->setHeight(h - 0.01);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            h = ge->getBox()->getHeight();
            ge->getBox()->setHeight(h + 0.01);
        }
    }

    else
    {
        // Change the box position.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            ge->getBox()->getCenter(BoxCenter);
            ge->getBox()->setCenter(BoxCenter[0]-0.01, BoxCenter[1]);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            ge->getBox()->getCenter(BoxCenter);
            ge->getBox()->setCenter(BoxCenter[0]+0.01, BoxCenter[1]);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            ge->getBox()->getCenter(BoxCenter);
            ge->getBox()->setCenter(BoxCenter[0], BoxCenter[1]+0.01);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            ge->getBox()->getCenter(BoxCenter);
            ge->getBox()->setCenter(BoxCenter[0], BoxCenter[1]-0.01);
        }
    }
}
/**
 \brief
 Converts pixel mouse coordinates to screen cooridnates, using the -1->1 screen bounds. Returns a points sf::vector2f with the
 converted x and y coords.

 \param mousePosition - an sf::Vector2i  that are the pixel coordinates of the mouse on the screen.
 */
sf::Vector2f UI::MouseToScreenConversion(sf::Vector2i mousePosition)
{

sf::Vector2f points;

GLfloat screenBounds[4];//l,r,b,t
screenBounds[0] = -1.0;
screenBounds[1] = 1.0;
screenBounds[2] = -1.0;
screenBounds[3] = 1.0;


points.x = (float)mousePosition.x / 700 * (screenBounds[1] - screenBounds[0]) + screenBounds[0];
points.y = screenBounds[3] - (float)mousePosition.y / 500 * (screenBounds[3] - screenBounds[2]);
return points;

}
/**
 \brief
Processes mouse button pressed events. These could be left or right clicks. This is important for the drag becuase
we need to set the clickOrigin to the initial click in order to do operations. This function helps with that.
 \param mouseButtonEvent - Takes a sf::Event::MouseButtonEvent from the UI loop.

 Also helps with setting the mouseDown boolean that is for the drag in the processMouseMoved.
*/
void UI::processMouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    std::string button = "";
    if (mouseButtonEvent.button == sf::Mouse::Left)
        button = "Left";

    clickOrigin = MouseToScreenConversion(sf::Vector2i(mouseButtonEvent.x, mouseButtonEvent.y));

    std::cout << "Pressed: " << button << ": (" << mouseButtonEvent.x << ", " << mouseButtonEvent.y << ")"  << "  ~  (" << clickOrigin.x << ", " << clickOrigin.y << ")" << std::endl;

    if (mouseButtonEvent.button == sf::Mouse::Left)
    {

        std::cout << "Left mouse is down\n";

        mouseDown = true;
    }

    else
    {

    mouseDown = false;
    }


}
/**
 \brief

 This function does a lot of work, mainly helps with the dragging of the box. Uses the clickOrigin and mouseDown class members to
 coordinate where the mouse is and what difference needs to be added.

 \param mouseMoveEvent - an sf::Event::MouseMoveEvent (it will get many of them)

 */
void UI::processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent)
{

    sf::Vector2f newMouseSpot = MouseToScreenConversion(sf::Vector2i(mouseMoveEvent.x, mouseMoveEvent.y));
     if (ge->getBox()->compareToBox(newMouseSpot.x, newMouseSpot.y))//turn box color red if mouse is over the box
    {//use box function above
    ge->getBox()->changeColor(true);
     }
    else//revert box to original colors
    {
            ge->getBox()->changeColor(false);

    }

    if (mouseDown)
    {
        if (ge->getBox()->compareToBox(newMouseSpot.x, newMouseSpot.y))
        {


        float bc[2];
        ge->getBox()->getCenter(bc);//change here???
        float x = bc[0] + (newMouseSpot.x - clickOrigin.x);
        float y = bc[1] + (newMouseSpot.y - clickOrigin.y);
        clickOrigin.x = newMouseSpot.x;
        clickOrigin.y = newMouseSpot.y;
        ge->getBox()->setCenter(x, y);
        std::cout << "Trying to move the box. Center of box:" << bc[0] << " " << bc[1] << "\n";
        }
    }
    if (mouseDown)
        std::cout << "Drag ~ ";
    std::cout << "Moved: (" << mouseMoveEvent.x << ", " << mouseMoveEvent.y << ")" << "  ~  (" << newMouseSpot.x << ", " << newMouseSpot.y << ")" << std::endl;


}
/**
 \brief

 Helps coordinate when the mouse is down and when it is not for the dragging.
    Sets mouseDown to false if the mouse was released, this is the key step.

    \param mouseButtonEvent - a sf::Event::MouseButtonEvent just like the mouseButtonPressed function.

    */
void UI::processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    std::string button = "";
    if (mouseButtonEvent.button == sf::Mouse::Left)
        button = "Left";
    else if (mouseButtonEvent.button == sf::Mouse::Right)
        button = "Right";
    else if (mouseButtonEvent.button == sf::Mouse::Middle)
        button = "Middle";

    sf::Vector2f sc = MouseToScreenConversion(sf::Vector2i(mouseButtonEvent.x, mouseButtonEvent.y));

    std::cout << "Released: " << button << ": (" << mouseButtonEvent.x << ", " << mouseButtonEvent.y << ")"  << "  ~  (" << sc.x << ", " << sc.y << ")" << std::endl;

    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        std::cout << "Mouse has been released \n";

        mouseDown = false;
    }

}
