#version 460 core

#define SIZE 256

layout (std430, binding = 2) buffer vertex {
    vec2 data[];
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
    aPos.x = sqrt(pow(data[x].x, 2.0f) + pow(data[x].y, 2.0f));
    aPos.y = sqrt(pow(data[y].x, 2.0f) + pow(data[y].y, 2.0f));
    aPos.z = sqrt(pow(data[z].x, 2.0f) + pow(data[z].y, 2.0f));

    int xoff = offset + x;
    int yoff = offset + y;
    int zoff = offset + z;
    aNormals.x = sqrt(pow(data[xoff].x, 2.0f) + pow(data[xoff].y, 2.0f));
    aNormals.y = sqrt(pow(data[yoff].x, 2.0f) + pow(data[yoff].y, 2.0f));
    aNormals.z = sqrt(pow(data[zoff].x, 2.0f) + pow(data[zoff].y, 2.0f));

    currentPosition = vec3(model * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(currentPosition, 1.0f);

    normals = aNormals;
}