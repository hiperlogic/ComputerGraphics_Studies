#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inPosition;

layout(binding = 0) uniform Color_UBO {
    vec4 color;
} c_ubo;

layout(location = 0) out vec4 outColor;

void main() {
    gl_Position = inPosition;
    outColor = c_ubo.color;
}
