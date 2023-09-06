#version 460 core
in vec4 color;
in vec3 currentPosition;
in vec3 normals;

out vec4 FragColor;

uniform vec4 lightColor;
uniform vec4 matColour;
uniform vec3 lightPosition;

void main()
{
   //vec3 lightDirection = normalize(lightPosition - currentPosition);
   vec4 skyColour = vec4(0.69f, 0.84f, 1.0f, 1.0f);
   vec3 lightDirection = normalize(vec3(0.1f, 0.1f, -1.0f));
   vec3 normalDirection = normalize(normals);

   float nSnell = 1.34f;
   float reflectivity;

   float dotLightNormal = dot(lightDirection, normalDirection);

   float thetai = acos(sqrt(1 - pow((dotLightNormal), 2.0f)));

   float sinthetat = sin(thetai) / nSnell;
   float thetat = asin(sinthetat);

   if(thetai == 0.0)
   {
      reflectivity = (nSnell - 1)/(nSnell + 1);
      reflectivity = reflectivity * reflectivity;
   }
   else
   {
      float fs = sin(thetat - thetai) / sin(thetat + thetai);
      float ts = tan(thetat - thetai) / tan(thetat + thetai);
      reflectivity = 0.5 * ( fs*fs + ts*ts );
   }
   
   float diffuse = max(dot(normalDirection, lightDirection), 0.0f);
   FragColor = (vec4(0.2f, 0.2f, 0.2f, 0.2f) + matColour) * skyColour * reflectivity;
}