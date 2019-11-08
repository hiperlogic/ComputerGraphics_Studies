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

and execute the following commands:
    On Windows
```
{PATH_TO}\glslc.exe basic_triang.vert -o basic_triang.spv
{PATH_TO}\glslc.exe basic_triang.frag -o frag.spv
```

    On Linux
```
{PATH_TO}/glslc basic_triang.vert -o basic_triang_v.spv
{PATH_TO}/glslc basic_triang.frag -o basic_triang_f.spv
```

Replacing the `PATH_TO` with the actual path where glslc is located.

This example will use the compiled shaders, but Vulkan provides a library to compile shaders in realtime, and let's try to use it in the future examples. This way, the OpenGL examples and the Vulkan examples will follow some homogeneite!

#### Loading a Shader

Once the SPIR-V shader is compiled, they need to be loaded by the application before being applied to the pipeline.
Although in future this series of documents will use the libshaderc to load the glsl shaders, compile them and insert the bytecode in runtime, for this example the compiled shader will be used and it will be loaded into a byte array managed by std::vector.

The file data will be streamed using the std::ifstream process, starting the position at the end of the file in order to retrieve the size to configure the vector, that is, the byte array to be returned. Before the byte array allocation it is important to check if the stream is opened. If not, an error ocurred and must be thrown.
Once the byte array (buffer) is configured (the memory area is allocated), the file pointer is moved to the begining and the data can be read into the buffer data. At the end of the reading the stream must be closed and the buffer returned.
Be sure to include the `fstream` header.

```C++
static std::vector<char> readFile(const std::string& filename) {
    // Declare the file (input) stream loading from filename, seting the pointer to the end position (ate) and consider it binary
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if(!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    // Returns the pointer position relative to the beginning of the stream.
    // Starting at the end this means that the position indicates the size in bytes of the file.
    size_t fileSize = (size_t) file.tellg();
    // Allocate the buffer array managed by vector to the filesize
    std::vector<char> buffer(fileSize);
    // Return the pointer to the beginning
    file.seekg(0);
    // Read the data (filesize bytes) into the buffer
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;

}
```

Use it to read the SPIR-V shader files just created within the `createGraphicsPipeline` private method. Store each one in an `auto` identifier. I advise the names `vertShaderCode` and `fragShaderCode` (for now) to store them. And now we need to create the module that will pass the shaders to the pipeline.

#### Creating Shaders Module

Creating a shader module is a simple process, just point to the buffer and specify its length. And since it is a creation procedure, a filled structure, `VkShaderModuleCreateInfo`, is needed to be provided to the creation instruction.
The catch in this process is that the size of the bytecode is specified in `bytes`, but the bytecode pointer is a `uint32_t` instead of an `char`, so, a casting is needed, and it must be a `reinterpret_cast`, since it is indicating that the pointer type (the data stored in the address) is of a different type, and the module can be created with `vkCreateShaderModule`.

```C++
VkShaderModule createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS){
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}
```

Couldn't the code vector be used?
Vulkan cannot know the type of array data being used. This is why the shader module structure is created, so the API be language independent. If you are writing the code in Assembly, Pascal, Python, C, C++, Lua, Lisp, F# or whatever, this homogeneous structure must be configured.

And since the creation is done explicitly, their destruction must be as well. But this time, at the end of the graphics pipeline. And this is done so because once the pipeline is configured, the code will be linked and the modules will no longer be needed.

So far the `createGraphicsPipeline` private method is as follow:

```C++
void createGraphicsPipeline() {
    auto vertShaderCode = readFile("shaders/basic_triangle_v.spv");
    auto fragShaderCode = readFile("shaders/basic_triangle_f.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    // (more code will come)

    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}
```

#### Shader Creation

Wait! Haven't we created the shaders?
No, we have coded, compiled, loaded and set each of the shaders to the created modules. The modules will be informed to the pipeline configuration procedures in order to set them.
Note that the data for the modules or the loaded shaders have no information if they are vertex or fragment (apart from the identifier names!). The data in the loaded shaders are just bytes and the data in the modules are the bytes, the size and the info that the structure is a shader module.
Now it is time to properly set them to the pipeline creation structure, binding them to the specific graphics pipeline stage. To do that the pipeline must be configured and created, the creation, as usual, uses a structure that must be configured (or filled) to identify what (or how the) object features must be created.

