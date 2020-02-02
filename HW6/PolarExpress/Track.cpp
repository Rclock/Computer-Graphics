#include "Track.h"

/**
\file Track.cpp
\brief Track class implementation. Basically makes 500 vertices along the given function to create the curve, loads data
to the graphics card using one simple VAO and buffer as a storage location. Uses a VertexData struct.

\author    Ryan Clocker
\version   1.0
\date      3/27/2019

*/

/**
\brief Constructor

Takes a radius as a parameter, makes a track at that desired radius distance from the origin all the way around. Uses a for loop
to intiialize points based on given function from assignment. Loads data to graphics card.

*/
Track::Track(float radius)
{

    vPosition = 0;
    vColor = 1;

    VertexData vertices[NumVertices];


    float j = 0;
    for (int i = 0; i < NumVertices; i++)//sets points along the curve.
    {
        vertices[i].position[0] = radius*cos(j);
        vertices[i].position[1] =  sin(3*(j)) - 2*cos(2*(j + 0.2)) + 2 *sin(7*(j));
        vertices[i].position[2] = radius*sin(j);
        vertices[i].color[0] = 1.0;
        vertices[i].color[1] = 1.0;
        vertices[i].color[2] = 1.0;
        j+= .01256;//500 divided by 6.28... (2pi)
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
                          GL_TRUE, sizeof(VertexData), BUFFER_OFFSET(0));

    // Setup vertex data position information.
    glVertexAttribPointer(vPosition, 3, GL_FLOAT,
                          GL_FALSE, sizeof(VertexData),
                          BUFFER_OFFSET(sizeof(vertices[0].color)));

    // Set position indexes for shader streams.
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);




}

Track::~Track()
{
    //dtor
}

void Track::draw()
{
    //glClear(GL_COLOR_BUFFER_BIT);

    // Draw the triangles.
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, NumVertices);
}

