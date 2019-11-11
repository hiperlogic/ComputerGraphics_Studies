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

## [01 - C - Answers about configuring the Vertex Attrib Array](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/01_c_Plain_Triangle_Programmable_Pipeline_Answers/README.md)

## [02 - Plain Color Triangle <br>  Immediate Mode](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/02_Plain_Color_Triangle/README.md)

#### More Colors

In the previous example the draw color was set prior to the start of the loop. This means that it is set only once.
If the draw color is set within the loop, each time the loop executes, the OpenGL state machine value for the current draw color is set. This can be a waste of resources if only one drawing is what is done, but can also provide some nice effects.
So, answering the question, yes, the glColor3f command can be written within the loop.
To show this let's provide a new example. Let's configure three colors within one vector and use an index and GLFW keyboard routines to change it.
First, the colors setup is written outside the loop, but within the mainLoop method:

```C++
float colors[] = {1.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.8f, 1.0f, 1.0f, 0.0f}; //Red, somewhat violet and yellow
```

We configure an index to indicate the current color:

```C++
int corrCol = 1; // The somewhat violet is the color of the previous code
```

And set the current color, all just prior to the loop:

```C++
glColor3f(colors[corrCol*3], colors[corrCol*3+1],colors[corrCol*3+2]);
``` 