This differs from OpenGL in the sense that in OpenGL you load and create the shaders with their type (vertex/fragment) already specified and just attach them to the program for linking (will be covered in the OpenGL section in the next document).

To create the pipeline shader stage a structure is needed. It must be filled with the kind of stage, an enumerable value that will indicate to what programmable stage the shader belongs, the module and the function to invoke.

The last information provides an interesting possibility.
While in OpenGL the shader starts at the main function, in Vulkan (and SPIR-V), the entry function can be configured in the shader stage. Multiple shader stages can be combined into a single shader module and that module be uses in a number of different shader stage, each calling a specific function.

There is one more option to the shader stage create info structure, and that is the `pSpecializationInfo` that allows to specify values to shader constants so the system can optimize the bytecode. It will be demonstrated in the future (TODO: Explore the usage of this field.)

Once the shader stages (one for vertex, one for fragment) are created, they must be aggregated in order to be properly set in the pipeline.

So, the `createGraphicsPipeline` method, so far, is:

```C++
void createGraphicsPipeline() {
    auto vertShaderCode = readFile("shaders/basic_triangle_v.spv");
    auto fragShaderCode = readFile("shaders/basic_triangle_f.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_SYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_SYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = vertShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // (More code to come!)

    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}
```

Since the Graphics Pipeline is a combination of programmable and fixed functions, and shaders represent the programmable, it is time to look for the fixed functions.

#### Fixed Functions in the Graphics Pipeline

Viewport size, color blending function, depth test logic, all of these were state configurations in older graphics APIs, like OpenGL 1.x or the fixed function pipeline in OpenGL. In Vulkan everythin has to be configured, as you may have noticed. That is why it is taking longer to draw the triangle, while in OpenGL we already have it (a crude, boring colorless triangle, but still... a triangle!).

1. Providing the vertices in OpenGL is a matter of feeding the vertices one by one with glVertex* or via vertex array using a vertex buffer object (VBO);
2. Before providing the vertices one by one or at the moment it tells the system to draw using the VBO, OpenGL needs to know how to assembly the vertices, that is done providing the graphics primitives, or, topologies;
3. To properly draw in the screen, the viewport must be set, so OpenGL could know how to map the application to device to screen coordinates and light the correct pixels;
4. (the Vertex Processing stage is not a fixed function)
5. (the Tesselation Processing stage is not a fixed function.)
6. (the Geometric stage is not a fixed function)
7. That vertex coordinate mapping and the primitive are passed to the rasterizer to generate the raster image (the pixels themselves, but as fragments for now), with configurations for filling (or just contour), depth information and other kinds of information;
8. (The fragment stage is not a fixed function)
9. Fragments are sampled for processes to fix visual glitches, such as antialiasing;
10. The fragments are processed for depth and stencil tests and the ones that passed are sent to the next stage;
11. The color blending is the process of color sampling, fragment testing and applying blending functions (with pixels already on the framebuffer) to modify (or maintain) the fragment color. After the process, it can be properly considered a pixel and sent to the framebuffer.
12. The framebuffer

The fixed functions are configurable, and configurations in Vulkan are set in structures. Some of the states provides more than one structure to be configured, so, let's look at them.
Note, OpenGL have a default configuration for the fixed functions in its pipeline setting, this means that in OpenGL things can be addressed separately, but Vulkan demands all to be set in order to configure its state and start processing.
Some issues here, like viewport, for instance, will only be addressed or mentioned in the OpenGL section in future documents. So, try to understand them here, but if not, they will be revisited for OpenGL.

##### Vertex Input (VkPipelineVertexInputStateCreateInfo)

The structure to configure how the data will be passed to the vertex shader describes the bindings and the attribute descriptions.
`Bindings` specifies some data indicating if the data provided is per-vertex or per-instance, the `attribute descriptions` specifies types of the attributes passed to the vertex shader, which binding to load them from and offset configuration.
Since the vertices are, for the moment, hardcoded in the shader, this structure will be specified as no vertex data for the moment. It will be revisited when discussing vertex buffer.

