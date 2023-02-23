#version 460

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in float fragAlpha;
layout(location = 3) in float fragMultiplyColor;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
    if(fragMultiplyColor > 0) 
        outColor = vec4(fragColor * texture(texSampler, fragTexCoord).rgb, texture(texSampler, fragTexCoord).a * fragAlpha);
    else
        outColor = vec4(texture(texSampler, fragTexCoord) * fragAlpha);
}