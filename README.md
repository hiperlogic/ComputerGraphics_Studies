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

## 02 - Plain Color Triangle

So far the instructions discussed how to configure CMake, create a window app (framework) and cconfigure OpenGL for immediate or retained mode printing a plain triangle.
In this project it will be shown how to change the color of the triangle, still maitaining it plain, in both modes: Immediate Mode and Retained Mode.
So, create a new project in CMakeLists. I would name it 02_OpenGL_plain_colored_triangle. I won't create two different projects for immediate and retained mode.

### Immediate Mode

The immediate mode commands are processed as soon as they are sent. This means, they set the state machine right away.
Simply put, the command glColor will set the current drawing color, so each vertex sent will be considered to be drawn with that color.
Considering the code written for the immediate mode in the previous project, let's write the command right before the start of the loop in the mainLoop method.

```C++
glColor3f(1.0, 0.1, 0.8);
```

And we are set. You have a quite pinkish triangle. Want to change its color, change the values and recompile.

Question: Can I put the color changing within the loop? Let's take a look at that in the next section.

## Vulkan - Rendering and Presentation

So far we still haven't produced "much" with Vulkan. All that was done was configuring the hardware, things that OpenGL does under the hood, following its own rules. And it is understandable. OpenGL is a Graphics API, while Vulkan is a Graphics Hardware Driver API, that means, it is used to configure and communicate with graphics hardware, and this demands more steps, set ups and instructions. But it also allows the configuration for better performance and/or resource usage.

What was the steps so far:
- Create the Vulkan Instance
    - Verify and Check validation layers (if Debug)
    - Configure the application information
    - Configure the instance information
    - Enumerate and Enable the instance Layers and Extensions
    - Configure the debug callback (if debug)
    - Create the instance with the configured instance information
    * `instance` stored for future references
- Setup the debug messenger callback
    * `debugMessenger` stored for future references
- Create the Surface using the WSI extensions
    - In this series it was done using GLFW.
    * `surface` stored for future references
- Set the Physical Device
    - Declare `desired device extensions` (so far: swapchain)
    - Enumerated Devices
    - Selected a Suitable Device
        - probed Queue Families
            (created structure to store queue family indices)
        - probed Extensions Supported
        - Probed Swap Chain Support
    * `physicalDevice` stored for future references
- Create Logical Device from Physical Device
    - Probed Queue Families
    - Declared create info configurations for each unique family according to indices
    - Enumerated desired physical device features
    - Declared create info configuration for the device
        - Set the number of queue create infos according to unique families
        - set the queue create infos data
        - set the number of `desired device extensions`
        - set the `desired device extensions` data
        - enable validation layers if debug
            - set the number of validation layers
            - set the validation layers data
    - Create the logical device from the physical device
    * `device` stored for future references
    - Create one queue for each queue family
    * store each queue for future reference
- Create the swapchain
    - query for swapchain support in the physical device and retrieve it
        - get surface capabilities
        - get surface format
        - get surface present modes
    - select surface format
    - select present mode
    - select extent
    * store `swapChainImageFormat` for future references
    * store `swapChainExtent` for future references
    - set image count to one more than the capabilities image count
    - adjust if image count > than capabilities maximum image count
    - Declare the swapchain create info structure
    - set the surface
    - set the min Image count
    - set the surface format
    - set the extent
    - set the array layers
    - set the image usage for the swapchain
    - probe and retrieve the queue families indices
    - Adjust for each unique family indices
    - set any transformation to be applied prior to retrieving data
    - set the composite alpha
    - set the presentMode
    - set if clipped
    - set the old swapchain
    - create the swapchain with the create info structure
    - `swapChain` stored for future reference
    - retrieve the image count set from the swapchain
    - adjust the size of the `swapChainImages` array (vector)
    - Retrieve the images from the swapChain into `swapChainImages`
    `swapChainImages` stored for future reference
- Create the image views
    - resize the `swapChainImageViews` vector to the name of swapChainImages
    - for each swapChainImages declare a `VkImageViewCreateInfo` structure
        - Set the image assigned to it. Use the index
        - Set the view type
        - set the `swapChainImageFormat`
        - set components (r, g, b, a) information
        - set fields for subresourceRange for image resolution processing
            - Set aspect mask
            - set baseMipLevel
            - set levelCount
            - set baseArrayLayer
            - set layerCount
    - Create the `swapChainImageView` using the filled structure
    * `stapShainImageView` stored for future reference
