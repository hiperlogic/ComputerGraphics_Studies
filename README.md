# ComputerGraphics_Studies
- Systems Covered (So Far): OpenGL, Vulkan
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

## [01 - C - Answers about configuring the Vertex Attrib Array](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/01_c_Plain_Triangle_Programmable_Pipeline_Answers/README.md)

## [02 - Plain Color Triangle <br>  Immediate Mode](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/02_Plain_Color_Triangle/README.md)

### [<p>   More Colors</p>](https://github.com/hiperlogic/ComputerGraphics_Studies/tree/02_a_Plain_Color_Triangle_Immediate_Mode_More_Colors)

### [Retained Mode - Programmable Pipeline and More Colors](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/02_b_Plain_Color_Triangle_Retained_and_Shaders/README.md)

### Using the Application to Change the Color

Shader and application are somewhat decoupled. The application send its data to the pipeline, which, as we experienced, can or cannot have shaders, although without shaders the result may not be what we expect.
The application prepares the data, the shaders process the data and generate the result. This means that apart from the usual vertex attrib pointer in the application and inbound variables in the shader, there must have some other form to inform parameters to the shaders, this is where the `uniform` keyword is used.

The `uniform` keyword, indicate variables whose values are retrieved from the application. They can be defined either in vertex or fragment shader. So, this is what we are looking for.

In order to use a uniform variable, apart from defining it in the shader, the variable needs to be located in the shader program from within the application. A process that is done with the command `glGetUniformLocation`, which receives two arguments: the program and a string containing the name of the variable.

If we define the variable in the vertex shader, since in this particular case it is a value that indicates the color to be drawn, it also needs to be output to the fragment shader, but it can be written directly in the fragment. Let's use the easiest form for this project and write it in the fragment shader.

Just after currColor, write the following sentence:

```C++
uniform vec4 recColor;
```

This declares a vec4 variable with the name recColor whose value is retrieved from the application.
Now we need to map this color to the output color just rewriting the only instruction in the main routine:

```C++
currColor = recColor;
```

Just make sure to set the recColor prior to calling the draw instructions and we're set for the fragment shader.

#### Passing the values to the Uniform.

It is the application responsibility to probe for the information where to store (send) the value on the OpenGL state machine so it can be retrieved by the desired uniform variable.
In order to do it, the `glGetUniformLocation` must be provided the program and the name of the variable, returning an identification to be used as reference to set the value in future instructions. The instruction needs to be declared after the program is set to be used, so, after the shaders deletion just write the following:

```C++
GLuint recColor = glGetUniformLocation(shaderProgram, "recColor");
```

That retrieves an identification so that when there is the need to change the color in the fragment shader, it can be done within the application. The application variable name and the shader variable name are just incidental, they can be different. The decision to name them the same is just for sake of simplicity in knowing which context is being dealt with.

In the fixed function pipeline example we set up an array with 9 values representing 3 colors and used an offset and the specific positions with glColor3f instruction to change the draw color, but the value in the shader is a vector of 4 floats. Also, glColor won't work with programmable pipeline.

Since the reference for the variable in the shader was retrieved by the application, we can set its value directly just calling the instruction glUniform, wich has a naming construction similar to glColor or glVertex.

So, let's create the 9 values vector for the color, as did in the fixed function pipeline, the integer variable storing the current color and the keyboard processing instructions. 

All that is left to do is to code the instruction that would do the function of glColor, meaning, the instruction to effectively set the current draw color. In this example this is done by setting the values in the fragment shader, in the uniform variable, so, it is just a matter to call, within the loop, the instruction:

```C++
glUniform4f(recColor, colors[corrCol*3], colors[corrCol*3+1],colors[corrCol*3+2], 1.0f);
```

With that the project is ready to be compiled and tested.

## Vulkan

Now that the vertices are being provided to the shaders, although using a not very efficient method, it is time to address the issue of using the application to provide other values to the shader. Values that does not change that often. Better memory transfer procedures will be presented later.