```C++
VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
vertexInputInfo.vertexBindingDescriptionCount = 0;
vertexInputInfo.pVertexBindingDescriptions = nullptr;
vertexInputInfo.vertexAttributeDescriptionCount = 0;
vertexInputInfo.pVertexAttributeDescriptions = nullptr;
```

The fields `pVertexBindingDescriptions` and `pVertexAttributeDescriptions` point to array of structs that describe the respective contexts for loading the vertex data.
This structure must be added to the createGraphicsPipeline right after the shaderStages array, or code a private function to define and return it.

##### Input Assembly (VkPipelineInputAssemblyStateCreateInfo)
In order to properly process the vertices that are fed to the vertex input, the Vulkan pipeline needs to know how to organize them. This is the responsibility of the `topology` field. It has the same function as the parameter set to glBegin (or the first parameter of glDrawArray/glDrawElements).
The possible (enumerated) topology values are:
* VK_PRIMITIVE_TOPOLOGY_POINT_LIST: Each vertex is a point drawn;
* VK_PRIMITIVE_TOPOLOGY_LINE_LIST: A line is drawn for every 2 vertices. The vertices are discarded after the operation;
* VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: A line is drawn for every 2 vertices. The first vertex is discarded, the last one is used as the first vertex for the next line (next vertex index);
* VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: A triangle is drawn for each 3 vertices. All vertices are discarded after the operation;
* VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: A triangle is drawn for each 3 vertices. The first vertex is discarded. The second and third are used in the next triangle (next vertex index) operation. The next vertex is treated as the new third vertex.

Vulkan provide one more functionality to the vertex processing, to break the line strip or triangle strip processing to start another strip. With this several strips can be set up using just one buffer with minimal primitive overlapping. To do this just set the field `primitiveRestartEnable` to `VK_TRUE` and use the special index `0xFFFF` or `0xFFFFFFFF`.
We are drawing a triangle, so let´s set the topology to a triangle list.

```C++
VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
inputAssembly.primitiveRestartEnable = VK_FALSE;
```

Set it in the createGraphicsPipeline. You can also create a method to return this structure.

##### Viewports and Scissors (VkViewport VkRect2D VkPipelineViewportStateCreateInfo)

Viewport describes the region of the framebuffer used by the renderer. Usually it is set to be (0,0)-(width, height), as well as in this tutorial, but there are other configurations.
Scissor describes the area within a viewport where the pixels will be stored. Every pixel outside that area will be discarded. For now, both will use the whole framebuffer.
Viewport also defines the min and maxDepth values for depth planes. The default is from 0.0 to 1.0 and will remain, but need to be set.

```C++
    VkViewport viewport = {}
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
```

With both structures configured, the pipeline viewport state can be configured. It is possible to use multiple viewports and scissor rectangles on some graphics cards, so those fields references arrays of them. Using multiple viewports requires enabling a GPU feature (See Logical Device Creation).

```C++
VkPipelineViewportStateCreateInfo viewportState = {};
viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
viewportState.viewportCount = 1;
viewportState.pViewports = &viewport;
viewportState.scissorCount = 1;
viewportState.pScissors = &scissor;
```

This configuration is not much different from glViewport (still to be investigated in the OpenGL sections).

##### Rasterizer (VkPipelineRasterizationStateCreateInfo)

