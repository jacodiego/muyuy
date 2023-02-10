#version 460

layout(binding = 0) uniform UniformBufferObject {
    float alpha;
    float scale;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out float fragAlpha;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4((inPosition - 0.5)*ubo.scale + (0.5*ubo.scale), 0.0, 1.0);
    fragAlpha = ubo.alpha;
    fragTexCoord = inTexCoord;
}