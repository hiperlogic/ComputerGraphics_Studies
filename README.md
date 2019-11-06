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

## 01 - B - Sending Data to OpenGL - Vertex Buffer Objects

From OpenGL 2.0 onward a new pipeline was introduced, the `Programable Pipeline`, an improvement and simplification of the fixed function pipeline that added a generic processor to the `Transform and Lighting` stage, now called `Vertex Shading` and to the section related to the visual processing of the result sent by the rasterizer, that is the processes of `Texture Environment`, `Color Sum`, `Fog` and `Alpha Test`, which was labeled `Fragment Shading`.

So, we had the following in the fixed pipeline:

API->Primitive Processing->**_Transform and Lighting_**->Primitive Assembly->Rasterizes->**_Texture Environment->Color Sum->Fog->Alpha Test_**->Depth and Stencil->Color Buffer Blend->Dither->Frame Buffer.

And got the following in the programmable pipeline:

API->Primitive Processing->**_Vertex Shader_**->Primitive Assembly->Rasterizes->**_Fragment Shader_**->Depth and Stencil->Color Buffer Blend->Dither->Frame Buffer.

### The Vertex Shader

At first glance it seems that little to no change was made to the transformation and lighting processing, since one stage was replaced by another, but this is a wrong perception.
First of all, the `Transform and Lighting` stage is limited.
The OpenGL matrices operation needed to be called with OpenGL specific operations like `glRotate` and `glTranslate`, this made the programming easier, but the matrices data could not be changed once the vertices were sent. They were fixed.
Also, OpenGL had only 8 configurable light sources and like the matrices, they could be changed only via application.
With the Vertex Shader we have new possibilities. We can change the transformation matrix per vertex sent, if we want, producing several advanced effects like water refraction, for instance, in the vertex domain, not on the fragment domain, considerably speeding up the process.
The illumination model may have become a little bit more complex (and we will see it when talking about lighting), but the 8 light sources limit is no longer valid, nor the fact that they are static, but the Vertex shader is the one responsible to provide data related to surface normals to the next pipeline stage in order to the illumination be properly computed. So, the programmer has to permorm a little extra effort and have a little extra knowlegde about illumination, but gains a lot more liberty to perform the effects he wants.
To make things better, extra information can be used to process lighting parameters to be used by the fragment shader, such as Normal Maps, Bump Maps or Speculation Maps, producing really impressive graphics.
These features will be detailed in their respective section.

### The Fragment Shader

The fragment shader is the one responsible to indicate how the pixel will look like. It will retrieve data such as color to be used, texture vertices or normal from the vertex shader and use them to proceed with the computation.
The features related to the fragment shader will be detailed in the projects that will use them.

### Vertex Buffer Object

The vertex buffer object is what in the old computer graphics jargon would be called `retained mode`, in opposition to the immediate mode.
In immediate mode, as discussed in the previous section, the mechanism process at each vertex sent and the process to send the vertex was configured in code, in the application.
Within the retained mode the application configure objects once and send them to the graphics processing with all information ready, that means, all vertices are sent at once.
In OpenGL the vertex buffer object can store more than vertices, but let's keep it simple for the moment and keep the concept to the name.
The very first thing that needs to be done is to uncomment the OpenGL configuration for GLFW. Those commented block that contains the hint instructions in the intitSystem routine.
In the mainLoop routine, the instructions from glMatrixMode to glColor3f are no longer valid, they can be removed as well.
And, of course, the instructions from glBegin and glEnd need to go. Now you should have the same code as in the first project, it just opens the window.
The vertex buffer object is configured with data that are in contiguous memory area. Thus, the triangle to be displayed needs to be configured this way, with its vertices placed in an array as demonstrated in the snippet bellow:

```
            // The triangle vertices to be drawn. They are placed in a contiguous memory area
            GLfloat triangle_buffer_data[]={
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f, 1.0f, 0.0f
            };
```

This snippet was coded within the mailLoop routine, but outside the loop instruction, since it is there where the drawings are done. But it should not be coded there properly... we will look out to understand where better to code it in the future.

