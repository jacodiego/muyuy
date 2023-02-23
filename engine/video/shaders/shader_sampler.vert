#version 460

layout(binding = 0) uniform UniformBufferObject {
    float alpha;
    float scale;
    float multiplyColor;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out float fragAlpha;
layout(location = 3) out float fragMultiplyColor;

void main() {
    gl_Position = vec4((inPosition - 0.5)*ubo.scale + (0.5*ubo.scale), 0.0, 1.0);
    fragAlpha = ubo.alpha;
    fragMultiplyColor = ubo.multiplyColor;
    fragTexCoord = inTexCoord;
    fragColor = inColor;
}