#include "Star.h"
#include <cmath>
/**
\file Star.cpp
\brief




\author    Ryan Clocker
\version   1.0
\date      2/23/2019

*/


/**
\brief This is used to initialize data and send it to the graphics card, initially. (before linear transformations)
*/
struct VertexData
{
    GLfloat color[3];    ///< 3-D color array for (r, g, b) colors.
    GLfloat position[4]; ///< 4-D position array for (x, y, z, w) coordinates.
};
/**
 \brief
 Function to generate y values of points around a circle. This is used to create the stars (2 circles)
 \param index --- index of point on the star we are drawing.
 \param radius --- radius of the circle we are drawing, inner or outer.
 \param k --- Number of points on the star. This is how we know how much to translate by.

*/
float generateX(float index, float radius, bool trans, GLint k)
{
    float x;
    if (trans)
    {
        x = radius * cos(index * (2*PI/k)+PI/2 + PI/k);
    }
    else
    {
        x = radius * cos(index * (2*PI/k)+PI/2);
    }

    return x;
}

/**
 \brief
 Function to generate y values of points around a circle. This is used to create the stars (2 circles)
 \param index --- index of point on the star we are drawing.
 \param radius --- radius of the circle we are drawing, inner or outer.
 \param k --- Number of points on the star. This is how we know how much to translate by.

*/
float generateY(float index, float radius, bool trans, GLint k)
{
    float y;
    if (trans)
    {
        y = radius * sin(index * (2*PI/k)+PI/2+PI/k);
    }
    else
    {
        y = radius * sin(index * (2*PI/k)+PI/2);
    }

    return y;
}

/**
\brief Constructor

Takes the parameters specified on homework sheet and generates a star with the preset values. Loads data to graphics card too.

*/
Star::Star(int numpoints, GLfloat outerRadius, GLfloat innerRadius,
           GLfloat ptred, GLfloat ptgreen, GLfloat ptblue,
           GLfloat ctred, GLfloat ctgreen, GLfloat ctblue)//set up all vertices, load everything. colors too.
{
    vPosition = 0;
    vColor = 1;
    srand(clock());

    numpoints = rand()%15 + 5;

    points = numpoints*6;


    VertexData vertices[points];
    GLint pointindex = 0;

    ptred = (double)rand() / (double)RAND_MAX;
    ptgreen = (double)rand() / (double)RAND_MAX;
    ptblue = (double)rand() / (double)RAND_MAX;
    ctred = (double)rand() / (double)RAND_MAX;
    ctgreen = (double)rand() / (double)RAND_MAX;
    ctblue = (double)rand() / (double)RAND_MAX;

    for (int i = 0; i < points; i= i + 6)//loop to initialize vertices, one point at a time. (6 vertices)
    {
        for (int j = 0; j < 3; j++)//fills color of vertex
        {
            vertices[i].color[j] = ptred;
            if (j > 0)//make the rest zero for a red vertex
                vertices[i].color[j] = ptgreen;
            if (j > 1)
                vertices[i].color[j] = ptblue;
        }
        vertices[i].position[0] = generateX(pointindex, outerRadius, false, numpoints);//outer vertex coords, red colored.
        vertices[i].position[1] = generateY(pointindex, outerRadius, false, numpoints);

        for (int j = 0; j < 3; j++)//colors of origin vertex
        {
            vertices[i+1].color[j] = ctred;
            if (j > 0)//make the rest zero for a red vertex
                vertices[i+1].color[j] = ctgreen;
            if (j > 1)
                vertices[i+1].color[j] = ctblue;
            //center red, blue, etc...Come back to here!
        }
        vertices[i+1].position[0] = 0;//origin vertex, white colored.
        vertices[i+1].position[1] = 0;

        for (int j = 0; j < 3; j++)//initialized color to white for inner vertex.
        {
            vertices[i+2].color[j] = ptred;
            if (j > 0)//make the rest zero for a red vertex
                vertices[i+2].color[j] = ptgreen;
            if (j > 1)
                vertices[i+2].color[j] = ptblue;
        }
        vertices[i+2].position[0] = generateX(pointindex, innerRadius, true, numpoints);//inner vertex
        vertices[i+2].position[1] = generateY(pointindex, innerRadius, true, numpoints);

        //SECOND TRIANGLE STARTS HERE
        for (int j = 0; j < 3; j++)//colors of inner vertex
        {
            vertices[i+3].color[j] = ptred;
            if (j > 0)//make the rest zero for a red vertex
                vertices[i+3].color[j] = ptgreen;
            if (j > 1)
                vertices[i+3].color[j] = ptblue;
        }
        vertices[i+3].position[0] = generateX(pointindex-1, innerRadius, true, numpoints);//connect to previous inner vertex
        vertices[i+3].position[1] = generateY(pointindex-1, innerRadius, true, numpoints);

        for (int j = 0; j < 3; j++)//colors of new outer point
        {
            vertices[i+4].color[j] = ptred;

            if (j > 0)//make the rest zero for a red vertex
                vertices[i+4].color[j] = ptgreen;
            if (j > 1)
                vertices[i+4].color[j] = ptblue;
        }

        vertices[i+4].position[0] = generateX(pointindex, outerRadius, false, numpoints);//outer point to connect with
        vertices[i+4].position[1] = generateY(pointindex, outerRadius, false, numpoints);
        for (int j = 0; j < 3; j++)//colors of origin vertex
        {
            vertices[i+5].color[j] = ctred;
            if (j > 0)//make the rest zero for a red vertex
                vertices[i+5].color[j] = ctgreen;
            if (j > 1)
                vertices[i+5].color[j] = ctblue;
        }
        vertices[i+5].position[0] = 0;//origin points for second triangle
        vertices[i+5].position[1] = 0;
        pointindex++;
    }

    // Generate and bind (turn on) a vertex array.


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind (turn on) a buffer (storage location).
    glGenBuffers(1, &ArrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ArrayBuffer);

    // Transfer the vertex information to the buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_STATIC_DRAW);

    // Setup color data position information.
    glVertexAttribPointer(vColor, 3, GL_FLOAT,
                          GL_TRUE, sizeof(VertexData), BUFFER_OFFSET(0));

    // Setup vertex data position information.
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,
                          GL_FALSE, sizeof(VertexData),
                          BUFFER_OFFSET(sizeof(vertices[0].color)));

    // Set position indexes for shader streams.
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);




}

/**

 \brief Destructor
 Destructor for star objects
 */
Star::~Star()
{
    glBindVertexArray(VAO);
    glDeleteBuffers(1, &ArrayBuffer);

}
/**
 \brief draws stars to screen using triangles method
 */

void Star::draw()
{
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, points);
}