There are some required steps to configure a vertex buffer object in OpenGL. This means basically allocate resources in the OpenGL state machine and set up the state to be configured prior to sending the data to configure it.
Those steps are:
* Generate the Vertex Arrays *resources*, also called by *names* or *objects* in some literature. This is done with the command glGenVertexArrays;
* Bind a vertex array resource to the current OpenGL vertex array with the command glBindVertexArray;
* Generate the buffer *resources*, also called by *names* or *objects* in some literature. This is done with the command glGenBuffers;
* Bind a buffer resource to the current OpenGL buffer with the command glBindBuffer, specifying that it is an array buffer using the constant GL_ARRAY_BUFFER;
* Specify the buffer where the data are contained using glBufferData using the following parameters:
    * Buffer Type: This indicates to OpenGL what is the buffer type to be informed. It is desirable that this buffer type be the same as the current bound buffer, GL_ARRAY_BUFFER, in this example;
    * Number of bytes: How many bytes are there in the buffer. In this case there are 36 bytes, because each vertex is a float (4 bytes) and there are 9 data, 3 components (x, y, z) in each of the 3 vertices (9*4=36). In the code a shortcut was used with the instruction sizeof(buffer), that gives us exactly how many bytes in the buffer there is. This is useful since sometimes there are no other ways to know how many bytes there are in a buffer;
    * The buffer: This is the proper buffer;
    * Processing Type: This is a constant indicating to OpenGL how to better treat that data. In this example the buffer is used to pass information to the processing pipeline (draw) and does not change (static), so it is configured as GL_STATIC_DRAW.

All this is configured in the mainLoop, previous to the loop start.
Now, the vertex buffer is configured and bound to a vertex array. It can be used to draw.

### Drawing with vertex buffer objects

There are some steps to proceed to the drawing of the vertex buffer:

* Enable a vertex attrib array in the OpenGL state machine so it can proceed with processing a vertex array. For now we will use the instruction glEnableVertexAttribArray(0), where 0 is the index of the vertex attrib array;
* Bind the buffer to be used. Although in this example this instruction should not be necessary, since the last bound vertex buffer is the only vertex buffer used, it is present just for sake of safety and education. If another vertex buffer had been configured and that was the last one, this instruction would be necessary, so, the instruction is exactly the same as the previous one written: glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
* Configure how the vertices in the buffer are to be processed: This is quite a tricky part, simple in this example, but will get some complexity in the next ones. The configuration is done with the commang *glVertexAttribPointer* with the following parameters:
    * The index of the enabled vertex attrib array to be used (*0*, in our case);
    * The number of data the buffer holds: there are 3 vertices, so this is the value to be informed in this parameter;
    * The type of data in the buffer (GL_FLOAT in the example);
    * Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed;
    * Stride specifies a byte offset after the consecutive number of data to be processed. In this case it is 0, since we have only the vertices;
    * Pointer specifies where in the buffer the data starts. In the example is 0.

The last two parameters will be better covered in the next example, the fourth parameter will be discussed later.

So, this is the configuration stage. The vertex attrib array is enabled, the vertex buffer is bound and the configurations for the vertex attrib to process the vertex buffer is done, all that remains is to draw the data. This is done calling glDrawArrays with the primitive, the start position and the number of data to be processed:

```
glDrawArrays(GL_TRIANGLES, 0, 3);
```

Right after the drawing, the VertexAttribArray was already used, so, it can be disabled to release the resource for other ones:

```
glDisableVertexAttribArray(0);
```

Some questions for you to experiment:

1. If the data we are setting is a static and to draw, do I really need to configure the vertex attrib array and set the vertex attrib pointer at each loop pass?
2. If such configuration (questioned in 1) can be done outside the loop, how to proceed with disabling the vertex attrib array? Does it need to be within the loop? Or outside? If outside, where?

## Vulkan Presentation Context: Surfaces, Swap Chain and Image Views

Vulkan is platform agnostic. This means that if you want to use it to compose images offscreen (offscreen rendering), it will do fine in any operating system the Vulkan driver is implemented and installed, while OpenGL demands a window. But if you want to show images on the screen, then you will need to interface with whatever graphical and window management system your operating system uses.