- Create the Render Pass
    - Declare and fill attachment descriprions
    - Declare attachment references
    - Assign the attachment references to the attachment descriptions
    - Declare subpass description
    - assign attachment references to the subpass
    - Declare render pass create info structure
        - set attachment count
        - set the attachments (array)
        - set the subpass count
        - set the subpasses (array)
    - create the `renderPass`
    * `renderPass` stored for future references
- Create the Graphics Pipeline
    - (this was done last section... and was massive!)
    - Set shader stages
        - read shader files
        - create shade modules
        - set shader stages infos
    - Set fixed functions
        - Input State
        - Assembly State
        - Viewport State
        - Rasterization State
        - MultisampleState
        - ColorBlendAttachment State
        - Color Blending state
        - set the Dynamic state
    - Declare and configure the Pipeline Layout Create Info structure
        - Set Layout Counts
        - Set the layouts
        - set the constant range count
        - set the constant ranges
    - create the pipeline layout using the structure
    * `pipelineLayout` is stored for future reference
    - Declare and configure the Graphics Pipeline Create info structure
        - Set the (shader) stage count
        - Set the shader stages
        - set the fixed function states
        - set the layout
        - set the renderpass
        - set the subpass index to start
        - (opt) set the base pipeline handle or
        - (opt) set the base pipeline index
    - Use the structure to create the graphic pipeline
    * `graphicsPipeline` is stored for future reference
    - Destroy shader modules

- cleanup explicitly created (and stored) references/objects when code ends.

### Buffers (framebuffers)

And if the vertices and color are hardcoded in the shaders, why it is still not drawing?

Last branch the render pass was set up to expect a sigle framebuffer (to be used as color buffer), with the same image format as the swap chain images.
However, the attachments specified during renderpass are bound by wrapping them into a VkFramebuffer object, and none was created yet.
Framebuffer object references all of the VkImageView objects that represent the attachments. As mentioned, only one, a color attachment, is configured for this example.
The image that we have to use for the attachment depends on the image the swap chain returns in the retrieval for presentation.
Let's create the framebuffer for each of the images in the swap chain, using the one that corresponds to the retrieved image at drawing time. However, an ideal situation would be to create one framebuffer for each attachment and a mechanism to assign in runtime the imageview. (TODO: Investigate this ideal alternative)

So, let's create the framebuffers as private attributes:

```C++
std::vector<VkFramebuffer> swapChainFramebuffers;
```

And a new private method `createFramebuffers` to be called right after the graphics pipeline creation.

This method must configure the size of the swapChainFramebuffer array and iterate through the image views, creating one framebuffer for each.
The framebuffer create structure must be set with the configured renderpass, a number of attachments (and their quantity), only the color attachment (via imageview) as we defined, width and height and the layers.

```C++
void createFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for(size_t i=0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {swapChainImageViews[i]};
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS){
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}
```

And since it is explicitly created, and maintained in the class, it must be explicitly destroyed in the cleanup, before the image views and the render pass:

```C++
...
for(auto framebuffer : swapChainFramebuffers) {
    vkDestroyFramebuffer(device, framebuffer, nullptr);
    ...
}
```

Now we have all the objects needed to draw the triangle. But to do that the draw command must be issued.
Vulkan does not process commands in the same manner as OpenGL does. The commands are sent as instructions to a queue for processing. To perform this, a command buffer must be defined. What about the queue? We already defined it, it is the graphics queue, it receives the drawing instructions.

### Command Buffers

Commands, drawing operations and memory transfers, all of these kind of instructions are not executed directly using function calls, they must be recorded, aggregated, sorted in command buffer objects. The hard work of setting these objects can be done in advance, and in multiple threads, then just tell Vulkan to execute the commands, they are ready. This would be the equivalent of the retained mode in OpenGL.

#### Command Pools

Commands created must use command pools, objects that manage the memory used to store the buffers and command buffers allocated from them. Since they are referenced within the application, it is suitable to create an attribute, lets make it private:

```C++
VkCommandPool commandPool;
```

