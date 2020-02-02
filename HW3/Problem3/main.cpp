#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif // __APPLE__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>

#include "GraphicsEngine.h"
#include "UI.h"

/**
\mainpage Boxes Part 3

\tableofcontents

\section intro Introduction

This project attempts to do many different things to small boxes. You can drag, create, delete, move, scale, outline, recolor, and more.


\subsection options User Options

If no modifier keys are pressed:

- Escape:  Ends the program.
- M: Toggles between fill mode and line mode to draw the triangles.
- R: Resets the window size to 700 X 500.
- A: Resets the window size to 600 X 600.
- F10: Saves a screen shot of the graphics window to a png file.
- Left: Moves all selected boxes and their outlines to the left with state processing.
- Right: Moves all selected boxes and their outlines to the left with state processing.
- Up: Moves all selected boxes and outlines upward with state processing.
- Down: Moves all selected boxes and outlines downward with state processing.


If you mouse hover over a box:
- That box turns red.

If the control key is down:

- Left: Decreases the width of all selected boxes with their outlines.
- Right: Increases the width of all selected boxes with their outlines.
- Up: Decreases the height of all selected boxes with their outlines.
- Down: Increases the height of all selected boxes with their outlines.
- Left Click Inside of a box: Selects the box
- Left CLick outside of a box: Deselects all boxes
- CTRL + A : Selects all boxes.
- CTRL + Q : Deselects all boxes.
- Left Click and drag: Drags the clicked-on box in the direction of the mouse smoothly with no snapping.
- Right CLick: Creates a box with it's center at the origin of the mouse click. Randomly sized between .1-.3 widths and heights. Box is a random color too.

If the alt key is down:

- Left: Decreases the width and height of all of the selected boxes and their outlines.
- Right: Increases the width and height of all of the selected boxes and their outlines.


If the shift key is down:

- Left Click and Drag on a selected box: Drags all selected boxes on the screen identically.


NOTE: The Blue, green, and white functionality by pressing alt+B, G, etc has not been called from the UI in this project. You would
have to specify which box, it would not work just bringing it in from the previous excercise as we are to re-use our code from program 2. This was not discussed in the
description of excercise 3 so it has not been called from the UI and is not functional here.


I think that is all of the functionalities. If I havent listed them here, but they were in the requirements, they are
in the program. It does everything from number 3.

\note Note that the shader programs "PassThroughVert.glsl" and "PassThroughFrag.glsl" are expected
to be in the same folder as the executable.  Your graphics card must also be able to support
OpenGL version 3.3 to run this program.


---

\subsection copyright Copyright

\author    Ryan Clocker
\version   1.0
\date      2/23/2019
\copyright 2019


---

\subsection license License

GNU Public License

This software is provided as-is, without warranty of ANY KIND, either expressed or implied,
including but not limited to the implied warranties of merchant ability and/or fitness for a
particular purpose. The authors shall NOT be held liable for ANY damage to you, your computer,
or to anyone or anything else, that may result from its use, or misuse.
All trademarks and other registered names contained in this package are the property
of their respective owners.  USE OF THIS SOFTWARE INDICATES THAT YOU AGREE TO THE ABOVE CONDITIONS.

*/

/**
\file main.cpp
\brief Main driver for the program.

This is the main program driver that sets up the graphics engine and links it to
the user interface processor.

\author    Don Spickler
\version   1.2
\date      12/22/2017

*/


/**
\brief The Main function, program entry point.

\return Standard EXIT_SUCCESS return on successful run.

The main function, responsible for initializing OpenGL and setting up
the SFML interface for OpenGL.

*/

int main()
{
    //  Program setup variables.
    std::string programTitle = "Homework #3 - Program #3";
    GLint MinMajor = 3;
    GLint MinMinor = 3;
    GLint WindowWidth = 700;
    GLint WindowHeight = 500;
    bool DisplayInfo = true;

    //  Other variables
    GLint major;
    GLint minor;
    sf::Clock clock;
    sf::Time time = clock.restart();
    long framecount = 0;

    // Try core context of 10.10 (too advanced) and see what card will resort to.
    // For setting up OpenGL, GLEW, and check versions.
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "OpenGL Setup", sf::Style::Default,
                            sf::ContextSettings(24, 8, 4, 10, 10, sf::ContextSettings::Core));

    window.setVisible(false);

#ifndef __APPLE__
    // Turn on GLEW for Windows and Linux.
    glewExperimental = true;
    if (glewInit())
    {
        std::cerr << "\nUnable to initialize GLEW ... exiting. \n";
        exit(EXIT_FAILURE);
    }
#endif // __APPLE__

    //  Get major and minor OpenGL version from graphics card.
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    //  Check card version to minimum required version.
    bool versionOK = true;
    if (major < MinMajor)
        versionOK = false;
    else if (major == MinMajor && minor < MinMinor)
        versionOK = false;

    if (!versionOK)
    {
        std::cerr << "\nGraphics card OpenGL version is " << major << "." << minor << std::endl;
        std::cerr << "Program required OpenGL version is " << MinMajor << "." << MinMinor << std::endl;
        std::cerr << "Exiting\n";
        exit(EXIT_FAILURE);
    }

    //  Output OpenGL version to the console window.
    if (DisplayInfo)
    {
        std::cout << "\n";
        std::cout << "Version  = " << glGetString(GL_VERSION) << "\n";
        std::cout << "Major    = " << major << "\n";
        std::cout << "Minor    = " << minor << "\n";
        std::cout << "Vendor   = " << glGetString(GL_VENDOR) << "\n";
        std::cout << "Renderer = " << glGetString(GL_RENDERER) << "\n";
        std::cout << "\n";
    }
    //  Close setup window and context.
    window.close();

    //  Create graphics engine.
    GraphicsEngine ge(programTitle, major, minor, WindowWidth, WindowHeight);
    UI ui(&ge);

    // Start the Game/GUI loop
    while (ge.isOpen())
    {
        // Call the display function to do the OpenGL rendering.
        ge.display();

        // Process any events.
        ui.processEvents();

        //  Increment frame counts
        framecount++;

        //  Get Elapsed Time
        float timesec = clock.getElapsedTime().asSeconds();
        char titlebar[1000];

        //  If another second has elapsed, display the FPS and total FPS.
        if (timesec > 1.0)
        {
            float fps = framecount / timesec;
            sprintf(titlebar, "%s     FPS: %.2f", programTitle.c_str(), fps);
            ge.setTitle(titlebar);
            time = clock.restart();
            framecount = 0;
        }
    }

    return EXIT_SUCCESS;
}