In order to display the rendered image to the screen Vulkan needs a window surface, provided by the Window System Integration (WSI) extensions, an instance level layer by the name `VK_KHR_surface`. It exposes a `VkSurfaceKHR` object that represents an abstract type of surface to present rendered images to and is backed by a window that the application must provide, which is being provided via GLFW. The extension is alredy configured and being provided, since it is retrieved by the `glfwGetRequiredInstanceExtensions` instruction, which provide the other needed extension, the one related to the operating system.

The window surface influences in the physical device seletcion. This means that it needs to be created after the instance creation and before the physical device selection. It was just postponed for the sake of simplifying the explanation and understanding of the Vulkan setup process.

Window surfaces are part of a larger topic about presentation and render targets. They are optional, as mentioned in the first paragraph.

### Window surface creation

The application need to know how to map the graphics to the windows. That means, it needs the reference to the surface object, so, create one more private attribute to the WindowAppWrapper class of type `VkSurfaceKHR` and name it `surface`.

The object is platform agnostig, but its creation is not. It relies on the operating system and on the window management system (or graphics server) responsible to manage the visual elements.

For instance, on Windows the creation need two information, the window handler (HWND) and the application instance handler indicated by HMODULE. So, there is a platform specific addition to the extension. For windows it is called `VK_KHR_win32_surface`, also automatically retrieved by `glfwGetRequiredInstanceExtension` and included in the list for the instance creation configuration.

The process to create, or instantiate, surfaces is no different than the process to create the Vulkan Instance or the Logical Device instance. Declare the needed structure, fill in the fields as specified, needed or desired and call the creation instruction. For surfaces the structure must be the one for the system, so, for windows it would be like:

```C++
// The structure to be configured for surface creation on Windows
VkWin32SurfaceCreateInfoKHR createInfo = {};
createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
// Retrieve the window handler (glfw provides a ready made routine)
createInfo.hwnd = glfwGetWin32Window(window);
// Retrieve the application instance handler
createInfo.hinstance = GetModuleHandle(nullptr);
// Create a win32 surface for the instance according to createInfo.
if (vkCreateWin32SurfaceKHR(instance, createInfo, nullptr, &surface) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface!");
}
```

But, the specified code is platform specific and the goal of this whole project is to implement the code as platform agnostic as possible, that's why glfw is being used. Luckily, glfw provide a routine that return a surface, making the process much easier.

So, create a new private method called `createSurface` and call `glfwCreateWindowSurface` with the parameters `instance`, `window`, `nullptr` and `&surface`, in this order, so it can create the surface for the vulkan `instance` to be displayed in the `window`, with no custom `allocator` and store the handler at the address indicated by `&surface` to be properly retrieved in the future.

```C++
void createSurface(){
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS){
        throw std::runtime_error("Failed to create window surface!");
    }
}
```

Although surface relies on the Vulkan instance, it is not automatically destroyed when instance is, so, it must be destroyed explicitly. And since it relies on Vulkan instance, it must be destroyed previously to the instance. Therefore, add the `vkDestroySurfaceKHR` instruction to the `cleanup` code prior to the instance destruction instruction.

```C++
    vkDestroySurfaceKHR(instance, surface, nullptr);
```

### Presentation Support

The surface binds the Vulkan instance, the application (window) and the logical device, but the device must provide the data for the surface in order to the graphics appear on the screen. And this need another queue, to sent the graphics information to the screen, a queue that belongs to the presentation family.

But, what about that queue that we queried for graphics?

That is the rendering queue, to accept graphics commands?

Anyway, we will need another queue, and one that is configured for presentation. There is already a queue in the class private attributes, just add another and call it `presentQueue`.

Now, some devices does not support WSI, this is the responsibility of the presentation queue. So, there is the need to probe the device for this suitability.

In Vulkan there is the possibility of a queue family to support one, but not other competence. So, there is the possibility of the Graphics and the Presentation families does not overlap. This is why the `QueueFamilyIndices` were declared as a struct. So it can be improved.

