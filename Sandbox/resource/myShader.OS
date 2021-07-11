@vertex
#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCoord;

out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 compositeMatrix;
uniform mat4 modelMatrix;

void main() {

    gl_Position = compositeMatrix * vec4(pos, 1.0);
    FragPos = (modelMatrix * vec4(pos, 1.0)).xyz;
    TexCoord = texCoord;

}

@fragment
#version 330 core

in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 color;
uniform sampler2D texture1;

out vec4 fragColor;

void main() {

    vec3 fPos = normalize(FragPos);
    fragColor = vec4(texture(texture1, TexCoord).xyz, 1);

}

@END