Then code the method to create and assign it. Object creation in Vulkan follows the usual pattern of declaring the create info structure, assign values to its fields and call the command to create with the desired configuration. For command pools there are only two parameters, which family queue does the commands belong and the flags.
Since the commands to be sent at this moment are for drawing, the index for the graphics family is the one to be used. The possible flags are:
- VK_COMMAND_POOL_CREATE_TRANSIENT_BIT hints that the command buffers are rerecorded with new commands very often, the "pattern" of the command buffer changes frequently;
- VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT allows command buffers to be rerecorded individually. Without this flag all of them have to be reset together.

In this example there will be only one command buffer recod and it will be used without change. So no flags will be used.

```C++
void createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = probeQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = 0;

    if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}
```

And clean it up explicitly, as expected! Since it is used until the application ends, clear it right before cleaning the device.

```C++
vkDestroyCommandPool(device, commandPool, nullptr);
```

#### Command Buffer Allocation

Once the pool is set up, it can be used to allocate the buffers, and those to record the drawing commands.
Command buffers are freed once their command pool is destroyed, there is no need to explicitly destroy them in the cleanup.
For the moment a command buffer will be created for every image in the swap chain once again. This requires an array of VkCommandBuffer objects, so, let's create them as private attributes. (TODO:) Is there a reason for this to be this way, or is this just a lazy way?

```C++
std::vector<VkCommandBuffer> commandBuffers;
```

And code the method to create them. As before, the vector size must be adjusted to the number of swap chain framebuffers and then the info structure configured for the creation command.
The command buffer allocate info structure need to know the commandPool that will allocate the buffers, the number of command buffers to allocate and what is the command buffer level to be assigned.

The command buffer level can be of primary or secondary level:
* VK_COMMAND_BUFFER_LEVEL_PRIMARY can be directly submitted to a queue for execution, but cannot be called from other command buffers;
* VK_COMMAND_BUFFER_LEVEL_SECONDARY cannot be directly submitted to a queue, but can be called from primary command buffers.

Second level command buffers can be really usefull to store standard commands (like code blocks or routines) to be called from primary buffers.
To render the triangle only the primary level will be needed.

```C++
void createCommandBuffers() {
    commandBuffers.resize(swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    // CONTINUE...
}
```

And the command buffers are ready, they can begin recording the commands.

#### Starting command buffer recording

To start recording a command buffer a VkCommandBufferBeginInfo structure must be configured to specify the command buffer usage. It is used in the vkBeginCommandBuffer instruction that effectively starts the command buffer.
The structure fields specify the flags that configures the command buffer usage and information about state inheritance when called from other command buffers, this means it is only set to secondary level command buffers.

The flags possible values are:
* VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT that rerecords right after executing once;
* VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT is only used for secondary level command buffers and used entirely within a single render pass;
* VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT indicates that even if there are pending execution, the command buffer can be resubmitted.

For the moment, none are applicable.

If the command buffer was already recorded, it will be reseted. They were not designed to append commands after the definition.

```C++
    // Continuing
    for (size_t i=0; i<commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        // Continue recording
    }
```

##### Starting a Render Pass

Drawing begins by starting the render pass. The instruction vkCmdBeginRenderPass is the command that triggers it, but it must be configured with a VkRenderPassBeginInfo.
The info structure to begin a renderpass configures which renderpass object is to be used, the framebuffer (or attachment) to be used, the area and position configuration to render, pixels outside this area will have undefined values.
Finally, the values to clear the framebuffers (or attachments). In this example, since the attachment is a color attachment, that defines a color buffer, the value must be a data representing an RGBA color.
The render pass is ready to begin its operations and the functions that record commands, prefixed with vkCmd, can be issued. All of them return `void`, so no error handling is performed (TODO: for now? or in fact?).

The commands have a pattern.
The  first parameter is always the command buffer where the command will be recorded, followed by details of the render pass provided. The final parameter controls how the drawing commands within the render pass will be provided. This control is provided by the following enumerations:
* VK_SUBPASS_CONTENTS_INLINE indicates that the render pass commands will be embedded in the primary command buffer and no secondary command buffers will be executed;
* VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS specifies that render pass commands will be executed from secondary command buffers.

Since no secondary buffers will be used, the first option will be set

```C++
    // Continuing recording

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[i];

    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Continue recording
```

