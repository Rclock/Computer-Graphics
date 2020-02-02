#include "Circle.h"
#include <cmath>
/**
\file Circle.cpp
\brief Circle object with color, scale factor, random speed.



Each circle is set up with a random scale factor to be used with a scaling matrix. Each circle has a randomly generated
speed associated with it, which updates it's position. Each circle is intialized with a random vec4 to send to the fragment
shader also, which is changed if it bounces. Every circle starts at the center.

\author    Ryan Clocker
\version   1.0
\date      2/23/2019

*/

/**
\brief Constructor
Sets up a circle with a random scaling factor, colorVec to send to fragment shader when it bounces, speed vector, etc.

Also loads data to graphics card.

*/

Circle::Circle()//set up all vertices, load everything. colors too.
{
    vPosition = 0;
    vColor = 1;
    radius = 1;//
    GLfloat red, green, blue;
    centerX = 0;// 2 * (double)rand() / (double)RAND_MAX - 1;
    centerY = 0; // 2 * (double)rand() / (double)RAND_MAX - 1;

    scalingFactor = (double)rand() / (double)RAND_MAX;
    if (scalingFactor < .15)//keep them within a reasonable size
        scalingFactor = .1;

    if (scalingFactor > .5)
        scalingFactor = .5;


    srand (clock());


    colorVec = glm::vec4((double)rand() / ((double)RAND_MAX), (double)rand() / ((double)RAND_MAX), (double)rand() / ((double)RAND_MAX), 1);
    speed[0] = 2*(GLfloat)rand()/RAND_MAX - 1;
    speed[1] = 2*(GLfloat)rand()/RAND_MAX - 1;


    starttime = 0;



    std::cout << "Random origin x: " << centerX << std::endl;
    std::cout << "Random origin y: " << centerY << std::endl;



    for (int i = 0; i < 100; i++)
    {
        vertices[i].position[0] = radius * cos(i * (2*PI/100));
        vertices[i].position[1] = radius * sin(i * (2*PI/100));
        vertices[i].color[0] = red;
        vertices[i].color[1] = green;
        vertices[i].color[2] = blue;
    }



    glGenVertexArrays(1, &CircleVAO);
    glBindVertexArray(CircleVAO);

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
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,
                          GL_FALSE, sizeof(VertexData),
                          BUFFER_OFFSET(sizeof(vertices[0].color)));

    // Set position indexes for shader streams.
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);




}

/**
\brief Destructor

Clears the graphics memory used for the rectangle.

*/

Circle::~Circle()
{
    glBindVertexArray(CircleVAO);
    glDeleteBuffers(1, &ArrayBuffer);
    glDeleteBuffers(1, &CircleEBO);
}






/**
\brief Resets the width of the circle.

\param w --- The width of the circle.

Resets the width of the box and then calls the data loading method.

*/

void Circle::setRadius(GLfloat r)
{
    radius = r;

}





/**
\brief Resets the center of the bubble.

\param cx --- X position of the center.

\param cx --- Y position of the center.

Resets the center of the box and then calls the data loading method.

*/

void Circle::setCenter(GLfloat x, GLfloat y)
{
    centerX = x;
    centerY = y;

}

/**
\brief Returns the radius of the circle.

*/

GLfloat Circle::getRadius()
{
    return radius;
}


/**
\brief Populates the array c with the (x, y) coordinates of the center.

The array c must have length at least two.

*/

void Circle::getCenter(GLfloat* c)
{
    c[0] = centerX;
    c[1] = centerY;
}
/**
\brief Populates the array c with the (x, y) coordinates of the last position of circle.

The array c must have length at least two.

*/
void Circle::getPosition(GLfloat *c)
{
    c[0] = position[0];
    c[1] = position[1];
}


/**
\brief Draws the rectangle to the OpenGL window.

*/

void Circle::draw()
{
    glBindVertexArray(CircleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0,100);

}
/**

\brief Updating circle position.

This function does a ton of work for this program. It takes care of the bounce, keeps track and modifies the
position of the box, takes care of initializing random color vec4 for a bounce.

 \param float time --- Used for determining elapsed time. comes from animclock.
\param GLfloat bounds[] --- pass in the screen bounds for the program so it can check for a bounce.

*/
GLint Circle::updatePosition(float time, GLfloat bounds[])
{
    float elptime = time - starttime;



    if (position[0] >= bounds[1] - radius*scalingFactor)//1*scaling factor???
    {
        //right side x case, only need to reverse the x component. keep y component the same
        bounce = true;
        centerX = bounds[1]-.0001 -radius*scalingFactor;
        centerY = position[1];
        speed[0] = -1*speed[0];




    }

    if (position[0] <= bounds[0] + radius*scalingFactor)//1*scaling factor???
    {
        bounce = true;
        centerX = bounds[0] + .001 +radius*scalingFactor;//resets center to slightly inside the window
        centerY = position[1];
        speed[0] = -1*speed[0];



    }

    if (position[1] <= bounds[2] + radius*scalingFactor)//1*scaling factor???
    {
        bounce = true;
        centerX = position[0];
        centerY = bounds[2] + .001+radius*scalingFactor;
        speed[1] = -1*speed[1];



    }

    if (position[1] >= bounds[3] - radius*scalingFactor)//1*scaling factor???
    {
        bounce = true;
        centerX = position[0];
        centerY = bounds[3] - .001 -radius*scalingFactor;
        speed[1] = -1*speed[1];



    }





    if (bounce)
    {
        starttime = time;
        elptime = time - starttime;

        bounce = false;
        position[0] = centerX + speed[0]*elptime;
        position[1] = centerY + speed[1]*elptime;
        colorVec = glm::vec4((double)rand() / ((double)RAND_MAX), (double)rand() / ((double)RAND_MAX), (double)rand() / ((double)RAND_MAX), 1);

        return 1;


    }
    else
    {
        position[0] = centerX + speed[0]*elptime;
        position[1] = centerY + speed[1]*elptime;
        return 0;
    }

}
/**

\brief Returns scale factor of a circle.
*/
GLfloat Circle::getScaleF()
{
    return scalingFactor;
}





