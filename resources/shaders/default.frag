#version 330 core
in vec4 color;
in vec3 currentPosition;
in vec3 normals;

out vec4 FragColor;

uniform vec4 lightColor;
uniform vec4 matColour;
uniform vec3 lightPosition;

void main()
{
   vec3 lightDirection = normalize(lightPosition - currentPosition);
   vec3 normalDirection = normalize(normals);
   float diffuse = max(dot(normalDirection, lightDirection), 0.0f);
   FragColor =  matColour * lightColor * diffuse;
}