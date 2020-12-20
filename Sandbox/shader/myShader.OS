@vertex
#version 330 core

layout(location=0) in vec3 pos;

out vec3 FragPos;

uniform mat4 compositeMatrix;
uniform mat4 modelMatrix;

void main() {

    gl_Position = compositeMatrix * vec4(pos, 1.0);
    FragPos = (modelMatrix * vec4(pos, 1.0)).xyz;

}

@fragment
#version 330 core

in vec3 FragPos;

uniform vec3 color;

out vec4 fragColor;

void main() {

    vec3 fPos = normalize(FragPos);
    fragColor = vec4(color * fPos, 1);

}

@END