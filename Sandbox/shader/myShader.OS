@vertex
#version 330 core

layout(location=0) in vec2 pos;

void main() {

    gl_Position = pos; 

}

@fragment
#version 330 core

uniform vec3 color;

out vec4 fragColor;

void main() {

    fragColor = vec4(color, 1);

}

@END