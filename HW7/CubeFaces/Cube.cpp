#include "Cube.h"

/**
\file Cube.cpp

\brief Implementation file for the Cube class.

\author    Ryan Clocker
\version   1.0
\date      4/26/2019


*/

/**
\brief Constructor

Simply stores the pointer of the GraphicsEngine.

*/

Cube::Cube()
{
    glGenVertexArrays(1, &vboptr1);
    glGenBuffers(1, &eboptr1);
    glGenBuffers(1, &bufptr1);

      glGenVertexArrays(1, &vboptr2);
    glGenBuffers(1, &eboptr2);
    glGenBuffers(1, &bufptr2);

      glGenVertexArrays(1, &vboptr3);
    glGenBuffers(1, &eboptr3);
    glGenBuffers(1, &bufptr3);

    glGenVertexArrays(1, &vboptr4);
    glGenBuffers(1, &eboptr4);
    glGenBuffers(1, &bufptr4);


    glGenVertexArrays(1, &vboptr5);
    glGenBuffers(1, &eboptr5);
    glGenBuffers(1, &bufptr5);

    glGenVertexArrays(1, &vboptr6);
    glGenBuffers(1, &eboptr6);
    glGenBuffers(1, &bufptr6);



    LoadDataToGraphicsCard();
}

/**
\brief Destructor

Removes allocated data from the graphics card.

*/

Cube::~Cube()
{
    glBindVertexArray(vboptr1);
    glDeleteBuffers(1, &bufptr1);
    glDeleteBuffers(1, &eboptr1);

    glBindVertexArray(vboptr2);
    glDeleteBuffers(1, &bufptr2);
    glDeleteBuffers(1, &eboptr2);

    glBindVertexArray(vboptr3);
    glDeleteBuffers(1, &bufptr3);
    glDeleteBuffers(1, &eboptr3);

    glBindVertexArray(vboptr4);
    glDeleteBuffers(1, &bufptr4);
    glDeleteBuffers(1, &eboptr4);

    glBindVertexArray(vboptr5);
    glDeleteBuffers(1, &bufptr5);
    glDeleteBuffers(1, &eboptr5);

    glBindVertexArray(vboptr6);
    glDeleteBuffers(1, &bufptr6);
    glDeleteBuffers(1, &eboptr6);


}

/**
\brief Loads the vertex and color data to the graphics card. Splits it up between the 6 VBO's.

*/