#### Drawing! (Finally!)

To provide the drawing commands, it is necessary to bind the graphics pipeline to the command buffer specifying if the pipeline object is a graphics or a compute pipeline.
(TODO: Compute pipeline is related to the computing shaders?)

```C++
    // continuing recording
    vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    // continue recording
```

Now the command buffer send the command to the pipeline with a draw command. We are drawing a triangle, providing 3 values, the vertices, that are hardcoded in the shader but they must be counted, it is the vertex count. The next value is the instance count, used for instanced rendering, a technique that is used for crowd or landscape rendering that uses inherited object properties to generate new ones with slight modifications. Since this is not being used, use the value 1.
The last two parameters are the offset into the vertex buffer and instanced rendering. Vertex Buffer offset will be discussed shortly in OpenGL. Here both are not being used, so, use the value 0.

```C++
    // continuing recording
    vkCmdDraw(commandBuffers[i], 3 , 1, 0, 0);
    // continue recording
```

The render pass can now end, as well as the command buffer, since this is the only command:

```C++
    // continuing recording
    vkCmdEndRenderPass(commandBuffers[i]);

    if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS){
        throw std::runtime_error("failed to record command buffer!");
    }
```

And... no, you won't be able to see anything yet.

### Rendering and Presentation

What you have done so far is the graphics card configuration of your "rendering" machine, a very simple one, yes, but is is a rendering machine, with the entry points to call its own features.
Now it can be configured for your application (there is almost no configuration, since it is very simple) and used.
In OpenGL, in the mainLoop, it calls OpenGL commands to draw. Vulkan is more elaborated, it needs more instructions, so, let's create another method to draw the frames.
Operations in Vulkan are paralelized. This means that things can go out of order is no synchronization is performed. And paralel operations can trigger race conditions.
To avoid race conditions some structures need to be used, like semaphores or fences. Let's keep with semaphores and create some.
Now the image can be acquired from the swap chain and the imageview framebuffer can be sent to the command buffer, with subpass dependencies set.
Finally, return the framebuffer to the swap chain so it can present the image.
Sync image creation command submition and image presentation (and framebuffer) consumption adjusting semaphores (available and finished) for each frames;

#### The Setup

    (TODO: The Text)
    Create the drawFrame method. Call it within mainLoop after PollEvents.

#### The Synchronization
    drawFrame
        Acquire image from swap chain
        execute command buffer with that image in the framebuffer (command to graphics queue)
        return the image to swap chain for presentation (image to present queue)

Events are set with a single call. But instructions take time to be executed.
There will be more calls than executions. This needs to be sync'ed.
Two types of sync: Fences and Semaphores
Fences can be queried. Semaphore can't.

##### Semaphores

create the semaphores (two private attributes) and the createSemaphores method

```C++
VkSemaphore imageAvailableSemaphore;
VkSemaphore renderFinishedSemaphore;

void createSemaphores() {
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if( vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS
        ||
        vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS){
            throw std::runtime_error("Failed to create semaphores!");
    }
}

void cleanup(){
    ...
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
}

```

##### Acquiring image from Swap Chain

```C++
void drawFrame() { 
    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
}
```

##### Submitting the command buffer

```C++
void drawFrame() { 
    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex]; // (See TODO: from last branch about command buffers and images)

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Here is where the queue comes in place!
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }
}
```

##### Subpass Dependencies (TODO: Put this on the create render pass text! This belongs to previous branch)

```C++
// create render pass
VkSubpassDependency dependency = {};
dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
dependency.dstSubpass = 0;

dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
dependency.srcAccessMask = 0;

dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//...
renderPassInfo.dependencyCount = 1;
renderPassInfo.pDependencies = &dependency;
```

##### Presentation

```C++
void drawFrame() { 
    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores = { imageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBufgfers = &commandBuffers[imageIndex]; // (See TODO: from last branch about command buffers and images)

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Here is where the queue comes in place! Send the commands (in command buffer) to the Graphics queue
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    // ...
    // Retrieve the image rendered from the swapChain and send it to the presentQueue for presentation
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResulte = nullptr;

    vkQueuePresentKHR(presentQueue, &presentInfo);
}
```

Now you can compile... and see the result!

Next: Changing Color with the App in the Immediate Mode
