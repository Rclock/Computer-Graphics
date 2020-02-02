#include "Plane.h"

/**
\file Plane.cpp
\brief Implementation of the Plane class.

\author    Don Spickler
\version   1.2
\date      1/24/2018

*/

/**
\brief Constructor

Creates the data for the plane and loads it to the graphics card.

*/

Plane::Plane()
{
    vPosition = 0;
    vColor = 1;
    vNormal = 2;
    vTex = 3;

    red = 1.0;
    green = 1.0;
    blue = 1.0;

    LoadDataToGraphicsCard();
}

/**
\brief Destructor

Clears the buffer data on the graphics card.

*/

Plane::~Plane()
{
    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
}

/**
\brief Creates the data needed for the plane and loads it to the graphics card.

*/

void Plane::LoadDataToGraphicsCard()
{
    GLuint indices[6];
    GLfloat points[16];
    GLfloat colors[16];
    GLfloat normalvectors[12];
    GLfloat tex[8];

    points[0] = -1;
    points[1] = -1;
    points[2] = 0;
    points[3] = 1;
    points[4] = 1;
    points[5] = -1;
    points[6] = 0;
    points[7] = 1;
    points[8] = 1;
    points[9] = 1;
    points[10] = 0;
    points[11] = 1;
    points[12] = -1;
    points[13] = 1;
    points[14] = 0;
    points[15] = 1;

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 3;
    indices[3] = 3;
    indices[4] = 1;
    indices[5] = 2;

    colors[0] = red;
    colors[1] = green;
    colors[2] = blue;
    colors[3] = 1;
    colors[4] = red;
    colors[5] = green;
    colors[6] = blue;
    colors[7] = 1;
    colors[8] = red;
    colors[9] = green;
    colors[10] = blue;
    colors[11] = 1;
    colors[12] = red;
    colors[13] = green;
    colors[14] = blue;
    colors[15] = 1;

    normalvectors[0] = 0;
    normalvectors[1] = 0;
    normalvectors[2] = 1;
    normalvectors[3] = 0;
    normalvectors[4] = 0;
    normalvectors[5] = 1;
    normalvectors[6] = 0;
    normalvectors[7] = 0;
    normalvectors[8] = 1;
    normalvectors[9] = 0;
    normalvectors[10] = 0;
    normalvectors[11] = 1;

    //  Lower Left  (-1, -1)
    tex[0] = 0;
    tex[1] = 1;

    //  Lower Right  (1, -1)
    tex[2] = 1;
    tex[3] = 1;

    //  Upper Right  (1, 1)
    tex[4] = 1;
    tex[5] = 0;

    //  Upper Left  (-1, 1)
    tex[6] = 0;
    tex[7] = 0;

    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glGenBuffers(1, &eboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normalvectors) + sizeof(tex), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normalvectors), normalvectors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normalvectors), sizeof(tex), tex);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(normalvectors) + sizeof(colors) + sizeof(points)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(colors) + sizeof(points)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);
}


/**
\brief Sets the location values and calls the load method.

\param v --- Location of the vertex data.
\param c --- Location of the color data.
\param n --- Location of the normal data.
\param t --- Location of the texture data.

*/

void Plane::LoadDataToGraphicsCard(GLint v, GLint c, GLint n, GLint t)
{
    vPosition = v;
    vColor = c;
    vNormal = n;
    vTex = t;

    LoadDataToGraphicsCard();
}

/**
\brief Draws the plane.

*/

void Plane::draw()
{
    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

/**
\brief Sets the color of the plane and reloads the data.

\param r --- Red component.
\param g --- Green component.
\param b --- Blue component.

*/

void Plane::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;

    LoadDataToGraphicsCard();
}

/*  Default texture positions

    //  Lower Left  (-1, -1)
    tex[0] = 0;
    tex[1] = 1;

    //  Lower Right  (1, -1)
    tex[2] = 1;
    tex[3] = 1;

    //  Upper Right  (1, 1)
    tex[4] = 1;
    tex[5] = 0;

    //  Upper Left  (-1, 1)
    tex[6] = 0;
    tex[7] = 0;

*/
