#version 460

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in int inIndex;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out int fragIndex;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragIndex = inIndex;
    fragTexCoord = inTexCoord;
}