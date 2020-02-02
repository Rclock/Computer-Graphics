#include "UI.h"

/**
\file UI.cpp
\brief User interface processor for the program.

\author    Ryan Clocker
\version   1.0
\date      3/15/2018

*/

/**
\brief Constructor

\param graph --- Pointer to the GraphicsEngine that this interface processor is attached.

Simply stores the pointer of the GraphicsEngine.

*/

UI::UI(GraphicsEngine* graph)
{
    ge = graph;
    boxnum = 0;
}

/**
\brief Destructor

No implementation needed at this point.

*/

UI::~UI() {}

/**
\brief The method handles the SFML event processing.

This method processes all events in the current SFML event queue and calls
the corresponding processing method.

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
    }

}

/**
\brief The function handles the keyboard input from the user.

\param keyevent --- The SFML key event.

\remark

- Escape:  Ends the program.
- M: Toggles between fill mode and line mode to draw the triangles.
- R: Resets the window size to 700 X 500.
- A: Resets the window size to 600 X 600.
- F10: Saves a screen shot of the graphics window to a png file.
- 1: Excercise 1 (Red circles)
- 2: Excercise 2 (Red/green circles)
- 3: An image that has many small boxes similar to graph paper, red and purple lines in x and y directions.
- 4: A black and red checkerboard.
- 5: Blue circles moving from the inside out.
- 6: A checkerboard that alternates between black and red to green and black, with blue circles moving in to out.
- 7: Fractal with q^2
- 8: Fractal with q^3
- 9: Fractal with q^4


*/

void UI::keyPressed(sf::Event::KeyEvent keyevent)
{
    switch (keyevent.code)
    {
    // Sets the current box to be altered.
    /* case sf::Keyboard::Num1:
     case sf::Keyboard::Num2:
     case sf::Keyboard::Num3:
     case sf::Keyboard::Num4:
     case sf::Keyboard::Num5:
     case sf::Keyboard::Num6:
     case sf::Keyboard::Num7:
     case sf::Keyboard::Num8:
     case sf::Keyboard::Num9:
         boxnum = keyevent.code - sf::Keyboard::Num0 - 1;
         break;*/



    // F10 takes a screenshot saves as JPG.
    case sf::Keyboard::F10:
        ge->screenshot();
        break;

    // If the M key is pressed toggle the display mode between Fill and Line.
    case sf::Keyboard::M:
        ge->changeMode();
        break;

    // Resets the window to (700, 500)
    case sf::Keyboard::R:
        ge->setSize(700, 500);
        break;

    // Sets the window to (600, 600)
    case sf::Keyboard::A:
        ge->setSize(600, 600);
        break;

    case sf::Keyboard::Num0:
        ge->setImage(0);
        break;

    case sf::Keyboard::Num1:
        ge->setImage(1);
        break;

    case sf::Keyboard::Num2:
        ge->setImage(2);
        break;

    case sf::Keyboard::Num3:
        ge->setImage(3);
        break;

    case sf::Keyboard::Num4:
        ge->setImage(4);
        break;

    case sf::Keyboard::Num5:
        ge->setImage(5);
        break;

    case sf::Keyboard::Num6:
        ge->setImage(6);
        break;

    case sf::Keyboard::Num7:
        ge->setImage(7);
        break;

    case sf::Keyboard::Num8:
        ge->setImage(8);
        break;

    case sf::Keyboard::Num9:
        ge->setImage(9);
        break;

    default:
        break;
    }
}
