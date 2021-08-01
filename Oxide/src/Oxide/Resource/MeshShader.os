
@vertex
#version 450 core

layout(location=0) in vec3 in_Pos;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 uAssembledMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uInverseModel;

void main() {

    vec4 calcedPos = uAssembledMatrix * vec4(in_Pos, 1.0);
    FragPos = (uModelMatrix * vec4(in_Pos, 1)).xyz;
    gl_Position = calcedPos;
    TexCoord = in_TexCoord;
    Normal = mat3(transpose(uInverseModel)) * in_Normal;
    

}

@fragment
#version 450 core

const uint USING_AMBIENT_TEXTURE = 1 << 0;
const uint USING_DIFFUSE_TEXTURE = 1 << 1;
const uint USING_SPECULAR_TEXTURE = 1 << 2;
const uint USING_SHININESS_TEXTURE = 1 << 3;
const uint USING_DISPLACEMENT_TEXTURE = 1 << 4;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;

uniform uint uMatProps;

uniform sampler2D uAmbientTexture;
uniform sampler2D uDiffuseTexture;
uniform sampler2D uSpecularTexture;
uniform sampler2D uShininessTexture;
uniform sampler2D uDisplacementTexture;

uniform float uShininess;
uniform vec3 uAmbientColor;
uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;

void main() {
    vec3 color;
    if ((USING_DIFFUSE_TEXTURE & uMatProps) > 0) {
        color = texture(uDiffuseTexture, TexCoord).xyz;
    } else {
        color = uDiffuseColor;
    }
    FragColor = vec4(color, 1.0);
}

@END