#include "Sphere.h"

/**
\file Sphere.cpp
\brief Implementation of the Sphere class.

\author    Don Spickler
\version   1.2
\date      1/24/2018

*/

/**
\brief Constructor

Creates the data for the sphere and loads it to the graphics card.

*/

Sphere::Sphere()
{
    vPosition = 0;
    vColor = 1;
    vNormal = 2;
    vTex = 3;

    red = 1.0;
    green = 1.0;
    blue = 1.0;

    lon = 10;
    lat = 10;

    reload = GL_FALSE;

    LoadDataToGraphicsCard();
}

/**
\brief Destructor

Clears the buffer data on the graphics card.

*/

Sphere::~Sphere()
{
    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
}

/**
\brief Creates the data needed for the plane and loads it to the graphics card.

*/

void Sphere::LoadDataToGraphicsCard()
{
    GLuint indices[6*(lon)*(lat)];
    GLfloat points[4*(lon+1)*(lat+1)];
    GLfloat colors[4*(lon+1)*(lat+1)];
    GLfloat normalvectors[3*(lon+1)*(lat+1)];
    GLfloat tex[2*(lon+1)*(lat+1)];

    for (int i = 0; i < lon+1; i++)
        for (int j = 0; j < lat+1; j++)
        {
            tex[2*i*(lat+1)+2*j] = 1.0*i/lon;
            tex[2*i*(lat+1)+2*j+1] = 1.0*j/lat;
        }

    for (int i = 0; i < lon+1; i++)
        for (int j = 0; j < lat+1; j++)
        {
            GLfloat theta = 2*PI*((float)i/lon);
            GLfloat psi = -PI/2 + PI*((float)j/lat);

            points[4*i*(lat+1)+4*j] = cos(psi)*cos(theta);
            points[4*i*(lat+1)+4*j+1] = cos(psi)*sin(theta);
            points[4*i*(lat+1)+4*j+2] = sin(psi);
            points[4*i*(lat+1)+4*j+3] = 1;
        }

    for (int i = 0; i < lon+1; i++)
        for (int j = 0; j < lat+1; j++)
        {
            colors[4*i*(lat+1)+4*j] = red;
            colors[4*i*(lat+1)+4*j+1] = green;
            colors[4*i*(lat+1)+4*j+2] = blue;
            colors[4*i*(lat+1)+4*j+3] = 1;
        }

    for (int i = 0; i < lon+1; i++)
        for (int j = 0; j < lat+1; j++)
        {
            GLfloat theta = 2*PI*((float)i/lon);
            GLfloat psi = -PI/2 + PI*((float)j/lat);

            normalvectors[3*i*(lat+1)+3*j] = cos(psi)*cos(theta);
            normalvectors[3*i*(lat+1)+3*j+1] = cos(psi)*sin(theta);
            normalvectors[3*i*(lat+1)+3*j+2] = sin(psi);
        }

    int indexarraypos = 0;
    for (int i = 0; i < lon; i++)
        for (int j = 0; j < lat; j++)
        {
            indices[indexarraypos++] = i*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j+1;
            indices[indexarraypos++] = i*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j+1;
            indices[indexarraypos++] = i*(lat+1)+j+1;;
        }

    if (!reload)
        glGenVertexArrays(1, &vboptr);

    glBindVertexArray(vboptr);

    if (!reload)
        glGenBuffers(1, &eboptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    if (!reload)
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

    reload = GL_TRUE;
}


/**
\brief Resets the data array sizes.

\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Sphere::resetArraySize(GLint ilon, GLint ilat)
{
    lon = ilon;
    lat = ilat;

    LoadDataToGraphicsCard();
}


/**
\brief Sets the location values and calls the load method.

\param v --- Location of the vertex data.
\param c --- Location of the color data.
\param n --- Location of the normal data.
\param t --- Location of the texture data.

*/

void Sphere::LoadDataToGraphicsCard(GLint v, GLint c, GLint n, GLint t)
{
    vPosition = v;
    vColor = c;
    vNormal = n;
    vTex = t;

    LoadDataToGraphicsCard();
}

/**
\brief Draws the sphere.

*/

void Sphere::draw()
{
    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glDrawElements(GL_TRIANGLES, 6*(lon)*(lat), GL_UNSIGNED_INT, NULL);
}

/**
\brief Sets the color of the plane and reloads the data.

\param r --- Red component.
\param g --- Green component.
\param b --- Blue component.

*/

void Sphere::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;

    LoadDataToGraphicsCard();
}
