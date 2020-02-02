#include "Star.h"
#include <cmath>
/**
\file Star.cpp
\brief




\author    Ryan Clocker
\version   1.0
\date      5/14/2019

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
Star::Star(int sun, int starno, int shooting)//set up all vertices, load everything. colors too.
{
    vPosition = 0;
    vColor = 1;
    srand(clock());
    GLfloat numpoints;
    if (shooting == 0)
        numpoints = rand()%10 + 10;
    else if (shooting == 1)
        numpoints = 15;
    else if (shooting  == 2)
        numpoints = 7;
    else if (shooting  == 3)
        numpoints = 20;

    points = numpoints*6;

    shootingStar = shooting;
    VertexData vertices[points];
    GLint pointindex = 0;


    GLfloat ptred, ptgreen, ptblue, ctred, ctgreen, ctblue;

    if (sun == 0)
    {
        ptred = (double)rand() / (double)RAND_MAX;
        ptgreen = (double)rand() / (double)RAND_MAX;
        ptblue = (double)rand() / (double)RAND_MAX;
        ctred = (double)rand() / (double)RAND_MAX;
        ctgreen = (double)rand() / (double)RAND_MAX;
        ctblue = (double)rand() / (double)RAND_MAX;
    }
    else
    {
        ptred = 1;
        ptgreen = 1;
        ptblue = 0;
        ctred = 1;
        ctgreen = 1;
        ctblue = 1;

    }
    if (shootingStar > 0)
    {
        //shootingStar = 1;
        ptred = 1;
        ptgreen = 1;
        ptblue = .8;
        ctred = 1;
        ctgreen = 1;
        ctblue = .8;

    }

    GLfloat outerRadius = 1;
    GLfloat innerRadius = .35;

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







    srand(clock());
    if (sun == 1)
    {
        position[0] = 0;
        position[1] = 0;
        centerX = 0;
        centerY = 0;

    }
    else
    {
        position[0] = 2 * (double)rand()/RAND_MAX - 1;
        position[1] = 2 * (double)rand()/RAND_MAX - 1;

    }
    //keep planets well on the screen
    if (position[0] > .7)
        position[0] = .7;
    if (position[0] < -.7)
        position[0] = .7;
    if (position[1] > .7)
        position[1] = 7;
    if (position[1] < -.7)
        position[1] = .7;

    //we do not want the planet to be inside the sun
    if (position[0] < .25 && position[0] > 0)
        position[0] = .25;
    if (position[1] < .25 && position[1] > 0)
        position[1] = .25;
    if (position[0] > -.25 && position[0] < 0)
        position[0] = .25;
    if (position[1] > -.25 && position[1] < 0)
        position[1] = .25;



    if (position[0] > 0 && position[1] > 0)
    {
        position[0] = (position[0] + 1/starno*.2);
        position[1] = (position[1] + 1/starno*.2);
    }
    if (position[0] > 0 && position[1] < 0)
    {
        position[0] = (position[0] + 1/starno*.2);
        position[1] = (position[1] - 1/starno*.2);
    }
    if (position[0] < 0 && position[1] > 0)
    {
        position[0] = (position[0] - 1/starno*.2);
        position[1] = (position[1] + 1/starno*.2);
    }
    if (position[0] < 0 && position[1] < 0)
    {
        position[0] = (position[0] - 1/starno*.2);
        position[1] = (position[1] - 1/starno*.2);
    }

    centerX = (position[0]);
    centerY = (position[1]);


    if (sun == 1)
    {
        velocity[0] = 0;
        velocity[1] = 0;
    }
    else
    {
        srand(clock());
        velocity[0] = rand() % 20 + 20;
        velocity[1] = rand() % 20 + 20;
    }




    srand(clock());



    scaleFactor = (double)rand() / (double)RAND_MAX;
    if (sun == 1)
    {
        scaleFactor = .3;
    }
    else
    {
        if (scaleFactor < .02)//keep them within a reasonable size
            scaleFactor = .02;

        if (scaleFactor > .1)
            scaleFactor = .1;
    }



    initialRot  = rand() % 200 + 100;

    if (shootingStar > 0)//for shooting stars
    {

        scaleFactor = .06;
        initialRot = 90000;
        position[0] = 2*(double)rand() / (double)RAND_MAX -1;

        position[1] = 2*(double)rand() / (double)RAND_MAX -1;
        if (position[1] > 0)
        {
            position[1] += 1;
            velocity[1] = -1*position[1]/2;
            //2*(double)rand() / (double)RAND_MAX - 1;

        }
        else
        {
            position[1]-= 1;
            velocity[1] = -1*position[1]/2;
        }
        if (position[0] > 0)
        {
            position[0] += 1.4;
            velocity[0] = -1*position[0]/2;
        }
        else
        {
            position[0]-= 1.4;
            velocity[0] = -1*position[0]/2;
        }
        centerX = position[0];
        centerY = position[1];
        //2*(double)rand() / (double)RAND_MAX - 1;
    }
    /*newstar->setInitialOuter(rand() % 50 + 10);*/
    initialOuterX = velocity[0];
    initialOuterY = velocity[1];








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
void Star::flipDirection(float time)
{
    velocityOuter = -1*initialRot*time;
}
void Star::flipRotation(float time)
{
    velocityRot = time*-1*initialRot;
}
void Star::updateDirection(float time, bool flip, float clock3)
{
    if (shootingStar < 1)
    {
        if (flip)
        {
            if (stoptime > 0)
                time = stoptime - time;
            else
                time = stoptime + time;

            std::cout << "flip=true, Rotating with: " << time << std::endl;

            velocity[0] = time*initialOuterX;
            velocity[1] = time*initialOuterY;
        }
        else
        {
            if (stoptime > 0)
                time = stoptime - time;
            else
                time = stoptime + time;

            std::cout << "flip = false, Rotating with: " << time << std::endl;

            velocity[0] = time*initialOuterX;
            velocity[1] = time*initialOuterY;

        }
    }
    if (shootingStar > 0) //for shooting star
    {
        position[0] = centerX + clock3*initialOuterX;
        position[1] = centerY + clock3*initialOuterY;
    }
    // std::cout << "Fliptime: " << time << std::endl;
}
void Star::updateRotation(float time, bool flip)
{
    if (flip)
        velocityRot = -1*time*initialRot;
    else
        velocityRot = time*initialRot;
}
GLfloat Star::getST()
{
    return stoptime;
}
GLfloat Star::getPosX()
{
    return position[0];
}
GLfloat Star::getPosY()
{
    return position[1];
}
GLfloat Star::getScale()
{
    return scaleFactor;
}
GLfloat Star::getRot()
{
    return velocityRot;
}
GLfloat Star::getOuter()
{
    return velocityOuter;
}

