#version 460 core

#define SIZE 256

layout (std430, binding = 2) buffer outputSSBO {
    vec2 outputData[];
};

out vec3 currentPosition;
out vec3 normals;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    int offset = 3 * int(pow(SIZE, 2));

    vec3 aPos = vec3(0.0f, 0.0f, 0.0f);
    vec3 aNormals = vec3(0.0f, 0.0f, 0.0f);
    
    int x = 3 * gl_VertexID;
    int y = 3 * gl_VertexID + 1;
    int z = 3 * gl_VertexID + 2;
    aPos.x = sqrt(pow(outputData[x].x, 2.0f) + pow(outputData[x].y, 2.0f));
    aPos.y = sqrt(pow(outputData[y].x, 2.0f) + pow(outputData[y].y, 2.0f));
    aPos.z = sqrt(pow(outputData[z].x, 2.0f) + pow(outputData[z].y, 2.0f));

    int xoff = offset + x;
    int yoff = offset + y;
    int zoff = offset + z;
    aNormals.x = sqrt(pow(outputData[xoff].x, 2.0f) + pow(outputData[xoff].y, 2.0f));
    aNormals.y = sqrt(pow(outputData[yoff].x, 2.0f) + pow(outputData[yoff].y, 2.0f));
    aNormals.z = sqrt(pow(outputData[zoff].x, 2.0f) + pow(outputData[zoff].y, 2.0f));

    currentPosition = vec3(model * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(currentPosition, 1.0f);

    normals = aNormals;
}