#version 400

layout (location = 0) in vec3 position;

out vec3 texCoords;

//uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
    texCoords = position;
    mat4 view = mat4(mat3(viewMatrix)); //mat4(mat3(viewMatrix)); // removing the translation component out of it
    // jinak by to byla krychle na jednom miste, tkato je na pozici kamery
    gl_Position = projectionMatrix * view * vec4(position, 1.0);
}