#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "GraphicsEngine.h"

/**
\file UI.h

\brief Header file for UI.cpp

\author    Ryan Clocker
\version   1.0
\date      2/25/2019

*/

/**
\class UI

\brief The UI class handles the user interface event processing for the program.  It
contains a pointer to the graphics engine as the communication link to the graphics
and scene objects.

*/

class UI
{
private:
    GraphicsEngine* ge;  ///< Pointer to the GraphicsEngine that this processor is attached.
    bool mouseDown = false;///<Determines if the mouse is still clicked (for dragging stuff)
    //bool mouseDownOnBox;

    sf::Vector2f clickOrigin;///< Used to determine exactly where a drag was started, changes to the next mouse move position

    void keyPressed(sf::Event::KeyEvent keyevent);
    void keyboardStateProcessing();
    sf::Vector2f MouseToScreenConversion(sf::Vector2i mousePosition);
    void processMouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent);
    void processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent);
    void processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent);

public:
    UI(GraphicsEngine* graph);
    ~UI();

    void processEvents();
};

#endif // UI_H_INCLUDED
