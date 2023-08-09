#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTex;

layout (std430, binding = 0) buffer MySSBO {
    vec3 data[];
} ssbo;

out vec3 currentPosition;
out vec3 normals;

uniform mat4 camMatrix;
uniform mat4 model;


void main()
{
   currentPosition = vec3(model * vec4(aPos, 1.0f));
   gl_Position = camMatrix * vec4(currentPosition, 1.0f);

   normals = aNormals;
}