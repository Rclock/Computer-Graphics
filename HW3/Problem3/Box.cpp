#include "Box.h"
#include <cmath>
/**
\file Box.cpp
\brief Rectangle object with yellow outlining, selecting, deselecting, and dragging capabilities.



This rectangle object has capabilities for a yellow outline to be drawn with it's own arrays and coordinates, has functionality
to "select" itself (yellow outline), functionality to compare mouse coordinates to all of the boxes, deselect itself, and
modify center coordinates of yellow outline and box itself in order to "drag" the box and outline. Two index arrays, one for the box
and one for the outline. One of size 6 and one of size 4. One draws 2 triangles, one draws a line loop across 4 vertices to make
a box outline. The load functions Load data to the graphics card in a similar fashion to how we have done it in the past.

\author    Ryan Clocker
\version   1.0
\date      2/23/2019

*/

/**
\brief Constructor

\param cx --- X position of the center.
\param cx --- Y position of the center.
\param w --- The width of the box.
\param h --- The height of the box.

Creates a box object, loads the attributes, generates the needed buffers
and IDs, and finally loads the object data up to the graphics card.

Also sets up a yellow array of vertex colors for the outline of the boxes to be drawn. Sets the yellow outline
Width and height to be a tad larger than the regular box so we can see it. Generates Vertex arrays and buffers once for
box and once for yellow outline. Loads box data and yellow outline data to graphics card in seperate functions.

*/

Box::Box(GLfloat cx, GLfloat cy, GLfloat w, GLfloat h)
{
    width = w;
    height = h;
    centerX = cx;
    centerY = cy;

    srand (clock());
    r = (double)rand() / ((double)RAND_MAX) * .9 + 0.1;//randomizes color each time
    g = (double)rand() / ((double)RAND_MAX) * .9 + 0.1;
    b = (double)rand() / ((double)RAND_MAX) * .9 + 0.1;
    colors[0] = r;
    colors[1] = g;
    colors[2] = b;
    colors[3] = r;
    colors[4] = g;
    colors[5] = b;
    colors[6] = r;
    colors[7] = g;
    colors[8] = b;
    colors[9] = r;
    colors[10] = g;
    colors[11] = b;
    glGenVertexArrays(1, &BoxVAO);
    glGenBuffers(1, &ArrayBuffer);
    glGenBuffers(1, &BoxEBO);

    colorsYellow[0] = 1;
    colorsYellow[1] = 1;
    colorsYellow[2] = 0;
    colorsYellow[3] = 1;
    colorsYellow[4] = 1;
    colorsYellow[5] = 0;
    colorsYellow[6] = 1;
    colorsYellow[7] = 1;
    colorsYellow[8] = 0;
    colorsYellow[9] = 1;
    colorsYellow[10] = 1;
    colorsYellow[11] = 0;

    yellowHeight = height+.01;
    yellowWidth = width+.01;

    glGenVertexArrays(1, &YellowVAO);
    glGenBuffers(1, &ArrayBufferYellow);
    glGenBuffers(1, &BoxEBOYellow);

    LoadDataToGraphicsCard();
    LoadYellow();
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
\brief Loads the yellow outline data to the graphics card.

Creates an array of vertex positions and colors from the 4 vertices of the box paired with
all high intensity yellow pixel colors, creates an index array, allocates graphics memory for the arrays and loads the arrays
into graphics memory. Finally, it sets up pointers to the vertex and color data in
the two arrays. This is for the yellow box outline, not the box itself.

*/
void Box::LoadYellow()
{
    vPosition = 0;
    vColor = 1;

    // Vertex and Color data for the box, using two triangles.

    GLfloat vertices[8];

    vertices[0] = centerX - yellowWidth/2;
    vertices[1] = centerY + yellowHeight/2;

    vertices[2] = centerX + yellowWidth/2;
    vertices[3] = centerY + yellowHeight/2;


    vertices[4] = centerX + yellowWidth/2;
    vertices[5] = centerY - yellowHeight/2;

    vertices[6] = centerX - yellowWidth/2;
    vertices[7] = centerY - yellowHeight/2;

    GLuint indices[4] = {0, 1, 2, 3};

    // Bind (turn on) a vertex array.
    glBindVertexArray(YellowVAO);

    // Load the indexing array on the graphics card.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoxEBOYellow);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Bind (turn on) the vertex buffer (storage location).
    glBindBuffer(GL_ARRAY_BUFFER, ArrayBufferYellow);

    // Allocate space for the vertex and color data. Do not load data at this point.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colorsYellow), NULL, GL_DYNAMIC_DRAW);

    // Load the vertex data.
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // Load the color data.
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colorsYellow), colorsYellow);

    // Setup vertex data position information. Note that the 5th parameter is 0, indicating tightly packed.
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Setup color data position information. Note that the 5th parameter is 0, indicating tightly packed.
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

    // Set position indexes for shader streams.
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);//vposition and vcolor??? what are they again??

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
\brief Resets the width of the yellow outline.