GLfloat Star::getVelocityX()
{
    return velocity[0];
}
GLfloat Star::getVelocityY()
{
    return velocity[1];
}
GLfloat Star::getInitialOuterX()
{
    return initialOuterX;
}
GLfloat Star::getInitialOuterY()
{
    return initialOuterY;
}

void Star::setCenterX(float num)
{
    centerX = num;
}
void Star::setCenterY(float num)
{
    centerY = num;
}
void Star::setST(float num)
{
    stoptime = num;
}
void Star::setPosX(float num)
{
    position[0] = num;
}
void Star::setPosY(float num)
{
    position[1] = num;
}
void Star::setScale(float num)
{
    scaleFactor = num;
}
void Star::setVRot(float num)
{
    velocityRot = num;
}
void Star::setVOuter(float num)
{
    velocityOuter = num;
}
void Star::setVelocityX(float num)
{
    velocity[0] = num;
}
void Star::setVelocityY(float num)
{
    velocity[1] = num;
}
void Star::setInitialOuterX(float num)
{
    initialOuterX = num;
}
void Star::setInitialOuterY(float num)
{
    initialOuterY = num;
}
void Star::setInitialRot(float num)
{
    initialRot = num;
}
int Star::getShooting()
{
    return shootingStar;
}
GLfloat Star::getCenterX()
{
    return centerX;
}
GLfloat Star::getCenterY()
{
    return centerY;
}




