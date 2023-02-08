#version 460

layout(location = 0) in float fragAlpha;
layout(location = 1) in vec2 fragTexCoord;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
    //outColor = texture(texSampler, fragTexCoord);
    outColor = vec4(fragAlpha * texture(texSampler, fragTexCoord));
    //outColor = vec4(fragColor, 1.0);
}