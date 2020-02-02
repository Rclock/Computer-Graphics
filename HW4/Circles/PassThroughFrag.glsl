#version 330 core

/**
\file PassThroughFrag.glsl

\brief Simple pass through fragment shader.

This is a simple GLSL pass through fragment shader.

\param [in] color --- vec4 color from vertex shader.

\param [out] fColor --- vec4 output color to the frame buffer.

\param uniform vec4 colors --- Sent in from the graphics engine to overlay the current data on graphics card.
*/

in  vec4 color;
out vec4 fColor;

uniform vec4 colors;

void main()
{

    fColor = colors;

}