In the OpenGL section the uniform data modifier was already discussed, and in OpenGL the binding and data transmission from application to the shader using uniforms is trivial, just get the uniform location from the program and use it to send the value.

In Vulkan this needs a more intricated setup involving `resource descriptors`, a way for shaders to freely access resources, like buffers and images. Does this mean that instead of the application passing the value, it is the shader that reads it? (TODO: Verify). This would be interesting!

Let's set up a buffer that contains the triangle color and have the vertex shader access it using a descriptor.
To declare a descriptor three steps are needed:
1. Specify a descriptor layout during pipeline creation
2. Allocate a descriptor set from a descriptor pool
3. Bind the descriptor set during rendering.

The descriptor layout specifies the types of resources the pipeline will access. This is something lika the renderpass specifies the types of attachments to be accessed.
The descriptor set specifies the location where the data is stored. In Vulkan it is a buffer (or image) resource. This will be bound to the descriptor.
Then, the descriptor set is bound for the drawing commands, just like the vertex buffers and framebuffers.

There are many type of descriptors, but since we are dealing with `uniforms`, the type used is the Uniform Buffer Object (UBO). Other descriptor types will be addressed in future sections.
The data to be communicated to the shader is a color data, so lets create a simple structure:

```C++
typedef struct Color_UBO {
    glm::vec4 color;
} Color_UBO;
```

Then this data can be copied to a VkBuffer and be accessed via uniform buffer object descriptor, like the following:

```C++
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
```

The intention is to send the drawing color each frame to be passed to the fragment shader. (This method of sending the color value works, but is not very effective. This is just being done here to illustrate how the result coded for OpenGL in this section can be done with Vulkan. It would be something like this, but Vulkan works differently, so, this probably is not a good idea. At least illustrate how to set up a `uniform`).

The fragment shader need to be changed as well.


```C++
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 inColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = inColor;
}
```

Name both `color_triang.vert` and `color_triang.frag` and compile them to `color_triang_v.spv` and `color_triang_f.spv`, respectively.

Note, in the vertex shader, the `binding` directive. It works similarly to the location directive and will be used in the descriptor layout. 

### Descriptor Set Layout

If you haven't defined the `Color_UBO` yet, do so. This is the structure to configure Vulkan telling about the descriptor in the vertex shader. Luckilly GLM provides exact matches for the types to be passed to the shaders.

Now it is time to create the method that will configure the descriptor set layout. Call it `createDescriptorSetLayout` and code it as a private method. Call it from initVulkan right befor creating the graphics pipeline.

Every binding needs to be described through a binding struct. In the event of the uniform data, it must be done via `VkDescriptionSetLayoutBinding`, which will indicate the `binding` index and coded in the shader, the descriptor type and the descriptor count.
The binding for this example is 0, and it is the only one, the type is a VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER and the count is 1.
We are passing just one data, but a set of data could be passed. For instance, to pass parameters for parametric mesh deformations, like bones in a skeleton to deform the related vertices.
The descriptor must also specify which stage it is bound too. In OpenGL this would be passed to the program, so it would be something like for the VK_SHADER_STAGE_ALL_GRAPHICS (TODO: Would it?), but in Vulkan this can be specified for the VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT, VK_SHADER_STAGE_GEOMETRY_BIT, and several [other stages](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkShaderStageFlagBits.html).
In this example we are passing it ti the vertex shader, in the OpenGL example (somewhat similar to this) it was declared in the fragment shader.
This is another field that will be addressed in the future, the pImutableSamplers, that deal with sampling related descriptors. For now set it to nullptr.

The descriptors need to be informed to the pipeline, and this is where the descriptor set layout comes to help. The name says it all, it is a set of descriptors, it is used throughout the application, so it must be declares as a private attribute.

```C++
VkDescriptorSetLayout descriptorSetLayout;
```