That instruction also needs to go within the loop, because we want it to affect the drawing when we press a key.
If it is coded only outside the loop, it will not be processed again and the color will be just the same.
Where to put it within the loop is up to you. It can be before clearing the buffers, between clearing the buffers and the drawing instructions, in the beginning or in the end of the drawing instructions, after the drawing instructions, after swapping buffers, even in between the vertices within the drawing instructions, although this would result in a quite weird picture for the very first frame (so you wouldn't notice)!

Finally, the code for the color change. What it will do is just increment the corrCol value and modding it with 3.

```C++
                if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
                    corrCol = (corrCol+1)%3;
                }
```

This way, when you press space, the color will change.
It will go very, very fast... I know... I just show how to change colors, not how to handle key presses. Good luck with that!

## Vulkan

Now the red triangle is being drawn on the screen, on a black background, but there are still improvements to the pipeline to be made.
In the end of the last branch an instruction were added to the end of the drawFrames method in order to "fix" the messages provided by the validation layer. But this instruction tells the whole pipeline to wait for the presentQueue to be idle, making it produce just one frame per iteration.
The stages that the pipeline processed through are idle and could be used to produce data for the next frame. Think of the stages as the computing pipeline stages, to fetch instruction, fetch data, execute, write, ... (remember the Computer Architecture classes!).
Multiple frames can be produced (_in flight_) while still bounding the ammount of work that piles up.

All we need to do is to specify the ammount of frames to be processed concurrently. This can be done declaring a constant.

```C++
const size_t MAX_FRAMES_IN_FLIGHT = 2;
```

And specifying a pair of semaphores to each frame, not to the pipeline.

```C++
/* replacing these
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
*/

std::vector<VkSemaphore> imageAvailableSemaphores;
std::vector<VkSemaphore> renderFinishedSemaphores;
```

Change the method that create the semaphores to create these:

```C++
void createSemaphores() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create semaphores for a frame!");
        }
}
```

And, of course, destroy them on the cleanup.

```C++
void cleanup() {
    ...
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
    }

    ...
}
```

And, since the semaphores are bound to the frames, it is important to track the current frame. A private attribute will be needed for this. Initialize it with 0.

```C++
size_t currentFrame = 0;
```

The semaphores are used in the drawFrame method, so it need to be updated:

```C++
void drawFrame(){
    ...
    vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    ...

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };

    ...

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};

    ...
    // vkQueueWaitIdle(presentQueue);  // This won't be needed anymore!

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
```

You can compile it and run to check the results.
And the results are... the messages are back. Yes, it is the same kind of messages we were having before adding the queue wait idle.

That's because the semaphores for each frame provides a synchronization process from GPU to GPU, not between CPU and GPU, and the drawFrames method is called in the application, via CPU.

To perform CPU-GPU synchronization there is a second type of synchronization primitive offered by Vulkan, the _fences_.
Fences are similar to semaphores, they can be signaled and waited for, but this time we will code the wait in the code. So far, the semaphore wait is implemented in Vulkan API, we just specified which semaphore is to be checked.

Right next to the semaphores, create the fence, one for each frame. Call it `inFlightFences`.

```C++
std::vector<VkFence> inFlightFences;
```

Since the sync objects are no longer just semaphores, rename the createSemaphores method to createSyncObjects.
The fence is created via the `vkCreateFence` instruction. It needs the device the fence will monitor, the fenceInfo structure, the optional allocator callback and the memory address to store the fence object.
The `VkFenceCreateInfo` structure can specify some flags for the fence creation. Since we will be waiting for the fence to be signaled in order to proceed with the frame generation, the fence will be created already signaled.

```C++
void createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}
```

and the destruction

```C++
void cleanup() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    ...
}
```

Now the fence can be used for synchronization. The `vkQueueSubmit` instruction accepts an optional parameter to pass a fence that should be signaled when the command buffer finishes executing. This signal will be used to indicate that a frame has finished.

```C++
void drawFrame(){
    ...
    if(vkQueueSubmit(graphicQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS){
        ...
    }
    ...
}
```

And the fences must be adjusted in order to make the drawFrame, that is, the CPU method, wait for them to be finished. For this the `vkWaitForFences` is called for the device, specifying how many fences to wait on, the array of fences, the waitAll condition and the timeout.
As occurrs with semaphores, if the maximum value for an unsigned integer is passed, the timeout is not considered.
The waitAll condition indicates if all the fences must be signaled in order to continue. If true and at least one fence is not signaled, the code waits for the condition to be met.

```C++
void drawFrame(){
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFences[currentFrame]);
    ...
}
```

The application can be compiled and executed. But there is a catch.
If the number of MAX_FRAMES_IN_FLIGHT is higher than the number of swap chain images, or if vkAcquireNextImageKHR returns images out of order, then it is possible that the rendering may happen into a swap chain image tht is already _in flight_. This demands tracking, for each swap chain image, if a frame in flight is currently using it.
We already are using fences to synchronize frames, and fences can have their states tracked. So, create another fence for images in flight and resize it on the createSyncObjects method. The imagesInFlight vector of fences will hold the number of images in the swap chain, all of the positions will be initialized with a null value.

```C++
void createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

    ...
}
```

It is on the drawFrame method that the value will be queried. The image is acquired, in fact, the image index, and the index in the imagesInFlight will be compared. If the index holds a null, it is ready to be used, else, it needs to wait.
Also, we have more calls to `vkWaitForFences`, so it is pertinent to move the `vkResetFences` call. A suggestion is to reset the fences right before actually using them, that means, right before submitting the information to the graphics queue.

```C++
    void drawFrame() { 
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;

        vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if(imagesInFlight[imageIndex] != VK_NULL_HANDLE){
            vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }

    ...

        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        // Here is where the queue comes in place! Send the commands (in command buffer) to the Graphics queue
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }


    ...
```

Now the system is synchronized. No more than two frames are enqueued and the synchronization process guarantees that they do not use the same image.

There are more examples of synchronization scenarions provided by [Khronos](https://github.com/KhronosGroup/Vulkan-Docs/wiki/Synchronization-Examples#swapchain-image-acquire-and-present) (TODO: Check out the synchronization examples)

And now we can safely say that the shader hardcoded triangle is finaly completed, with the Vulkan bootstrap adequately configured. It was a lot of work, but once the Vulkan configuration process is understood, the remainder is general computer graphics. Vulkan provides extensive control of the device to the developer and that adds complexity.

So, I'd say, we are closing the gap with OpenGL. 

One thing though! We can't resize the windows!

###  Swap chain recreation.

Next: Plain Colored Triangle Retained Mode, Version 1 - The Fragment Shader Configuration
