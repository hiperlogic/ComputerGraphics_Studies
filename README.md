# ComputerGraphics_Studies
- Systems Covered (So Far): OpenGL, ~~Vulkan~~
________________________________________________________________________________
## [00 - Initial Setup](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/master/README.md)

________________________________________________________________________________
## [00 - A - Initial Configuration: Application Framework and CMake Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_CMake_Project_Create/README.md)

________________________________________________________________________________
## [00 - B - CMake Specific Configuration, Project Folder Structure and Code](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_a_Project_Structure_And_Setup/README.md)

________________________________________________________________________________
## [00 - C - Finishing the Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_b_app_framework/README.md)

________________________________________________________________________________
## [00 - D - App Framework Extras](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_c_app_framework_extras/README.md)

## [01 - Creating the First Graphics and A New Project](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/01_OpenGL_The_First_Graphics/README.md)

## [01 - A - Sending Data to OpenGL](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/01_a_plain_triangle/README.md)

## [01 - B - Sending Data to OpenGL - Programmable Pipeline](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/01_b_plain_triangle_programmable_pipeline/README.md)

## 01 - C - Answers about configuring the Vertex Attrib Array

Before we go to proceed to the next project, let's answer those some questions for you to experiment:

1. If the data we are setting is a static and to draw, do I really need to configure the vertex attrib array and set the vertex attrib pointer at each loop pass?

    The data is static, meaning it does not change, the vertex attrib array is the same every loop, and, it is the only one. In this example it can be configured just once, outside the loop.

2. If such configuration (questioned in 1) can be done outside the loop, how to proceed with disabling the vertex attrib array? Does it need to be within the loop? Or outside? If outside, where?
    The vertex attrib array can be configured outside the loop, previous to it, but it must not be disabled within it, or prior to its start. It must be disabled after the loop ends or the triangle will be drawn only on the first frame and then never more. Since the screen is cleared each frame, it will appear that the triangle is never drawn.

The code implements the discussed issue.

## Vulkan Graphics Pipeline

The initial OpenGL sections of the last 2 branches discussed a little about the graphics pipeline, the fixed function pipeline, implemented since the first OpenGL version and the current programmable pipeline, with its shaders computational stages, at least vertex and fragment shading, providing more flexibility and freedom to the graphics developers (at the small cost of some complexity).
Vulkan was designed with a specific pipeline that extends the one discussed in the previous section:

API->Primitive Processing->**_Vertex Shader_**->**_Tessellation_**->**_Geometry Shader_**->Rasterizer->**_Fragment Shader_**->Depth and Stencil->Color Buffer Blend->Frame Buffer.

The stages in **_bold+italic_** are the programmable stages, while the ones with normal face are fixed pipeline stages.
Fixed pipeline stages are those that can be tweaked by setting their stages prior to the processing, and the result is alwas expected. The programmable stages, on the other hand, can be configured as the developer thinks best.
If you are only reading the Vulkan sections, which I advice not to do, read the OpenGL as well, it is good to have a point of reference for comparison, then let's discuss each stage:

* Primitive Processing, sometimes know as Input Assembler, collects the raw data from the buffers specified. These buffers can provide each data or an index to the proper data to avoid repetitions or duplicates, like duplicated vertexes or color indices. The buffers can provide vertex data, color data, normal data, texture coordinates data or other information. These data are then forwarded to the next stage to be processed;
* **_Vertex Shader_** processes each vertex data, applying transformations to turn the vertex position from the model space to the view and then screen space. It receives from the application the base graphics type to know how to group and process the vertices, providing the computed data to the next stage in the pipeline as well as forwarding any other data, either in a bypass or computed;
* **_Tessellation_** is the process that given a geometric type and the vertices used to create it, subdivides the result producing more vertices and adding more details to produce the graphics. This is an optional stage;
* **_Geometry Shader_** retrieves the vertices and along with some procedural or functional rules, create new vertices and shapes to compose the end result;
* Rasterization is the process of retrieving the data processed by the previous stages, using the vertices and other attributes such as color and normal, to produce a blueprint of how the data should be organized to be shown on the screen. At this point there is a mapping from a fragment to a screen position, but a screen position can have several fragments;
* **_Fragment Shader_** receives the fragments from the rasterization process and do the processing it was programmed to do with them. It still produces fragments, they still cannot be considered pixels, although several visual attributes, like colors or texture fragment, are already assigned to them;
* Depth and Stencil is the process that will determine if the fragment will go to the next stage. They rely on some configured rules, like, for instance, the Depth Buffer decides which fragment will be present based on the incomming fragment depth value and the depth value of the fragment already stored in the buffer. The same happens with Stencil, but not for depth, for any other attribute configured, like a color fragment, for instance;
* Color Buffer Blend processes the fragments and if the blending equation is configured, blends the colors using it, producing, then, the final results to send to the framebuffer.

The graphics pipeline in Vulkan is not as fixed as in OpenGL and DirextX, so it must be recreated from scratch if there is the intention of changing shaders, bind different framebuffers or change the blend function, with some stages being optional.
This also increases the complexity to the graphics developers, but provides possibilities to improve the performance of the desired functions, or to produce the desired results.
For instance, when computing shadow maps to render shadows, only the depth buffer is needed, the fragment shader can be disabled.

For now, we want to create a triangle, so, let's proceed to that.

### Creating the Graphics Pipeline

Unlike OpenGL (discussed in the next section) or DirectX, shaders in Vulkan are not text files, but binary bytecodes. This was defined to provide more uniformity in the shader processing hardware specifications and enable hardware optimizations.
Vulkan provides a shader compiler, the glslc, so that the shader used in OpenGL can be transformed to SPIR-V, the bytecode shader specification for Vulkan. This compiler has the same standard parameters as GCC.

To produce the triangle in the screen the application needs a vertex shader and a fragment shader.

#### Vertex Shader

The vertex shader processes each incomming vertex considering a graphics primitive. It takes the vertex atributes, like world position, color, normal and texture coordinates as input. Only position is mandatory. The output is a fragment positioned in clip coordinates and the attributes that will be passed to the rasterizer and fragment shader, like color and texture coordinates.
A clip coordinate is a four dimensional area, represented by a vector from the vertex shader, that is turned into a normalized device coordinate. The normalized device coordinates are homogeneous coordinates that map the framebuffer to a two dimensional coordinate system reanging from -1 to 1.

For the first triangle, the coordinates will be hardcoded directly in the vertex shader. It should be passed via `primitive processing`, but this would demand the creation of a buffer and a buffer view and a sort of other resources that would delay the display of the triangle, so, by now, the triangle in Vulkan is harcoded, not provided by the application.

The vertex shader used will provide an array of vec2 with 3 values and set it to the implicit variable gl_Position.

```C
#version 450

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
```

`glVertexIndex` indicates which vertex is to be processed in the shader "pass".

#### The fragment shader

The fragment shader for the moment will set every fragment to the color red. You only need to declare the return variable `outColor`:

```c
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}
```

The proper color output will be discussed later.

#### Compiling the shaders

Store the each of the shaders in a separate file. I'll call them `basic_triangle.vert` and `basic_triangle.frag`, in the `Shaders` folder. Shaders don't have an official extension, but those are commonly used to identify them.

Next: Coloring the plain triangle Immediate Mode
