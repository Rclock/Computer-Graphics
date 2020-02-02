#version 330 core

/**
\file PassThroughFrag.glsl

\brief Simple pass through fragment shader.

This is a simple GLSL pass through fragment shader.

\param [in] color --- vec4 color from vertex shader.

\param [out] fColor --- vec4 output color to the frame buffer.

\param vec4 colors --- Colors over the vertices sent to the graphics card for current star. Comes from GE. Random colors.
*/

in  vec4 color;
out vec4 fColor;

uniform vec4 colors;

void main()
{

    fColor = color;
    //fColor = colors;

}
