#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTex;

out vec4 color;

uniform mat4 camMatrix;
uniform vec4 matColour;

void main()
{
   gl_Position = camMatrix * vec4(aPos, 1.0f);
   // Use the normal direction for shading calculations
   vec3 normal = normalize(aNormals);
   // Perform shading calculations using the normal direction
   color = matColour; // Example shading calculation
}