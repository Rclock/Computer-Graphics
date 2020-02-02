#version 330 core
//#version 410 core

/**
\file PhongMultipleLightsAndTexture.glsl

\brief Fragment shader that calculates Phong lighting for each fragment,
textual for the same fragment and combines the two. In OpenGL 4.1 you
can use a variable index in a sampler2D array, but in OpenGL 3.3 you
can only use constants as sampler2D array indexes.

\param [in] position --- vec4 vertex position from memory.
\param [in] color --- vec4 vertex color from memory.
\param [in] normal --- vec3 normal vector from memory.
\param [in] tex_coord --- vec2 texture coordinate from memory.
\param [out] fColor --- vec4 output color to the frame buffer.
\param [uniform] Lt --- Light struct containing a single light attribute set.
\param [uniform] Mat --- Material struct containing a single material attribute set.
\param [uniform] eye --- vec3 position of the viewer/camera.
\param [uniform] GlobalAmbient --- vec4 global ambient color vector.
\param [uniform] useTexture --- boolean that determines if the texture is used.
\param [uniform] textrans --- mat4 texture transformation.
\param [uniform] tex1 --- sampler2D, the texture.

*/
float attenuation;

struct Light
{
    bool on;             ///< Light on or off.
    vec4 position;       ///< Position of the light.
    vec3 spotDirection;  ///< Direction of the spot light.
    vec4 ambient;        ///< Ambient color of the light.
    vec4 diffuse;        ///< Diffuse color of the light.
    vec4 specular;       ///< Specular color of the light.
    float spotCutoff;    ///< Spot cutoff angle.
    float spotExponent;  ///< Spot falloff exponent.
    vec3 attenuation;    ///< Attenuation vector, x = constant, y = linear, z = quadratic.
};

struct Material
{
    vec4 ambient;     ///< Ambient color of the material.
    vec4 diffuse;     ///< Diffuse color of the material.
    vec4 specular;    ///< Specular color of the material.
    vec4 emission;    ///< Emission color of the material.
    float shininess;  ///< Shininess exponent of the material.
};

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 tex_coord;

uniform Light Lt[10];
uniform Material Mat;
uniform vec3 eye;
uniform vec4 GlobalAmbient;
uniform int numLights;
uniform bool useTexture[10];
uniform mat4 textrans;

uniform sampler2D tex[10];

out vec4 fColor;

void main()
{
    float deg = 0.017453292519943296;

    vec4 cc = vec4(0.0);
    bool usingLights = false;
    vec4 globalAmbientPortion = GlobalAmbient;




    vec4 texhom = vec4(tex_coord, 0, 1);
    vec4 transtex = textrans * texhom;
    vec2 transtex2 = vec2(transtex);

    int textureCount = 0;
    for (int i = 0; i < 10; i++)
        if (useTexture[i])
            textureCount++;

    vec4 texColor = vec4(0.0);

//  Can use arrays of textures in GLSL 4.1+
/*
    for (int i = 0; i < 10; i++)
        if (useTexture[i])
            texColor += 1.0/textureCount * texture(tex[i], transtex2);
// */


//  Index for texture arrays must be constant GLSL 3.3-
//*
    if (useTexture[0])
        texColor += 1.0/textureCount * texture(tex[0], transtex2);
    if (useTexture[1])
        texColor += 1.0/textureCount * texture(tex[1], transtex2);
    if (useTexture[2])
        texColor += 1.0/textureCount * texture(tex[2], transtex2);
    if (useTexture[3])
        texColor += 1.0/textureCount * texture(tex[3], transtex2);
    if (useTexture[4])
        texColor += 1.0/textureCount * texture(tex[4], transtex2);
    if (useTexture[5])
        texColor += 1.0/textureCount * texture(tex[5], transtex2);
    if (useTexture[6])
        texColor += 1.0/textureCount * texture(tex[6], transtex2);
    if (useTexture[7])
        texColor += 1.0/textureCount * texture(tex[7], transtex2);
    if (useTexture[8])
        texColor += 1.0/textureCount * texture(tex[8], transtex2);
    if (useTexture[9])
        texColor += 1.0/textureCount * texture(tex[9], transtex2);
// */
     float lightDistance =length(vec3(Lt[0].position)-vec3(position));
    float attenuation = 1.0 / (Lt[0].attenuation[0] +
                                       Lt[0].attenuation[1] * lightDistance +
                                       Lt[0].attenuation[2] * lightDistance * lightDistance);

    if (textureCount > 0)
        //fColor = 0.4*fColor + 0.6*texColor;

    fColor = attenuation * texColor;
    fColor = min(fColor, vec4(1.0));
    //fColor = attenuation*texColor;
}