Now the descriptor set layout can be created. As every Vulkan object, the creation instruction needs a filled structure to create the object. For descriptor set layout the instruction is `vkCreateDescriptorSetLayout` and the structure is `VkDescriptorSetLayoutCreateInfo`, which provides how many bindings it describes and the array of bindings. The descriptor set layout need to be called after the renderpass and before the graphics pipeline.

And the createDescriptorSetLayout function would be:

```C++
void createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding c_uboLayoutBinding = {};
    c_uboLayoutBinding.binding = 0;
    c_uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    c_uboLayoutBinding.descriptorCount = 1;
    c_uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    c_uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &c_uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}
```

The descriptor set layour is being explicitly created, it needs to be explicitly destroyed. In cleanup, just after the swap chain cleanup, call the instruction 
```C++
vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
```

Finally, the descriptor set layout must be specified during the pipeline creation to tell Vulkan that the shaders will be using them. In the createGraphicsPipeline the VkPipelineLayoutCreateInfo need to be changed to reference the layout object:

```C++
VlPipelineLayoutCreateInfo pipelineLayoutInfo = {}
pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
pipelineLayoutInfo.setLayoutCount = 1;
pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
```

### Uniform Buffer

In order to specify the buffer that contains the uniform data to the shader, the buffer must exist.
We already looked out how to create the vertex buffer, and that the buffer must be assighed to an allocated memory in the device.

Since we are dealing with uniforms, let's create a vector to store as many as we need, because unlike the vectors, the uniforms are information that are requested per frames and the mechanism coded for Vulkan is processing frames concurrently, (in flight). So, the buffers will be provided per frames.

```C++
std::vector<VkBuffer> uniformBuffers;
std::vector<VkDeviceMemory> uniformBuffersMemory;
```

There are a number of buffers that can be created, and all of them are created following a pattern.
In order to avoid repeating code, let's create a method to create the buffer needed.
This method parameters need to specify the size to be allocated on the device, the buffer usage flags, the memory properties flags, the buffer reference and the reference to the buffer memory in the device.

```C++
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory){
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(device, &bufferInfo, nullptr, &biffer) != VK_SUCCESS){
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}
```

Those buffers need to be created and allocated. Create another private method and call it in initVulkan, just after the vertex buffer. Call it `createUniformBuffers`. 
The method need to know the size of the color uniform buffer object, resize the vectors to the number of images in the swapchain and be created.
Let's use the createBuffers helper function to perform the needed operations.

```C++
void createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(Color_UBO); // What do you think, could we use the vec4 directly here?

    uniformBuffers.resize(swapChainInages.size());
    uniformBuffersMemory.resize(swapChainImages.size());

    for (size_t i = 0; i<swapChainImages.size(); i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }
}
```

Since the uniform buffer update will be called via application there is no need to perform the memory mapping here. The uniform data will be used for all draw calls, this means that the buffer should only be destroyed when we stop rendering.
Since the uniform buffers depend on the number of swap chain images, which could change after a recreation, it is pertinent to clean them up in the cleanupSwapChain, which means that they also need to be recreated in the recreateSwapChain.

```C++
void cleanupSwapChain() {
    ...
    for (size_t i=0; i < swapChainImages.size(); i++) {
        vkDestroyBuffer(device, uniformBuffer[i], nullptr);
        vkFreeMemory(device, uniformBuffersMEmory[i], nullptr);
    }
}
...

void recreateSwapChain() {
    ...
    createUniformBuffers();
    createCommandBuffers();
}
```

### Updating uniform data

Unlike the vertex buffer, the uniform data is not memory mapped, this means that in order to update the value, the shader must be explicitly informed via application, as it is done with `glUniform*` for OpenGL.
To do that let's call the `updateUniformBuffer` method, that will be coded, within the drawFrame, passing the index of the image index assigned. Of course, this need to be called after the synchronization acquires the image and before the creation of the submitInfo.

