#version 330 core

/**
\file AspectRatioVert.glsl

\brief Simple pass through vertex shader.

This is a simple GLSL pass through vertex shader.

\param [in] icolor --- vec4 color from vertex array.

\param [in] position --- vec4 position from vertex array.

 \param [out] color --- vec4 output color to the fragment shader.

 \param uniform ProjectionMatrix --- Used for aspect ratio, multiplied on to model matrix to transform vertices.

 \param uniform ModelMatrix --- Used to transform the vertices loaded to the graphics card.


*/

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 fColor;
layout(location = 2) in vec2 in_tex_coord;

out vec4 color;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
out vec2 tex_coord;


void main()
{


    color = fColor;
    tex_coord = in_tex_coord;

    gl_Position = ProjectionMatrix*ModelMatrix*position;
   // gl_Position = PVM*position;
}