\param w --- The width of the yellow outline.

Resets the width of the yellow outline and then calls the yellow outline loading method.

*/
void Box::setYellowWidth(GLfloat w)
{
    yellowWidth = w;
    LoadYellow();
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
\brief Resets the height of the yellow outline.

\param h --- The height of the yellow outline.

Resets the height of the yellow outline and then calls the data loading method for the yellow outline.

*/
void Box::setYellowHeight(GLfloat h)
{
    yellowHeight = h;
    LoadYellow();
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
\brief Resets the center of the yellow outline.

\param cx --- X position of the center.

\param cx --- Y position of the center.

Resets the center of the outline and then calls the data loading method.

*/

void Box::setYellowCenter(GLfloat cx, GLfloat cy)
{
    centerX = cx;
    centerY = cy;
    LoadYellow();
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
\brief Returns the width of the yellow outline.

*/
GLfloat Box::getYellowWidth()
{
    return yellowWidth;
}

/**
\brief Returns the height of the rectangle.

*/

GLfloat Box::getHeight()
{
    return height;
}
/**
\brief Returns the height of the yellow outline.

*/
GLfloat Box::getYellowHeight()
{
    return yellowHeight;
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
\brief Populates the array c with the (x, y) coordinates of the centey (yellow outline)

The array c must have length at least two.

*/
void Box::getYellowCenter(GLfloat* c)
{
    c[0] = YellowCenterX;
    c[1] = YellowCenterY;
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
/**
\brief Draws the yellow outline to the OpenGL window. Similar to the regular draw()

*/
 void Box::drawYellow()
 {
    glBindVertexArray(YellowVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoxEBOYellow);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, NULL);
 }

 /**
\brief Returns true if the passed mouse coordinates are over the box. Uses geometry and arithmetic.

\param x --- Should be a screen position converted mouse position, the x value of that.
\param y --- Should be a screen position converted mouse position, the y value of that.

Returns false if the passed mouse position coordinates are not over the box.

*/
 bool Box::compareToBox(GLfloat x, GLfloat y)
 {



        float xy[2];
        getCenter(xy);

        if ((x >= xy[0] && y >= xy[1]) && (x <= centerX+width/2 && y <= centerY+height/2))//q1 good
        {
        return true;
        }
        if ((x >= xy[0] && y <= xy[1]) && (x <= centerX+width/2 && y >= centerY+(-1)*height/2))//q4 good
        {
        return true;
        }
        if ((x <= xy[0] && y >= xy[1]) && (x >= centerX+(-1)*width/2 && y <= centerY+height/2))//q2 good
        {
        return true;
        }
        if ((x <= xy[0] && y <= xy[1]) && (x >= centerX+(-1)*width/2 && y >= centerY+(-1)*height/2))//q3... good
        {
        return true;
        }

return false;
 }
 /**
\brief Returns true if the passed mouse coordinates are over the box. Uses geometry and arithmetic.

\param revert --- A boolean representing whether or not you want to revert the colors of the box to red. If revert is false,
original box colors are set.

Used in conjunction with the mouse over to change colors. If the box was changed to green, blue, etc before the hover, it will go
back to that color that it was changed to. Not to the original color at the start of the program, as was asked in class.

*/
 void Box::changeToRed(bool revert)
{
        if (revert)
        {
            setVertexColor(0, 1, 0, 0);//top left vertex();
            setVertexColor(1, 1, 0, 0);
            setVertexColor(2, 1, 0, 0);
            setVertexColor(3, 1, 0, 0);

        }
        else
        {
            if (regular)
            {
            setVertexColor(0, 1, 0, 0);//top left vertex
            setVertexColor(1, 0, 1, 0);
            setVertexColor(2, 0, 0, 1);
            setVertexColor(3, 1, 1, 1);
            }
            else if (blue)
            {
            setVertexColor(0, 0, 0, 1);//vertex 1 (top left)
            setVertexColor(1, 0, 0, 1);//vertex 2
            setVertexColor(2, 0, 0, 1);//vertex 3
            setVertexColor(3, 0, 0, 1);
            }
             else if (white)
            {
            setVertexColor(0, 1, 1, 1);//vertex 1 (top left)
            setVertexColor(1, 1, 1, 1);//vertex 2
            setVertexColor(2, 1, 1, 1);//vertex 3
            setVertexColor(3, 1, 1, 1);
            }
             else if (green)
            {
            setVertexColor(0, 0, 1, 0);//vertex 1 (top left)
            setVertexColor(1, 0, 1, 0);//vertex 2
            setVertexColor(2, 0, 1, 0);//vertex 3
            setVertexColor(3, 0, 1, 0);
            }
            else
            {
            setVertexColor(0, r, g, b);//top left vertex
            setVertexColor(1, r, g, b);
            setVertexColor(2, r, g, b);
            setVertexColor(3, r, g, b);
            }
        }
//LoadDataToGraphicsCard();
}
 /**
 \brief Purpose is to select the given box. Sets the box's boolean member attribute "selected" to true. Also initialized yellow colors to be loaded,
to be honest I may not need to do that part here (done in constructor) but I do not have enough time to delete it and fully test it again to make sure
nothing crazy happens. Better safe than sorry. It sets the boolean to true.
*/
void Box::select()
{
            selected = true;
            colorsYellow[0] = 1;
            colorsYellow[1] = 1;
            colorsYellow[2] = 0;
            colorsYellow[3] = 1;
            colorsYellow[4] = 1;
            colorsYellow[5] = 0;
            colorsYellow[6] = 1;
            colorsYellow[7] = 1;
            colorsYellow[8] = 0;
            colorsYellow[9] = 1;
            colorsYellow[10] = 1;
            colorsYellow[11] = 0;

            LoadYellow();

}
/**
\brief This function very simply sets the boolean member attribute of this box to false. This would stop a yellow outline
from being drawn in other parts of this project.
*/
void Box::deselect()
{
selected = false;
}
/**
 \brief This function will drag a selected box (we know it is selected because of the function that calls this one). It performs the
arithmetic necessary to drag box and yellow outline, adds the difference between the previous center location and new
mouse position to the center values of both.

\param differencex --- Was calculated in function that calls this one, difference in x values that needs to be added to the center to drag it.
\param differencey --- Same as the above, just for the y value.

*/
void Box::dragSelectedBox(float differencex, float differencey)
{
centerX = centerX + differencex;
centerY = centerY + differencey;
YellowCenterX = YellowCenterX + differencex;
YellowCenterY = YellowCenterY + differencey;
LoadDataToGraphicsCard();
LoadYellow();
}
