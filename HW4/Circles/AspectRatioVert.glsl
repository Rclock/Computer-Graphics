#version 330 core

/**
\file AspectRatioVert.glsl

\brief Simple pass through vertex shader.

This is a simple GLSL pass through vertex shader.

\param [in] icolor --- vec4 color from vertex array.

\param [in] position --- vec4 position from vertex array.

\param [out] color --- vec4 output color to the fragment shader.

\param uniform mat4 projectionMatrix, ModelMatrix --- Used to do aspect ratio and linear transformation of object.

\param uniform vec4 colors --- Color to recolor the c

*/

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 icolor;

out vec4 color;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;




void main()
{
    color = icolor;

    gl_Position = ProjectionMatrix*ModelMatrix*position;
}