Add one more field to the struct, the same type as the `graphicsFamily`, but name it `presentFamily`. Change the `isComplete` method so it return the conjunction of both has_value. (Conjunction is the &&, disjunction is the ||)

```C++
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};
```

Now it is time to update the `probeQueueFamilies` to probe for an index of a queue family that supports the present. 

To find the queueFamily that allows drawing commands (the graphicsFamily), the queue family queueFlags were compared with the constant defined in `VK_QUEUE_GRAPHICS_BIT`. To check for presentation capabilities is different. For that use the instruction `vkGetPhysicalDeviceSurfaceSupportKHR`, which takes the physical device, the queue family index and surface as parameters, and the last parameter is to store a boolean value that will be true if supports the presentation.




```C++
...
    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }
...
```

And now the system returns a suitable device only if the device provides both queue families: graphics and presentation, one to receive drawing commands and the other to provide the image data. It is time to setup the presentation queue.

This is no different than creating the graphics queue. Set the structure and call the command, but it can be improved.
Instead of declaring two different create info structures, use a vector of `VkDeviceQueueCreateInfo`s and a set with the family indices retrieved. This, of course, is done in the logical device creation method.

```C++
// Return the family indices
QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
// Build a set with t he family indices.
std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

// And set up a base queue priority
float queuePriority = 1.0f;
```

Now it is time to traverse the set and build each queue create info structure.
Note that since it is a set, if both indices are the same, only one structure will be created. But, the set header must be included in order to be used (```#include <set>```).

```C++
for(uint32_t queueFamily: uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
}
```

Then, in the logical device create info structure, set the number of queue info count and the queue infos data created:

```C++
createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
createInfo.pQueueCreateInfos = queueCreateInfos.data();
```

Finally, add the call to retrieve the queue handle and assign it to the class attribute created.

```C++
vkGetDeviceQueue(device, indices.presentFamily.value(),0, &presentQueue);
```

### The swapchain

Our system is now configured, with the layers and extensions desired, to be referenced and debugged (Vulkan Instance), with a suitable device (Logical Device that represents the Physical Device), that allows the application to send drawing commands (the Graphics Queue) and retrieve presentation data (Presentation Queue) to set available to a surface configured with the WSI.
The issue now is about where is this data retrieved from? Where does Vulkan properly draw?
OpenGL makes this easily, it automatically set up a framebuffer that can be single or double, but Vulkan need to be told what to do, and how to do it. The process Vulkan provides for this resource is called `swap chain` and can be understood essentially as a queue of images waiting to be presented to the screen.
The application acquire an "image" to draw to it and then return it to the queue. The queue machinations and the image presenting conditions depend on how the swap chain is configured, but its general purpose is to synchronize the process of images presentation with the screen refresh rate.

#### Swap Chain Support

As with the presentation family queue, not all graphics are capable of presenting images directly to a screen. They may be render farm devices without display output, for instance. Also, image presentation is heavily related to the specific window system and their associated surfaces, which are not responsibility of the Vulkan core. The device must provide this extension, identified as `VK_KHR_swapchain` and must be explicitly enabled. This means, the device must be queried for its support, and it must be true in order to be suitable.

There is already a method to check if the device is suitable, so, use it!

So far, only layers, the validation layers for the Vulkan instance creation, were explicitly declared to be used, yes, the debug instance extension as well. Device extensions work somewhat the same way, so create one more private attribute, a vector of null terminated strings and name it `deviceExtensions`. Initialize it with the value `"VK_KHR_swapchain"`, or with the macro `VK_KHR_SWAPCHAIN_EXTENSION_NAME`. Both are the same. So, now we have a list, with just one element, of device extensions required for the system to be configured. (This can be turned into a dynamic process, how about it?)

```C++
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
```

In order to simplify things, create one more private method and call it `checkDeviceExtensionSupport`. Pass the device as parameter and returns a boolean value. This method will be called from the `isDeviceSuitable`, it will tell if the device has the required extensions so that a proper value can be configured.

To check the extensions the process is already known. It was done with layers and instance extensions. Retrieve the extensions count as usual (it is related to the device, so, just the device and the address to store the number of extensions), retrieve the extensions, as usual, and check. But this time the process to check will be a little different.