Currently the updateUniformBuffer will only set the color of the triangle to be drawn, let's configure some colors to be applied. Let's also code the current color being used.

```C++
const std::vector<Color_UBO> colors = {
    {glm::vec4(1.0f,1.0f,1.0f,1.0f)},
    {glm::vec4(1.0f,0.0f,0.0f,1.0f)},
    {glm::vec4(0.0f,1.0f,0.0f,1.0f)},
    {glm::vec4(0.0f,0.0f,1.0f,1.0f)}
};

int currColor = 0;
```

Now, currColor can be used as the index for the proper value to be set to the uniform.

```C++
void updateUniformBuffer(uint32_t currentImage) {
    Color_UBO the_color = colors[currColor];

    void* data;
    vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(the_color), &data);
        memcpy(data, &the_color, sizeof(the_color));
    vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}
```

And we are almost there. Although this is not the fastest way to pass frequently changing values to shaders (and we are not! TODO: Can the updateUniformBuffer be called outside the drawFrame?), the proper method, using _push constants_ will be addressed in future sections.
Now it is time to actuallyu bind the buffer to the uniform buffer descriptor so the shader can access the data.

### Descriptor Pool

The Uniform Buffers are created and the Descriptor Set is defined. Now it is time to bind both. As usual create one more method, call it `createDescriptorPool`, calling it from initVulkan right after the call to createUniformBuffers.

The poolSize specifies the type of the buffer and how many descriptors will be created. In this situation, the number of swap chains images. They are used in the structure to create the descriptor pool.
The structure to create the descriptor pool specifies how many pool sizes it will provide, the address of the pool sizes array, as well as how many max sets it can reference, also, the size of swap chain images.

As with the other explicitly created structures, the descriptor pool needs to be retrieved throughout the application, so it will be declared as a private attribute in order to have its reference saved when calling vkCreateDescriptorPool. And it needs to he cleaned up as well, this time in the swap chain cleanup method.

```C++
VkDescriptorPool descriptorPool;

...
void createDescriptorPool() {
    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

...
void cleanupSwapChain(){
    // cleaned up uniform buffer and memory
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}

...

void recreateSwapChain() {
    ...
    createUniformBuffers();
    createDescriptorPool();
    createCommandBuffers();
}
```


### Descriptor set (VkDescriptorSetAllocateInfo)

Now the descriptor sets can be allocated via descriptor pool. As usual, let's create a private method for this, `createDescriptorSets` and call it right after creating the descriptor pool in initVulkan and in recreateSwapChain.


```C++
std::vector<VkDescriptorSet> descriptorSets;
...
void createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(swapChainImages.size());
    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS){
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for(size_t i = 0; i< swapChainImages.size(); i++){
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(Color_UBO);

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWeite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement =0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }

}
```

With the descriptor sets updated, the createCommandBuffers need to be adjusted to actually bind the right descriptor set for each swap chain image to the descriptors in the shader using the instruction cmdBindDescriptorSets, before the vkCmdDraw call.
Vertex and index buffers are unique to graphics pipelines, but descriptor sets are not and must be specified to which pipeline, graphics or compute, to bind. In this case, to the graphics. Then the layout the descriptor is based on, followed by the index of the first descriptor set and the number of sets to bind, that means, just one set, the index is 0 because we just have one. Then the descriptor set array. The last two are used for dynamic descriptors and will be addressed in the future.

```C++
vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
vkCmdDraw...
```

Now, let's create a public method to increase the curr color.

void changeColor(){
    currColor = (currColor+1)%4;
}

And call it when a key is pressed. Space, like in OpenGL, within mainLoop.

If you've followed this to the letter and it still does not work, check if:
1. You haven't commited a typo
2. I haven't commited a typo
3. You have initialized every configuration (the create* private methods) within initVulkan, because there are so many I may have forgotten.

And this is how you set an Uniform. Quite a lot.

Next, multi colored triangle. Let's extend the Vertex struct.

Next: Multiple Colored Triangle