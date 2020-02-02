#include "Box.h"

/**
\file Box.cpp
\brief Simple rectangle object for drawing

Simple rectangle object with OpenGL data loading and drawing interfaces.  The data that
is being stored is a created in two separate arrays, one for vertices and the other for
color.  These blocks of data are transferred to a single array buffer on the graphics
card in two separate locations (that is the data is not intermixed) and hence we can
set up the reading pointers as having tightly packed data. There is also an index array
of 6 values {0, 3, 2, 0, 2, 1} that represent the vertices that will be drawn in two
triangles.  So one triangle will use vertices (0, 3, 2) and the other will use vertices
(0, 2, 1).

Note: Added functionality to change color of entire box to green, blue, white, or back to regular colors.

\author    Ryan Clocker
\version   1.0
\date     2/25/2019

*/

/**
 \brief Constructor

\param cx --- X position of the center.
\param cx --- Y position of the center.
\param w --- The width of the box.
\param h --- The height of the box.

Creates a box object, loads the attributes, generates the needed buffers
and IDs, and finally loads the object data up to the graphics card.

*/

Box::Box(GLfloat cx, GLfloat cy, GLfloat w, GLfloat h)
{
    width = w;
    height = 1;
    centerX = cx;
    centerY = cy;
    colors[0] = 1;
    colors[1] = 0;
    colors[2] = 0;
    colors[3] = 0;
    colors[4] = 1;
    colors[5] = 0;
    colors[6] = 0;
    colors[7] = 0;
    colors[8] = 1;
    colors[9] = 1;
    colors[10] = 1;
    colors[11] = 1;

    glGenVertexArrays(1, &BoxVAO);
    glGenBuffers(1, &ArrayBuffer);
    glGenBuffers(1, &BoxEBO);

    LoadDataToGraphicsCard();
}

/**
 \brief Destructor

Clears the graphics memory used for the rectangle.

*/

Box::~Box()
{
    glBindVertexArray(BoxVAO);
    glDeleteBuffers(1, &ArrayBuffer);
    glDeleteBuffers(1, &BoxEBO);
}

/**
 \brief Loads the box data to the graphics card.

Creates an array of vertex positions and colors from the data stored in the object,
creates an index array, allocates graphics memory for the arrays and loads the arrays
into graphics memory. Finally, it sets up pointers to the vertex and color data in
the two arrays.

*/

void Box::LoadDataToGraphicsCard()
{
    vPosition = 0;
    vColor = 1;

    // Vertex and Color data for the box, using two triangles.

    GLfloat vertices[8];

    vertices[0] = centerX - width/2;
    vertices[1] = centerY + height/2;

    vertices[2] = centerX + width/2;
    vertices[3] = centerY + height/2;

    vertices[4] = centerX + width/2;
    vertices[5] = centerY - height/2;

    vertices[6] = centerX - width/2;
    vertices[7] = centerY - height/2;

    GLuint indices[6] = {0, 3, 2, 0, 2, 1};

    // Bind (turn on) a vertex array.
    glBindVertexArray(BoxVAO);

    // Load the indexing array on the graphics card.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Bind (turn on) the vertex buffer (storage location).
    glBindBuffer(GL_ARRAY_BUFFER, ArrayBuffer);

    // Allocate space for the vertex and color data. Do not load data at this point.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);

    // Load the vertex data.
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // Load the color data.
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    // Setup vertex data position information. Note that the 5th parameter is 0, indicating tightly packed.
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Setup color data position information. Note that the 5th parameter is 0, indicating tightly packed.
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

    // Set position indexes for shader streams.
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

/**
 \brief Resets the width of the box.

\param w --- The width of the box.

Resets the width of the box and then calls the data loading method.

*/

void Box::setWidth(GLfloat w)
{
    width = w;
    LoadDataToGraphicsCard();
}

/**
 \brief Resets the height of the box.

\param h --- The height of the box.

Resets the height of the box and then calls the data loading method.

*/

void Box::setHeight(GLfloat h)
{
    height = h;
    LoadDataToGraphicsCard();
}

/**
 \brief Resets the center of the box.

\param cx --- X position of the center.

\param cx --- Y position of the center.

Resets the center of the box and then calls the data loading method.

*/

void Box::setCenter(GLfloat cx, GLfloat cy)
{
    centerX = cx;
    centerY = cy;
    LoadDataToGraphicsCard();
}

/**
 \brief Changes the color of the selected vertex.

\param v --- Number of the vertex to alter. Valid numbers are 0, 1, 2, 3, with 0 representing
the upper left vertex and then moving clockwise for the other three vertices.

\param r --- Red intensity for the vertex.

\param g --- Green intensity for the vertex.

\param b --- Blue intensity for the vertex.

Resets the color of vertex v to the new (r, g, b) value and calls the data loading method.

*/

void Box::setVertexColor(GLint v, GLfloat r, GLfloat g,GLfloat b)
{
    colors[3*v] = r;
    colors[3*v + 1] = g;
    colors[3*v + 2] = b;
    LoadDataToGraphicsCard();
}

/**
 \brief Returns the width of the rectangle.

*/

GLfloat Box::getWidth()
{
    return width;
}

/**
 \brief Returns the height of the rectangle.

*/

GLfloat Box::getHeight()
{
    return height;
}

/**
 \brief Populates the array c with the (x, y) coordinates of the center.

The array c must have length at least two.

*/

void Box::getCenter(GLfloat* c)
{
    c[0] = centerX;
    c[1] = centerY;
}

/**
 \brief Draws the rectangle to the OpenGL window.

*/

void Box::draw()
{
    glBindVertexArray(BoxVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoxEBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}
