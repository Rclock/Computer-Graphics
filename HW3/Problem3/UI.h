#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "GraphicsEngine.h"

/**
\file UI.h

\brief Header file for UI.cpp

\author    Ryan Clocker
\version   1.0
\date      2/24/2019
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
    bool mouseDown = false;///< Is the mouse clicked
    bool ctrldown = false;///< Is the control down

    sf::Vector2f clickOrigin;///< Used for the drag functions, determines where last event was. Used in arithmetic.

    void keyPressed(sf::Event::KeyEvent keyevent);///< PRocess keypressed events
    void keyboardStateProcessing(); ///<Process state of keyboard
    sf::Vector2f MouseToScreenConversion(sf::Vector2i mousePosition); ///<Convert from pixel to screen

    void processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent);///<Processes button released events, sets booleans.

    void processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent);///< Process all mousemoved events. Recoloring, etc.

public:
    UI(GraphicsEngine* graph);
    ~UI();
    bool ctrlDownPublic = false;
    void processEvents();
};

#endif // UI_H_INCLUDED