void Cube::LoadDataToGraphicsCard()
{
    GLuint vPosition = 0;
    GLuint vColor = 1;
    GLuint vNormal = 2;
    GLuint vTex = 3;



    GLushort indices[] = {0, 1, 2,
                          2, 3, 0};


    GLfloat points[] = {-0.5, 0.5, 0.5, 1,
                        -0.5, -0.5, 0.5, 1,
                        0.5, -0.5, 0.5, 1,
                        0.5, 0.5, 0.5, 1};



    GLfloat normalvectors[] = {0, 0, 1,
                         0, 0, 1,
                         0, 0, 1,
                         0, 0, 1};



    GLfloat colors[] = {1, 0, 0,
                        1, 0, 0,
                        1, 0, 0,
                        1, 0, 0 };


    GLfloat tex[8];
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




    glGenVertexArrays(1, &vboptr1);
    glBindVertexArray(vboptr1);

    glGenBuffers(1, &eboptr1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &bufptr1);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr1);
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


    GLushort indices2[] = {0, 1, 2,
                          2, 3, 0};//wouldn't these stay the same each time since it's broken up

    GLfloat points2[] = {-0.5, 0.5, -0.5, 1,
                -0.5, -0.5, -0.5, 1,
                0.5, -0.5, -0.5, 1,
                0.5, 0.5, -0.5, 1 };


    GLfloat normalvectors2[] = {  0, 0, -1,
                         0, 0, -1,
                         0, 0, -1,
                         0, 0, -1};

    GLfloat colors2[] = {0, 1, 1,
                0, 1, 1,
                0, 1, 1,
                0, 1, 1};

    GLfloat tex2[8];
     //  Lower Left  (-1, -1)
    tex2[0] = 0;
    tex2[1] = 1;

    //  Lower Right  (1, -1)
    tex2[2] = 1;
    tex2[3] = 1;

    //  Upper Right  (1, 1)
    tex2[4] = 1;
    tex2[5] = 0;

    //  Upper Left  (-1, 1)
    tex2[6] = 0;
    tex2[7] = 0;





    glGenVertexArrays(1, &vboptr2);
    glBindVertexArray(vboptr2);

    glGenBuffers(1, &eboptr2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &bufptr2);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points2) + sizeof(colors2) + sizeof(normalvectors2) + sizeof(tex2), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points2), points2);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2), sizeof(colors2), colors2);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2) + sizeof(colors2), sizeof(normalvectors2), normalvectors2);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2) + sizeof(colors2) + sizeof(normalvectors2), sizeof(tex2), tex2);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(normalvectors2) + sizeof(colors2) + sizeof(points2)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(colors2) + sizeof(points2)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points2)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);



    GLushort indices3[] = {0, 1, 2,
                          2, 3, 0};//wouldn't these stay the same each time since it's broken up

    GLfloat points3[] = {-0.5, 0.5, 0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1};


    GLfloat normalvectors3[] = { 0, 1, 0,
                         0, 1, 0,
                         0, 1, 0,
                         0, 1, 0};

    GLfloat colors3[] = { 1, 1, 0,
                        0, 0, 1,
                        0, 0, 1,
                        0, 0, 1};

    GLfloat tex3[8];
     //  Lower Left  (-1, -1)
    tex3[0] = 0;
    tex3[1] = 1;

    //  Lower Right  (1, -1)
    tex3[2] = 1;
    tex3[3] = 1;

    //  Upper Right  (1, 1)
    tex3[4] = 1;
    tex3[5] = 0;

    //  Upper Left  (-1, 1)
    tex3[6] = 0;
    tex3[7] = 0;


    glGenVertexArrays(1, &vboptr3);
    glBindVertexArray(vboptr3);

    glGenBuffers(1, &eboptr3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &bufptr3);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points3) + sizeof(colors3) + sizeof(normalvectors3) + sizeof(tex3), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points3), points3);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points3), sizeof(colors3), colors3);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points3) + sizeof(colors3), sizeof(normalvectors3), normalvectors3);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points3) + sizeof(colors3) + sizeof(normalvectors3), sizeof(tex3), tex3);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(normalvectors3) + sizeof(colors3) + sizeof(points3)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(colors3) + sizeof(points3)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points3)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);


    GLushort indices4[] = {0, 1, 2,
                          2, 3, 0};//wouldn't these stay the same each time since it's broken up

    GLfloat points4[] = {-0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, -0.5, 0.5, 1};


    GLfloat normalvectors4[] = {0, -1, 0,
                                0, -1, 0,
                                0, -1, 0,
                                0, -1, 0};

    GLfloat colors4[] = {1, 1, 0,
                        1, 1, 0,
                        1, 1, 0,
                        1, 1, 0};

    GLfloat tex4[8];
     //  Lower Left  (-1, -1)
    tex4[0] = 0;
    tex4[1] = 1;

    //  Lower Right  (1, -1)
    tex4[2] = 1;
    tex4[3] = 1;

    //  Upper Right  (1, 1)
    tex4[4] = 1;
    tex4[5] = 0;

    //  Upper Left  (-1, 1)
    tex4[6] = 0;
    tex4[7] = 0;


    glGenVertexArrays(1, &vboptr4);
    glBindVertexArray(vboptr4);

    glGenBuffers(1, &eboptr4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr4);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices4), indices4, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &bufptr4);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points4) + sizeof(colors4) + sizeof(normalvectors4) + sizeof(tex4), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points4), points4);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points4), sizeof(colors4), colors4);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points4) + sizeof(colors4), sizeof(normalvectors4), normalvectors4);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points4) + sizeof(colors4) + sizeof(normalvectors4), sizeof(tex4), tex4);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(normalvectors4) + sizeof(colors4) + sizeof(points4)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(colors4) + sizeof(points4)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points4)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);


     GLushort indices5[] = {0, 1, 2,
                          2, 3, 0};//wouldn't these stay the same each time since it's broken up

    GLfloat points5[] = { 0.5, -0.5, 0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1};


    GLfloat normalvectors5[] = {1, 0, 0,
                         1, 0, 0,
                         1, 0, 0,
                         1, 0, 0};

    GLfloat colors5[] = {0, 1, 1,
                        0, 1, 1,
                        0, 1, 1,
                        0, 1, 1};

    GLfloat tex5[8];
     //  Lower Left  (-1, -1)
    tex5[0] = 0;
    tex5[1] = 1;

    //  Lower Right  (1, -1)
    tex5[2] = 1;
    tex5[3] = 1;

    //  Upper Right  (1, 1)
    tex5[4] = 1;
    tex5[5] = 0;

    //  Upper Left  (-1, 1)
    tex5[6] = 0;
    tex5[7] = 0;


    glGenVertexArrays(1, &vboptr5);
    glBindVertexArray(vboptr5);

    glGenBuffers(1, &eboptr5);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr5);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices5), indices5, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &bufptr5);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points5) + sizeof(colors5) + sizeof(normalvectors5) + sizeof(tex5), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points5), points5);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points5), sizeof(colors5), colors5);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points5) + sizeof(colors5), sizeof(normalvectors5), normalvectors5);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points5) + sizeof(colors5) + sizeof(normalvectors5), sizeof(tex5), tex5);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(normalvectors5) + sizeof(colors5) + sizeof(points5)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(colors5) + sizeof(points5)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points5)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);



     GLushort indices6[] = {0, 1, 2,
                          2, 3, 0};//wouldn't these stay the same each time since it's broken up

    GLfloat points6[] = {-0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        -0.5, 0.5, 0.5, 1};


    GLfloat normalvectors6[] = {-1, 0, 0,
                         -1, 0, 0,
                         -1, 0, 0,
                         -1, 0, 0};

    GLfloat colors6[] = { 1, 0, 1,
                        1, 0, 1,
                        1, 0, 1,
                        1, 0, 1};

    GLfloat tex6[8];
     //  Lower Left  (-1, -1)
    tex6[0] = 0;
    tex6[1] = 1;

    //  Lower Right  (1, -1)
    tex6[2] = 1;
    tex6[3] = 1;

    //  Upper Right  (1, 1)
    tex6[4] = 1;
    tex6[5] = 0;

    //  Upper Left  (-1, 1)
    tex6[6] = 0;
    tex6[7] = 0;


    glGenVertexArrays(1, &vboptr6);
    glBindVertexArray(vboptr6);

    glGenBuffers(1, &eboptr6);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr6);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices6), indices6, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &bufptr6);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr6);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points6) + sizeof(colors6) + sizeof(normalvectors6) + sizeof(tex6), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points6), points6);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points6), sizeof(colors6), colors6);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points6) + sizeof(colors6), sizeof(normalvectors6), normalvectors6);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points6) + sizeof(colors6) + sizeof(normalvectors6), sizeof(tex6), tex6);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(normalvectors6) + sizeof(colors6) + sizeof(points6)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(colors6) + sizeof(points6)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points6)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);

}

/**
\brief Draws the box to the screen.

*/

void Cube::draw(GLint whichface)
{
    if (whichface == 1)
    {
    glBindVertexArray(vboptr1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }
    //send integer to shader to specify the texture to use here
    if (whichface == 2)
    {
    glBindVertexArray(vboptr2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }

    if (whichface == 3)
    {
    glBindVertexArray(vboptr3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }
    if (whichface == 4)
    {
    glBindVertexArray(vboptr4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr4);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }
    if (whichface == 5)
    {
    glBindVertexArray(vboptr5);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr5);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }

    if (whichface ==6)
    {
    glBindVertexArray(vboptr6);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }
}