Instead of looping and checking for the extension needed, the traversal will remove the extension from the needed extension list. If the extension to be removed is not in the needed extension list, no problem, if it is, it is removed. At the end if the needed extension list is empty, this means that the device supports all needed.

```C++
bool checkDeviceExtensionSupport() {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    // Copy the required extensions because they will be needed to be enabled later
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto* extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
```

In the logical device creation, update the createInfo structure to contain the information related to the device extensions needed:

```C++
createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
createInfo.ppEnabledExtensionNames = deviceExtension.data();
```

And now the method to detect if the device is suitable also informs if the device is capable of swapchain, with the logical device creation enabling it.

```C++
bool isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = probeueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    return indices.isComplete() && extensionsSupported;
}
```

Once the extension is verified and enabled, it is time to be configured.

### Configuring the Swapchain

Checking if the device have an extension and enabling the extension in the device is not enough for it to be properly used. It need to be configured, but before that, the extension specifications must be checked in order to know if it supports the operations or the data it will operate. This is the next step in the swap chain configuration.

There are some properties to be checked, the main three are:
* Basic surface capabilities: The minimum and maximum number of images on the swap chain and their minimum and maximum dimensions;
* Surface formats: What are the pixel formats and how they translate into bytes, what is the color space;
* Available presentation modes.

For these informations, considering these three, let's use a structure to store the possible values.
The structure will hold the capabilities, which is already represented by a Vulkan structure, a vector of formats, each is a Vulkan structure, and a vector of present modes, that are also Vulkan structure.

```C++
typedef struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
```

The swapchain details are retrieved from a device, each device have their own set of support. This means that the method to retrieve the swapchain details must know to which device it is probing. They are also related to surface objects, but this is already provided by the class.

The first needed instruction is to retrieve the surface capabilities of the device.

    If you've paid enough attention you should have realized that Vulkan has a naming convention. Instructions always start with `vk`, followed by a command `Get`, `Create`, `Enumerate`, and such. Then there is the subject `PyshicalDevice`, `Instance`, `Device`, `Queue`, `PyshicalDeviceQueueFamily` (this is a composed subject). If it is a core, non implicit, instruction, it shall have the company identification, like `KHR`.

So, the routine needed is `vkGetPhysicalDeviceSurfaceCapabilitiesKHR`, which demands the physical device and the surface to inform the capabilities.

With the surface capabilities (TODO: What are surface capabilities) now it is time to check out the formats.
Formats are stored in the VkSurfaceFormatKHR vulkan structure and the device surface can provide several formats. This means that to probe we need to count how many formats are available and then retrieve them. This is done considering the same procedure already performed for validation layers and extensions, this time using the instruction `vkGetPhysicalDeviceSurfaceFormatsKHR` which demands the physical device handler, the surface handler, the address to store  the number of formats available (or to define a number of formats to retrieve), and where to store the formats retrieved (if null, provides the number of formats available).

Finally, the presentation modes process is identical, only using the instruction `vkGetPhysicalDeviceSurfacePresentModesKHR`.

```C++
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}
```

With all these instructions, it is time to extend the `isDeviceSuitable` method to verify the adequacy of swap chain support. For now it is adequate if there is at least one supported image format and one supported presentation mode for the given window surface. That means, if nor the support nor the presentation modes are empty.

```C++
bool isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = probeueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}
```

### Adjusting the swap chain settings

