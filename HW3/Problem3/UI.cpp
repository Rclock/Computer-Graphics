#include "UI.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
/**
\file UI.cpp
\brief User interface processor for the program.

\author    Ryan Clocker
\version   1.0
\date      2/23/2019

*/
GLfloat numboxes = 0;

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

This method processes all events in the current SFML event queue and calls the
corresponding processing method.  At the end it calls the keyboard state processor
method, outside the event loop.

*/

void UI::processEvents()
{
    // Process user events
    sf::Event event;
    bool ctrldown;
    while (ge->pollEvent(event))
    {
        // Close Window or Escape Key Pressed: exit
        if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            ge->close();

        // Key is pressed.
        if (event.type == sf::Event::KeyPressed)
            {
            keyPressed(event.key);
            }

        // Window is resized.
        if (event.type == sf::Event::Resized)
            ge->resize();

        if (event.type == sf::Event::MouseMoved)//process mouse moved event
        {
         processMouseMoved(event.mouseMove);
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {

            processMouseButtonReleased(event.mouseButton);
        }
        /**
        /brief

        The below handles all mouse button pressed events. This is the bulk of the program. I started implementing here instead
        of in a function, and I just sort of kept going. Not enough time to go back and move it all down. I am just fortunate
        everything works at this point.
        */
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f points = MouseToScreenConversion(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            ge->clickOrigin = points;
            if (event.mouseButton.button == sf::Mouse::Right)//create a new box
            {
                cout << "Right mouse pressed \n";
                if ((sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)))
                {

                cout << "Control is down\n";
                ctrlDownPublic = true;

                 srand (clock());

                if(!(ge->compareToAllBoxes(points.x, points.y)))
                {

                ge->createBoxes(points.x, points.y, ((double)rand()) / ((double)RAND_MAX) * .2 + 0.1, ((double)rand()) / ((double)RAND_MAX) * .2 + 0.1);

                }
                else
                {
                ge->compareToAllBoxesDelete(points.x, points.y);
                }


                cout << "X and y center values for box " << points.x << " " << points.y << " \n";

                }
            }
            if (event.mouseButton.button == sf::Mouse::Left)//
            {
                mouseDown = true;

              //  srand (time(NULL));


                if ((sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)))
                {

                ge->selectBox(points.x, points.y);//checks if you are inside a box, selects if so.
                if (!ge->compareToAllBoxes(points.x, points.y))//deselects all boxes if clicked outside any box
                {
                ge->deselectAll();
                }

                }




            }


        }




    }
    // Process the state of the keyboard outside of event firing,
    keyboardStateProcessing();
}

/**
 \brief The function handles the keyboard input events from the user.

\param keyevent --- The SFML key code for the key pressed.

\remark

If no modifier keys are pressed:
– Escape: Ends the program.
– M: Toggles between fill mode and line mode to draw the triangles.
– R: Resets the window size to 700 X 500.
– A: Resets the window size to 600 X 600.
– F10: Saves a screen shot of the graphics window to a png file.

--- If CTRL + A: selects all boxes.
--- If CTRL + Q: Deselects all boxes.

--- Important note: In "other program features", it was not specified that we needed to be able to change these boxes to green,
blue, and white. The changered function has that capability, but it will not be called from the UI because you would need to
know which box we are talking about. This was not talked about in the description for number 3, so it is not used in the UI.


*/

void UI::keyPressed(sf::Event::KeyEvent keyevent)
{
    int key = keyevent.code;
    bool altdown = keyevent.alt;
    ctrlDownPublic = keyevent.control;
    //bool shiftdown = keyevent.shift;

    if (ctrlDownPublic)//do deselect and select all here because we do not need state processing for this.
    {
     if (key == sf::Keyboard::A)//select all if ctrl+A
        {

            ge->selectAll();
        }
        if (key == sf::Keyboard::Q)//deselect all if ctrl+Q
        {

            ge->deselectAll();
        }

    }
/*if (altdown)
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
}*/

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

• If no modifier keys are pressed:
– Left: Moves the box to the left.
– Right: Moves the box to the right.
– Up: Moves the box up.
– Down: Moves the box down.

• If the control key is down:
– Left: Decreases the width of the box.
– Right: Increases the width of the box.
– Up: Decreases the height of the box.
– Down: Increases the height of the box.

• If the alt key is down:
– Up: Increases the height and width of the box.
– Down: Decreases the height and width of the box.

*/

void UI::keyboardStateProcessing()
{
    GLfloat BoxCenter[2];


    sf::Vector2i wp;
    sf::Vector2u ws;


    bool ctrldown1 = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
    bool altdown = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);

    if (ctrldown1)
    {
        // Change the box dimensions.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            ge->decreaseWidth();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            ge->increaseWidth();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            ge->increaseHeight();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
         ge->decreaseHeight();
        }
    }
    else if (altdown)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
        ge->decreaseHeight();
        ge->decreaseWidth();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
        ge->increaseHeight();
        ge->increaseWidth();
        }


    }

    else
    {
        // Change the box position.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            ge->moveSelectedLeft();
        }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
        ge->moveSelectedRight();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            ge->moveSelectedUp();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
        ge->moveSelectedDown();
        }
    }
}
/**
 \brief
Converts mouse position coordinates to screen coordinates (pixel to screen) takes a vector of mousepositon coordinates.

 \param mousePosition --- sf::Vector2i of mouse position coordinates
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
Processes all events having to do with the mouse moved. Drags things, recolors things, etc.

 \param sf::Event::MouseMoveEvent mouseMoveEvent: --- Pass in the event of the mouse moving. Use that to get coordinates.
*/
void UI::processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent)
{
    //important: move color changing code to its own graphics engine function working directly with the box, or in problem 2 move it to the box
    sf::Vector2f newMouseSpot = MouseToScreenConversion(sf::Vector2i(mouseMoveEvent.x, mouseMoveEvent.y));

    ge->checkRed(newMouseSpot.x, newMouseSpot.y);


    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
    {
                    if (ge->compareToAllSelectedBoxes(newMouseSpot.x, newMouseSpot.y))
                    {
                        if (mouseDown)
                        {
                        cout << "Hello\n";
                        ge->dragSelectedBoxes(newMouseSpot.x, newMouseSpot.y);
                        }
                    }
    }
    else
    {

    if (mouseDown)
    {
    ge->compareToAllBoxesDrag(newMouseSpot.x, newMouseSpot.y);
    }
    }


    //turn box color red if mouse is over the box

    //something with the above line actually makes it delete two boxes sometimes...


    if (mouseDown)
        std::cout << "Drag ~ ";
////    std::cout << "Moved: (" << mouseMoveEvent.x << ", " << mouseMoveEvent.y << ")" << "  ~  (" << newMouseSpot.x << ", " << newMouseSpot.y << ")" << std::endl;


}
/**
 \brief
Processes mouse button released events. Changes mouseDown to false, determines type of mouse button release.

 \param mouseButtonEvent --- Uses this to get the info (left, right, etc) from the event.
*/
void UI::processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    mouseDown = false;
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
