#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 1) uniform samplerCube texSampler;
//layout (binding = 1) uniform samplerCube samplerCubeMap;

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, fragColor);
    //outColor = texture(samplerCubeMap, fragColor);
}