The rasterizer takes the geometric information provided by the (processed) vertices and vertices assembly configuration and produces the raster values, or fragments, to be provided to the fragment shader.
The rasterizer is the stage to configure the depth testing and the face culling method, as well as some drawing methods, like the polygons (triangles) will be filled or just outlined (wireframe). These are the fields to be set:
* depthClampEnable (bool) indicates if the fragments that are beyond the near and far planes are clamped to them (true) or discarder (false);
* rasterizerDiscardEnable (bool) indicates if the geometry should pass through the rasterizer (false) or not (true). If true, the output to the framebuffer is disabled;
* polygonMode (enumeration) tells the rasterizer if it whoulg fill the polygons, just draw the outline or just draw the polygon vertices. Respectively VK_POLYGON_MODE_FILL, VK_POLYGON_MODE_LINE, VK_POLYGON_MODE_POINT. Any mode other than `fill` requires enabling a GPU feature;
* lineWidth (float) indicates how thick will be the line. The default is 1.0f;
* cullMode (enumeration) tells if the culling is enabled (discarding some polygons) and how it is done. The possible values are VK_CULL_MODE_BACK_BIT, VK_CULL_MODE_FRONT_BIT, VK_CULL_MODE_FRONT_AND_BACK_BIT, VK_CULL_MODE_NONE. The vertices order to draw the polygon face are responsible to indicate if the face is a front or back face;
* frontFace (enumeration) indicates the method to calculate the frontface of a triangle, if clockwise or counterclockwise. This means that whan the vertices were transformed (via Vertex Shader pipeline) and provided to the raster in a counterclockwise order, if frontFace is set to VK_FRONT_FACE_COUNTER_CLOCKWISE, that face is facing the screen/camera, is is set to CK_FRONT_FACE_CLOCKWISE, it is facing away from the screen/camera (it is a back face), and this influences the culling method;
* depthBiasEnable (bool), depthBiasConstantFactor (float), depthBiasClamp (float), depthBiasSlopeFactor (float) are used to alter depth valuers by adding a constant or biasing them. Several rendering techniques uses depth related information, like shadow mapping, but for the moment set the enable to false. The others, as the depthBias is not enabled, are optional.

```C++
VkPipelineRasterizationStateCreateInfo rasterizer = {};
rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
rasterizer.depthClampEnable = VK_FALSE;
rasterizer.rasterizedDiscardEnable = VK_FALSE;
rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
rasterizer.lineWidth = 1.0f;
rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
rasterizer.depthBiasEnable = VK_FALSE;
rasterizer.depthBiasConstantFactor = 0.0f;
rasterizer.depthBiasClamp = 0.0f;
rasterizer.depthBiasSlopeFactor = 0.0f;
```

So far, none of these configurations were discussed in the OpenGL sections, but they will, so, if you are following both, pay attention to the fields meanings, this may help you in the future.

##### Multisampling (VkPipelineMultisampleStateCreateInfo)

This structure configures how the fragments and pixels (already in the framebuffer) are to be processed to perform antia-liasing. Enabling it will require enabling a GPU feature and this will be revisited. For now just put these values in the code:

```C++
VkPipelineMultisampleStateCreateInfo multisampling = {};
multisampling.sType = VK_STRUCTURE_TYPW_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
multisampling.sampleShadingEnable = VK_FALSE;
multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
multisampling.minSampleShading = 1.0f;
multisampling.pSampleMask = nullptr;
multisampling.alphaToCoverageEnable = VK_FALSE;
multisampling.alphaToOneEnable = VK_FALSE;
```

##### Depth and stencil testing (VkPipelineDepthStencilStateCreateInfo)

For the moment depth and stencil testing are not being used. This info can be passed as a nullptr.

##### Color Blending (VkPipelineColorBlendAttachmentState VkPipelineColorBlendStateCreateInfo)
Right now the input received the vertex, the assembly processed them send them to the vertex shader to be processed and transformed, and the rasterizer produced the result closest to what will be sent to the framebuffer, but those are fragments, not pixels. The fragments goes to the fragment shader and then to the depth and stencil testing, the remainder fragment results are now to be processed to be sent to the framebuffer.
This process, know as color blending, performs one of two operations, mix the value already on the framebuffer with the one incomming from the pipeline to produce a new value or combine the old and new values using a bitwise operation.
For the color blending stage two structures need to be configured. The VkPipelineColorBlendAttachmentState contains the configuration per attached framebuffer and the VkPipelineColorBlendStateCreateInfo contains the global color blend settings. In this example only one framebuffer will be used.