Right now the device and surface were probed for information and the capabilities, formats and presentation modes are available to be consulted and configured.
The subject of graphics properties are extensive, there is the issue of the color depth (how many colors are supported), how the color is represented, how images on screen are swapped (let's say... Double Buffering, like the basic OpenGL?), images resolutions and more. 
Some of these features are limited by how others are configured, and the size of the storage space as well.
Let's determine the right settings for the revelant fields.

#### Surface Format

Each format data, as represented by the VkSurfaceFormatKHR, contains a (pixel)`format` and `colorSpace` member. The `format` specifies the number of pixels for each color chanel and their types. For example, `VK_FORMAT_B8G8R8A8_UNORM` means that the pixel is represented by 32 bits, 8 unsigned bits per color in the following order: Blue, Green, Red, and Alpha. The `colorSpace` indicates if the desired color space (SRGB, in this case) is supported or not using the flag `VK_COLOR_SPACE_SRGB_NONLINEAR_KHR`.
(TODO: Can the colorspace anb format be enumerated?)

For color space and pixel format let's use the SRGB and VK_FORMAT_B8G8R8A8_UNORM, respectively:

```C++
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    // If not found... let's resign and return the first format

    return availableFormats[0];
}
```

#### Presentation Mode

Representing the actual conditions for showing images to the screen, the presentation mode is the most important setting of the swap chain and provides four possible modes:
* VK_PRESENT_MODE_IMMEDIATE_KHR: Images submitted by the application are transferred to the screen right away. Think of this as a kind of single buffer. Go on, research the troubles of single buffer graphics!
* VK_PRESENT_MODE_FIFO_KHR: The swap chain is a queue. The display takes the image from the front of the queue whan the display is refreshed and the program inserts the rendered images at the back of the queue. This mimics the vertical sync, that is the moment where the double buffer swaps... Oh! Swap chain!...
* VK_PRESENT_MODE_FIFO_RELAXED_KHR: Quite similar to the previous, but if the queue is empty when the vertical blank is triggered, instead of waiting for the next vertical blank it transfers the image immediately, which can cause some glitches;
* VK_PRESENT_MODE_MAILBOX_KHR: Another variation of the FIFO, but instead of locking the application to the queue, it replaces the last image in the queue by the one provided by the application.

If a device with presentation mode is retrieved, then it is guaranteed that VK_PRESENT_MODE_FIFO_KHR is present.

```C++
VkPresentationKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){
    return VK_PRESENT_MODE_FIFO_KHR;
}
```

But, the third mode allow for the graphics presentation to be as close as possible to the graphics processing without glitches, so, if present, let's use it!

```C++
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}
```

And it will return the best option. Remaining only the last major property.

#### Swap Extent

Swap extent represents the resolution of the swap chain images and it is almost always exactly equal to the resolution of the window to be drawed. The ranges of the possible resolutions are defined by the capabilities structure VkSurfaceCapabilitiesKHR.
Although Vulkan advises to match the resolution of the window by setting the width and height in the `currentExtent` member, some window managers allow applications to differ by setting the width and height in the `currentExtent` to a special value, the maximum value of `uint32_t`, defined in the header `cstdint`. In that case, let's pick up the best resolution within the `minImageExtent` and `maxImageExtent` bounds.

```C++
...
#include <cstdint>
#include <algorithm>

...

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = {win_width, win_height};

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}
```

The instructions `std::max` and `std::min` provided in the header `algorithm` are used to clamp the values to the maximum and minimum allowed.

### Creating the Swap Chain

With the helper functions ready to assist in the choices to make at runtime, all the information needed can be retrieved to properly create the swap chain. And it will be another private method, which will be named `createSwapChain`, to configure the fields with the appropriated values and create the apropriate object to be used. Call it within `initVulkan` right after the logical device creation.

```C++
void initVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    setPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
}
```

And code it following procedure of querying the swap chain support in the physical device to retrieve the ShapChainSupportDetails object structure to be processed. Use it to retrieve the proper surface format, the presentation mode and the extent.
Then, configure how many images to have in the swapchain. The device implementation specifies the minimum number required to function and stores it in the `minImageCount` field of the capabilities field at the swap chain support retrieved, use it incremented by 1 just to be sure that if the minimum is one, we will have at least the possibility of a double buffer... Oh yeah! We are not done yet!

But, if the minimum is also the maximum, the configuration would incurr in error, so, just make sure that it does not exceeds the maximum number of images. If it does, clamp to the maximum. Oh, it can be retrieved in the maxImageCount of the swap chain capabilites.

And now, with all information set, it is time to create the swap chain. Creation process is standardized. Declare the structure, fill in the fields and use it in the specific create instruction.
The fields to be filled are defined in the `VkSwapChainCreateInfoKHR` structure and the type is advised to be `VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR`. (If you still haven't realised a pattern between the structure name and the value at the `sType` field, try to do it now!)
Provide the `surface`, the `minImageCount`, the `imageFormat` and the `colorSpace`, what is the `extent`, how many `imageArrayLayers` (let's stick with 1 for now) and the `imageUsage` (keep with `VK_IMAGE_COLOR_ATTACHMENT_BIT` for now) (TODO: Hey! What are these two last fields for? What are their possible values and their outcomes?)

The field `imageArrayLayers` specifies the amount of layers each image consists of. For usual desktop application this should be always 1. For stereoscopic 3D applications it needs to be bigger. Hey! It relates to the stereo buffer in OpenGL?
The field `imageUsage` specifies the operations that will be used in the images in this swap chain. Right now, it is direct drawing, that is the meaning of `VK_IMAGE_COLOR_ATTACHMENT_BIT`, but it can be used for other purposes, like rendering images for post-processing (VK_IMAGE_USAGE_TRANSFER_DST_BIT) and probably depth testing (Hey! I've heard things may happen this way in Vulkan, after all, an image is a buffer and in OpenGL it is a depth buffer!)

```C++
void createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    // Select the best format (as configured) from the ones available
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.Formats);
    // Select the best present modes (as configured) from the ones available
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    // Select the most apropriated extent (as configured)
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    // Retrieve the minimum image count and increase by 1. Let's guarantee at least the double buffer
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    // Max image is greater than 0, but the image count is greatet than max image. Clamp to max image
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount){
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

    // Proceed to filling the structure
    VkSwapChainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface; // surface is retrieved from the WindowAppWrapper class (for the moment)
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // continue
}
```

We are dealing with queues to send drawing command and retrieving "pixel data" (for lack of a better definition). One is defined by queues from the graphics family, the other by queues from the presentation family. Some devices provide family queues with more than one feature and they can be used in an exclusive mode, that means, one queue fits all (for now), but some may provide family queues with restrictions, for instance, family with graphics features not providing presentation features, so, their indices will differ, the swap chain need to know how to handle this.

To handle this conundrum, because we are dealing with things at runtime (yes, it can be configured in an external configuration file too!), let's treat this retrieving the QueueFamilyIndices by probing the queue families and checking if the indices are the same. 
If they are the same just set the `imageSharingMode` field of the structure to `VK_SHARING_MODE_EXCLUSIVE`, but if they differ, this means that the swap chain uses two indices for queue family, they are concurrent.
Set the `imageSharingMode` to `VK_SHARING_MODE_CONCURRENT`, the `queueFamilyIndexCount` to the number of indices (2 in this case) and the pQueueFamilyIndices to an array containing the queue family indices.

```C++
    // continued
    QueueFamilyIndices indices = probeQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily == indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    // continue
```

Finally, finish setting the present mode, the `preTransform` field with the `currentTransform` field value at `capabilites`, the `compositeAlpha` field with `VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR`, the `clipped` field with `VK_TRUE` and the `oldSwapChain` with `VK_NULLK_HANDLE`.
* preTransform allows transformations to be applied to the swap chain if supported. Like rotating some degrees or scaling;
* compositeAlpha specifies how the alpha channel should be used. In this case, it is ignored;
* clipped will ignore pixels obscured (by other windows). This can be set to true unless the value of that pixel (in the resulting image) is important somehow. This provides a minor performance gain.
* oldSwapChain must store references to invalidated or corrupted swap chains. This can happen, for instance, when a window is resized. This is a topic to be discussed in future developments of this document.

With the structure filled, use the instruction `vkCreateSwapchainKHR` specifying which device will be used, the structure info address, the allocator pointer (null for the moment) and the address to store the swap chain, which is the private attribute still to be created for this task, so go on and put it next to the devices and queues. 

```C++
VkSwapchainKHR swapChain; // This is a class private attribute
```

If the create instruction does not return VK_SUCCESS, throw an exception.

```C++
    // continued
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    // Continue
```

The swapChain must be cleaned up explicitly using the instruction vkDestroySwapChainKHR that receives the device that the swap chain belongs, the swap chain handler and a third parameter that will be a null pointer for now. Since it uses the device, it must be destroyed before the device is destroyed.

Now that we have the swap chain configured and created, it is time to retrieve the swap chain images.

#### Swap Chain Images

With the swap chain created, it is time to retrieve the handles for the images it provides. Since those images are referenced during rendering operations, it is pertinent to store their handles as private attributes:

```C++
std::vector<VkImage> swapChainImages;
```

The images are created within the swap chain creation and will be automatically cleaned up once the swap chain is destroyed, so, no cleanup code needed.
Since only a minimum number of images is provided in the swap chain creation, the system may instantiate more than that. This means that the actual number of images is unknown and must be retrieved via instruction `vkGetSwapChainImagesKHR`, also used to retrieve the images themselves. The procedure to retrieve those data are the standard Vulkan procedure for probing and retrieving. The instructions will be added just after the swap chain is created:

```C++
    // Continued
    vkGetSwapChainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapChainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
```

And to end the swap chain and image creations, store the `format` and `extent` in private attributes, they will be needed in the future. The recently built method uses the `format` and `extent`, so, just assign them to the attributes:

```C++
    VkFormat swapChainFormat;
    VkExtent2D swapChainExtent;
```

Now the system is configured with a set of images to be drawn to and retrieve for presentation, with a method of swaping them to avoid visual glitches, but they need to be set up as render targets to the graphics pipeline!

#### Image Views and Render Targets

Images, as well as buffers, specify a contiguous area in memory. Buffers specify only one dimensional data, Images can be 1D, 2D or 3D, but they are just that. How many bytes and the basic organization. In order to properly use them, views are necessary.
Views works as contexts definitions, they are like interfaces that specifies what the data mean, for what purpose it is used and how to manipulate them. For example, the image can be treated as a 2D texture depth texture without mipmapping or as the final render combination.

For each image, if they are to be accessed, an image view need to be created. And since it defines the interface for the image manipulation, they need to be accessible throughout the code. This means, they need to be declared in the class level. Since they must not be available outside the class, they must be private (or protected, at least).

```C++
std::vector<VkImageView> swapChainImageViews;
```

Then, let's code the method to create the image views. As well as the image views themselves, the method context is just the Vulkan configuration class (that is also our windowWrapper class, for the moment), so, a private method that will be called after the swap chain creation, in the initVulkan method. Let's call it `createImageViews`, with no return (void).

Initially, resize the `swapChainImageViews` to match the size of `swapChainImages`. Each entry will be processed, creating a ImageView and assigning them to the entry. And for Vulkan object creation, the procedure is the standard one, declare the structure, fill its fields and call the instruction to create the desired object using the filled structure with the configuration set. If one image view fails, let's halt the whole process.
Each iteration it creates a new image view, assigning to it the related image (indexed related), the view type as image 2D, the format (and that's why it was told to store the format), how the color components are mapped, and the subresource range.
The color components mapping setting in the code indicates the default mapping, other options are needed if you want a monochrome texture, for example. TODO: More exploration is required to identify the possibilities.
Subresource range describe the image purpose and which part of the image should be accessed. The images needed, for the purpose of drawing a triangle, will be used as color targets (VK_IMAGE_ASPECT_COLOR_BIT), without mipmapping levels or multiple layers (we are not working with stereographic 3D projection, that is why only 1 array layer is enough, otherwise it would need to be at least 2).

```C++
void createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
}
```

An interesting aspect about image views is that multiple image views can be created for a single image. With this practice an image view can be created to represent the left view and another represent the right view, but related to the same image.

Unlike images, the image views were explicitly created. They need to be explicitly destroyed with the instruction `vkDestroyImageView`, indicating the device handler and the image view handler. 

```C++
    for(auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }
```

We are nearly set to start drawing. The image and image views are ready to be used, but only to retrieve data for the moment. This means they are not ready to be drawn, to be used as a render target. They need to be configured as framebuffers, which is done in the graphics pipeline.
This means that, for the moment, the basic Vulkan configuration is done.

Next: 
    OpenGL Coloring the plain triangle Immediate Mode
    Vulkan The graphics pipeline
