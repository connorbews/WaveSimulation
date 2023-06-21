#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTex;

out vec3 color;

out vec2 texCoord;

uniform mat4 camMatrix;

void main()
{
   gl_Position = camMatrix * vec4(aPos, 1.0f);
   color = vec3(0.92f, 0.86f, 0.76f);
   texCoord = aTex;
}