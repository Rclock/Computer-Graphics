#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/glu.h>
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
\mainpage 3D Planets and a Spaceship

\tableofcontents

\section intro Introduction

This program includes many different planets along with a sun and a spaceship. You can view everything through a spherical camera or take
the more interesting route and fly the spaceship with one of it's three cameras.

Notable features:
- Found some cool oldschool fonts online from old IBM/VGA devices and MS-DOS machines. Used one of those fonts for all of the freetype.
- The Sun has it's own lighting, it can be turned on or off
- If you are in the forward camera, the spaceship engine noise will dampen as you are inside now
- The 3 planets in a row have textures that can be changed to any texture in the program by pressing T,
if you fly close enough to them there will be green text that shows up on the screen to tell you this.
- There are random asteroids and they have their own sound associated with them
- 3 different flying cameras, one keeps tight behind the ship, one lets you roll the ship without changing your camera's
orientation (does not update your up vector), one is the same but a closer up view from the ship instead of behind
- There is a controls menu that will show up on the screen if you press C, it shows ship controls and general program
UI that is important.
- The freetype X Y Z letters that sit above the coordinates for the ship view and position adapt and adjust themselves
when the values increase (otherwise they would not be lined up anymore when the ship values become large or small)
- Sun lights have a linear attenuation factor, they will fade out as you move further from the sun
- YPR values in degrees for the ship are displayed on screen and updated
- Spaceship will get further and further infront of you as you increase speed. Same for decreasing but vice versa.
Power is displayed on the screen.
- You can freeze the planets to look closer at textures. Also there are things out in space for you to go and fly to and check out.


\subsection options User Options

- Escape:  Ends the program.
- M: Toggles between fill mode and line mode to draw the triangles.
- F10: Saves a screen shot of the graphics window to a png file.
- 0: Spherical camera. Starts centered at the sun.
- 1: This is the flight camera. This camera will sit tight behind the spaceship and follow all of it's orientation changes.
- 2: Free flight camera. This camera will not follow the up vector of the ship when it rolls so you can watch it roll around things.
- 3: Forward camera. Similar to the free flight but you are viewing form closer up on the ship.
- T: Cycles textures on the three row planets out in space.
- O: Orbits the spaceship. It will put the spaceship in orbit around the sun, can go into the spherical camera and watch the sun's lighting on the ship this way too.
- I: Turns ship info on or off. This is the displayed info when flying the ship, the ship data like power, flight direction, ship position, and ship orientation.
- C: Turns the control menu on or off. This shows most the the important UI on the screen with freetype.
- A: Draws the axes, toggles on/off.
- F: Freezes the planets. Toggles on/off.
- L: Turns the sun lights on or off.
- S: Stops the ship, no matter how fast you are going forward or backward.
- V: Turns VSync on or off.

If the spherical camera is on:

If no modifier keys are pressed:

- Left: Increases the camera's theta value.
- Right: Decreases the camera's theta value.
- Up: Increases the camera's psi value.
- Down: Decreases the camera's psi value.

If the control or Z key is down:

- Up: Decreases the camera's radius.
- Down: Increases the camera's radius.

If any of the spaceship's cameras are on:

If no modifier keys are pressed:

- Left: Increases the ship's yaw.
- Right: Decreases the ship's yaw.
- Up: Increases the ship's pitch.
- Down: Decreases the ship's pitch.

If the control or Z key is down:

- Left: Increases the ship's roll.
- Right: Decreases the ship's roll.
- Up: Adds power to the spaceship, moving forward.
- Down: Decreases power, retracting the ship and moving it backward if power < 0%.

If the shift or S key is down:

- Left: Strafes the ship left.
- Right: Strafes the ship right.
- Up: Strafes ship up.
- Down: Strafes the ship up.

If the Alt or the X key is down then the movement is on one of the lights:

If no modifier keys are pressed:

- Left: Increases the light's theta value.
- Right: Decreases the light's theta value.
- Up: Increases the light's psi value.
- Down: Decreases the light's psi value.

If the control or Z key is down:

- Up: Decreases the light's radius.
- Down: Increases the light's radius.

If the spherical camera is currently selected, a click and drag with the left mouse
button will alter the theta and psi angles of the spherical camera to give the impression
of the mouse grabbing and moving the coordinate system.

\note Note that the shader programs "VertexShaderLightingTexture.glsl" and "PhongMultipleLightsAndTexture.glsl"
are expected to be in the same folder as the executable.  Your graphics card must also be
able to support OpenGL version 3.3 to run this program.


---

\subsection copyright Copyright

\author    Ryan Clocker
\version   1.3
\date      5/14/2019
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

\author    Ryan Clocker
\version   1.2
\date      5/14/2019

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
    std::string programTitle = "3D Planets";
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
