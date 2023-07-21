#version 330 core
out vec4 FragColor;

uniform vec4 lightColor;
in vec4 color;

void main()
{
   FragColor =  color * lightColor;
   //vec4(0.005960569716989994f, 0.0f, 0.8000000715255737f, 1.0f)
}