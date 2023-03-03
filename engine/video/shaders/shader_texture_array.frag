#version 460

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in flat int fragIndex;

layout(set = 0, binding = 0) uniform sampler texSampler;
layout(set = 0, binding = 1) uniform texture2D textures[2];

layout(location = 0) out vec4 outColor;

void main() {
	outColor = texture(sampler2D(textures[fragIndex], texSampler), fragTexCoord);
}