```C++
VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
colorBlendAttachment.blendEnable = VK_FALSE;
colorBlendAttachment.srcColorBLendFactor = VK_BLEND_FACTOR_ONE;
colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

VkPipelineColorBlendStateCreateInfo colorBlending = {};
colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
colorBlending.logicOpEnable = VK_FALSE;
colorBlending.logicOp = VK_LOGIC_OP_COPY;
colorBlending.attachmentCount = 1;
colorBlending.pAttachments = &colorBlendAttachment;
colorBlending.blendConstants[0] = 0.0f;
colorBlending.blendConstants[1] = 0.0f;
colorBlending.blendConstants[2] = 0.0f;
colorBlending.blendConstants[3] = 0.0f;
```

Just set the values for now. Blending will be discussed in the last section: Transparency.

##### Dynamic State (VkPipelineDynamicStateCreateInfo) (optional)

Some states discussed, although a small ammount, can be changed without having to recreate the pipeline. Viewport size, line width and blend constants are some of them, but this will demand the creation of a structure for dynamic states, like:

```C++
VkDynamicState dynamicStates[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_LINE_WIDTH
};

VkPipelineDynamicStateCreateInfo dynamicState = {};
dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
dynamicState.dynamicStateCount = 2;
dynamicState.pDynamicStates = dynamicStates;
```

For the moment this structure will be provided as a nullptr;

#### Pipeline Layout (VkPipelineLayout VkPipelineLayoutCreateInfo)

Some values can be passed or set in shaders via application. They are called `uniform` values and can be used to alter the behavior of shaders. They are regularly used to set transformation matrices or to create texture samplers.
These values need to be specified (declared and configured) during pipeline creation by creating a VkPipelineLayout object. Even though none are being used, they will be in future sections, the pipeline layout need to be specified, so, one will be created with empty layouts.

Since this will be referred within the application, create a private attribute:

```C++
VkPipelineLayout pipelineLayout;
```

In the createGraphicsPipeline continue defining the needed info structures:

```C++
VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
pipelineLayoutInfo.setLayoutCount = 0;
pipelineLayoutInfo.pSetLayouts = nullptr;
pipelineLayoutInfo.pushConstantRangeCount = 0;
pipelineLayoutInfo.pPushConstantRanges = nullptr;

if(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
}
```

And, as usual, explicit object creation demands explicit object destruction. At `cleanup` add the destrucion instruction for pipeline layout:

```C++
vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
```

And the fixed function state is configured for the example. Now almost the full graphics pipeline has been discussed, but there is one more object before drawing and that is the render pass.


#### Setting up Render Passes (VkAttachmentDescription)

Although the graphics pipeline is configured, it needs information about the resources it manipulates. Vertices, normals, colors and texture coordinates are inputs, but those are disposed within those resources, usually color ad depth/stencil buffers. the pipeline must be informed how many color and depth buffers are available, how they will be used or handled throughout rendering operations, how many samples to use for each of them.
Render Pass objects wrap such informations and they need to be fed to the graphics pipeline.
A proper strategy is to create a routine (`createRenderPass`) to instantiate render passes objects and call it before the graphics pipeline is specified.

For this first example only one color buffer attachment will be used, represented by one of the images from the swap chain. It uses the color format specified and stored in the class object.
Each buffer have a set of configurations describing what will be done with some specific data before and after a set of operations.

```C++
void createRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    // Continue
}
```

The main operations, that states the whole rendering process, are the `loadOp` and `storeOp`. The first indicates what will be done with the buffer before the rendering process starts, the last indicates what will be done after it ends, both relates to the color and depth data. Their possible values differ:

    For `loadOp` the possible values are:
*   VK_ATTACHMENT_LOAD_OP_LOAD states that when rendering is about to begin, the existing contend of the buffer is preserved;
*   VK_ATTACHMENT_LOAD_OP_CLEAR states that the buffer is to be cleared before the rendering process begin;
*   VK_ATTACHMENT_LOAD_OP_DONT_CARE just ignores existing contents.

    For `storeOp` the possible values are:
*   VK_ATTACHMENT_STORE_OP_STORE states that rendered contents will be stored in memory and can be read later
*   VK_ATTACHMENT_STORE_OP_DONT_CARE states that contents of the framebuffer will be undefined after the rendering process.

In OpenGL, the presence of the glClear operation in the begining of the rendering loop would represent the VK_ATTACHMENT_LOAD_OP_CLEAR for a render pass. But don't mess, Vulkan process is somewhat different. Render passes can be applied to different buffers, not all of them writing in the framebuffer.

