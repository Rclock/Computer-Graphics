#include "UI.h"

/**
\file UI.cpp
\brief User interface processor for the program. (not much)

\author    Ryan Clocker
\version   1.0
\date      3/5/2019

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

This method processes all events in the current SFML event queue and calls the
corresponding processing method.  At the end it calls the keyboard state processor
method, outside the event loop.

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

    // Process the state of the keyboard outside of event firing,
    keyboardStateProcessing();

}

/**
 \brief The function handles the keyboard input events from the user.

 \param keyevent --- The SFML key code for the key pressed.

\remark

If no modifier keys are pressed:

- M: Toggles between fill mode and line mode to draw the triangles.

- S: Vertical Sync off.
- F10: Saves a screen shot of the graphics window to a png file.
- ESC: Ends program.



*/

void UI::keyPressed(sf::Event::KeyEvent keyevent)
{
    int key = keyevent.code;

    switch (key)
    {
    // F10 takes a screenshot.
    case sf::Keyboard::F10:
        ge->screenshot();
        break;
    case sf::Keyboard::M:
        ge->changeMode();
        break;
    case sf::Keyboard::S:
        if (previous)
        {
            ge->setVerticalSyncEnabled(false);
            previous = false;
        }
        else
        {
            ge->setVerticalSyncEnabled(true);
            previous = true;
        }
        break;

    default:
        break;
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


}
