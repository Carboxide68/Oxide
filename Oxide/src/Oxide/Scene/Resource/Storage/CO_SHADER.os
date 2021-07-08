
@vertex
#version 430 core

(layout=0) in vec3 position;
(layout=1) in vec3 normal;
(layout=2) in vec2 texCoord;

uniform bool usingNormal;
uniform bool usingTexCoord;

uniform mat4 assembledMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main() {

    gl_Position = assembledMatrix * vec4(position, 1.0);
    FragPos = (modelMatrix * vec4(position)).xyz
    if (usingTexCoord)
        TexCoord = TexCoord;

    if (usingNormal)
        Normal = normalMatrix * normal

}

@fragment

in vec3 Normal;
in vec2 TexCoord;

uniform bool usingNormal;
uniform bool usingTexCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform bool usingDiffuseTexture;
uniform bool usingSpecularTexture;

vec3 diffuseColor;
float specularStrength;

int main() {

    

}