So far, only a triangle will be rendered. So, in the begining clear the buffer, in the end, store it.

```C++
void createRenderPass() {
    
    //...

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    // Continue
}
```

The buffer also provides information for other stages, such as for the stencil operations, using the fields `stencilLoadOp` and `stencilStoreOp`. This example does not uses stencil operations, so it will simply not care.

```C++
void createRenderPass() {

    // ...

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Continue
}
```

Textures and framebuffers only differ for their usage. Both are represented by VkImage objects in Vulkan, both specifies a certain pixel format, but the layout of pixels in memory can change based on what is the purpose of the image. These layouts prior to and after the rendering process also must be configured.
Some of the most common layouts are:
* VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL is used for optimal retrieval of the value;
* VK_IMAGE_LAYOUT_PRESENT_SRC_KHR is used for images to be presented in the swap chain;
* VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL is used for images to receive data from other images via copy operation.

In discussing texturing this topic will be discussed in more depth, but for now the initial layout is undefined and the final is to be presented to the screen.

```C++
void createRenderPass() {

    // ...

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    // Continue
}
```

##### Subpasses (VkAttachmentReference VkSubpassDescription)

OpenGL does not have the concept of render passes, however, it is somehow possible to achieve some similar effects using things like the accumulation buffer or rendering the color buffer to a texture to be used in the next rendering iteration. Those were highly inefective.
Vulkan render passes allows to attach rendering operations that are depended on intermediary framebuffers, prior to drawing on the screen, in order to perform postprocessing or produce intricated visual effects. This means that the render pass have configurable subpasses, and this is a quite versatile tool to produce elaborated graphics.
It was said earlier, Vulkan provides more flexibility for the developers, at the cost of complexity or effort, and this is one of this process. But if these intermediary rendering operations are grouped into one render pass, Vulkan can optimize the process, conserving memory bandwith for better performance. Only one subpass will be used for the triangle.
Every subpass has at least one reference to an attachment. So far only one attachment was configured, and it will be the only one for this example.
Since it is a reference, the object to store it is of type VkAttachmentRererence, and it references the attachment via attachment index in the attachment descriptor. the structure looks like the following:

```C++
VkAttachmentReference colorAttachmentRef = {};
colorAttachmentRef.attachment = 0;
colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
```

In this example, that will be used, the attachment references the index of the attachment descriprion in the attachment description array. Only one description was created, so, its index is 0.
Layout specifies the attachment image layout during the subpass. In the attachment only the image layout previous and after the rendering was configured. Vulkan automatically transition the layouts during the subpasses. Since the attachment configured is a color buffer, and this is its intended use, the image layout must refer to a color attachment, and it is set for performance.

The subpass is described using a `VkSubpassDescription` structure. The structure fields indicates what is the bind point in the pipeline for this subpass, how much of each attachment kind it has and the address of the each attachment reference.

```C++
VkSubpassDescription subpass = {};
subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
subpass.colorAttachmentCount = 1; // only 1 color attachment
subpass.pColorAttachments = &colorAttachmentRef;
```

The index of the attachment in this array (we still have only 1)  is directly referenced from the fragment shader with the `layout(location=0) out vec4 outColor;` directive! (TODO: This considering the fragment shader coded because it is a color attachment?)

The following attachments can be referenced by a subpass:
* pInputAttachments are the ones read from a shader;
* pResolveAttachments are used for multisampling color attachments;
* pDepthStencilAttachment is used for depth and stencil data (TODO: is it in singular why?)
* pPreserveAttachments are used to preserve data for subpass communications.

##### The Render Pass

The attachment is created (a color attachment) and referenced in a subpass description. The render pass is ready to be created.
RenderPass is called with a certain frequency during the application execution, so, the reference to it must be preserved. This means, a VkRenderPass attribute must be created... a private one, of course! Do it right before the pipelineLayout.

```C++
VkRenderPass renderPass;
```

Vulkan object creation demands structure definition passed as argument in a creation instruction. It would be no different for RenderPass objects which stores the number of attachments, the array of attachments (and this is where the attachment reference uses the index), the number of subpasses and the array of subpasses. The data must be fed to a vkCreateRenderPass instruction.

