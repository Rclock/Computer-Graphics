#include "Ties.h"

/**
\file Ties.cpp
\brief Ties class implementation. Makes 500 ties between 500 points on the two tracks. Uses two points for each tie.
Picks a point from one track and corresponding point from the other track, draws a tie between them using
GL_LINES drawing mode. Loads data to graphics card using a simple VAO and buffer for storage.

\author    Ryan Clocker
\version   1.0
\date      3/27/2019

*/

/**
\brief Constructor

Constructs many ties to go between the two curves that we know are predetermined to be at radius values of 9.6 and 10.
Takes a point from the 9.6 radius curve and pairs it up with the 10.0 radius curve so they are one after the other
in the vertices array, this makes it so using GL_LINES will connect them as a segment and then move to the next pair of
points to construct another segment or "tie" between.

*/
Ties::Ties()
{

    vPosition = 0;
    vColor = 1;

    VertexD vertices[NumVertices];

    float radius;
    radius = 9.6;
    float j = 0;
    for (int i = 0; i < NumVertices; i+=2)
    {
        radius = 9.6;
        vertices[i].position[0] = radius*cos(j);
        vertices[i].position[1] =  sin(3*(j)) - 2*cos(2*(j + 0.2)) + 2 *sin(7*(j));
        vertices[i].position[2] = radius*sin(j);
        vertices[i].color[0] = 1.0;
        vertices[i].color[1] = 1.0;
        vertices[i].color[2] = 1.0;
        radius = 10;//pick point from second curve
        vertices[i+1].position[0] = radius*cos(j);
        vertices[i+1].position[1] =  sin(3*(j)) - 2*cos(2*(j + 0.2)) + 2 *sin(7*(j));
        vertices[i+1].position[2] = radius*sin(j);
        vertices[i+1].color[0] = 1.0;
        vertices[i+1].color[1] = 1.0;
        vertices[i+1].color[2] = 1.0;
        j+= .01256;//500/2*PI
        std::cout << "Value of j: " << j << std::endl;
        std::cout << "Value of i: " << i << std::endl;
    }


// Generate and bind (turn on) a vertex array.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind (turn on) a buffer (storage location).
    glGenBuffers(1, &Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);

    // Transfer the vertex information to the buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_STATIC_DRAW);

    // Setup color data position information.
    glVertexAttribPointer(vColor, 3, GL_FLOAT,
                          GL_TRUE, sizeof(VertexD), BUFFER_OFFSET(0));

    // Setup vertex data position information.
    glVertexAttribPointer(vPosition, 3, GL_FLOAT,
                          GL_FALSE, sizeof(VertexD),
                          BUFFER_OFFSET(sizeof(vertices[0].color)));

    // Set position indexes for shader streams.
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);




}

Ties::~Ties()
{
    //dtor
}

void Ties::draw()
{
    //glClear(GL_COLOR_BUFFER_BIT);

    // Draw the triangles.
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, NumVertices);
}