```C++
VkRenderPassCreateInfo renderPassInfo = {};
renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
renderPassInfo.attachmentCount = 1;
renderPassInfo.pAttachments = &colorAttachment;
renderPassInfo.subpassCount = 1;
renderPassInfo.pSubpasses = &subpass;

if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS){
    throw std::runtime_error("failed to create render pass!");
}
```

And of course, there it is, an explicit creation. Every explicit creation demands an explicit destruction. This one uses the device, so, must be destroyed before the device is.

```C++
void cleanup() {
    ...
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    ...
}
```

#### Creating the Graphics Pipeline Object (VkGraphicsPipelineInfo)

Now the WindowAppWrapper configures the graphics pipeline by specifying the shader stages, the fixed function state, the pipeline layout and the render pass. It is time to properly create the graphics pipeline.
Just recollecting:
* Shader Stages specifies the shader modules and aggregates them to define the functionality of the programmable stages of the graphics pipeline. The shader modules are assigned to shader stages that specifies the kind of shader and the shader entry function;
* Fixed function state is the fixed function state machine. Unlike OpenGL nearly everything needs to be set, but the main objective is to get a state machine configured for rendering, like OpenGL basically is;
* Pipeline layout: configures the communications between the application and the shaders;
* Render Passes configures the buffers and how they are used in the graphics pipeling using the communications set.

It is time to finish the configuration and create the graphics pipeline using the structure. This needs to be done before the shaders are destroyed and of course, this means it is done in the `createGraphicsPipeline` method.
The structure holds how many shader stages are sed, the address of the array with shader stages, each of the fixed function structures created, the pipeline layout the renderpass and the index of the subpass where this graphics pipeline will be used.
The graphics pipeline is configured specifically for a render pass, but there are some render passes configuration that provides "compatibility" and can be assigned to the same graphics pipeline. To be compatible they must comply to [some characteristics](https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#renderpass-compatibility). All resumes to the attachments.
There are two optional parametes, the handle and the index, that can be used to create derived (new) pipelines. This idea is to provide a differential way of creating pipeline, reducing code written and instructions executed. For now, they will be a null handle and an invalid index (-1).

Preparing for the final steps of graphics pipeline creation, it must be referenced within the application, so, you know what to do. Create a private attribute! Right!

```C++
VkPipeline graphicsPipeline;
```

And configure the structure:

```C++
VkGraphicsPipelineCreateInfo pipelineInfo = {};
pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
pipelineInfo.stageCount = 2;
pipelineInfo.pStages = shaderStages;

pipelineInfo.pVertexInputState = &vertexInputInfo;
pipelineInfo.pInputAssemblyState = &inputAssembly;
pipelineInfo.pViewportState = &viewprotState;
pipelineInfo.pRasterizationState = &rasterizer;
pipelineInfo.pMultisampleState = &multisampling;
pipelineInfo.pDepthStencilState = nullptr; // Optional
pipelineInfo.pColorBlendState = &colorBlending;
pipelineInfo.pDynamicState = nullptr; // Optional

pipelineInfo.layout = pipelineLayout;

pipelineInfo.renderPass = renderPass;
pipelineInfo.subpass = 0; //Subpass index to start. Like entry point

pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
pipelineInfo.basePipelineIndex = -1; // Optional

if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS){
    throw std::runtime_error("Failed to create graphics pipeline");
}
```


Hey! This creation routine has more functions... how so!
For starters, realize that it is in plural. So, it is designed to create more than one object!
The second parameter is an optional parameter, which is null in this example. It references a VkPipelineCache object. An object which can store and reuse data relevant to pipeline creation across multiple calls to the creation function. Even across application execution if the cache is stored in a persistent unit, making possible to speed up pipeline creation in future executions.
Since the graphics pipeline is demanded for the drawing operations and is explicitly created, it is used throughout the application, a reference to it is already set as private attribute, and need to be explicitly destroyed... at the end of the program!

```C++
void cleanup() {
    ...
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    ...
}
```

A lot has been done. How about run your program?!



Next: Coloring the plain triangle Immediate Mode
