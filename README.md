# ComputerGraphics_Studies
- Systems Covered (So Far): OpenGL, ~~Vulkan~~
________________________________________________________________________________
## [Initial Setup](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/master/README.md)

________________________________________________________________________________
## [Initial Configuration: Application Framework and CMake Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_CMake_Project_Create/README.md)

________________________________________________________________________________
## [CMake Specific Configuration, Project Folder Structure and Code](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_a_Project_Structure_And_Setup/README.md)

________________________________________________________________________________
## [Finishing the Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_b_app_framework/README.md)

________________________________________________________________________________
## App Framework Extras

There are a couple of things to discuss about the application framework that we will improve during this process.
The first is the clear color. This is a very easy task to do and will be discussed here.
Then there is improvements to CMakeFiles in order to make our lives easier. Since CMake is fundamental, here this will be presented as well.
Finally there is the issue about the class that represents the app. It uses GLFW, but it is also calling OpenGL commands (I'm talking about you, glClear... and the command to set the clear color that will be presented here as well!).
The AppFramework class should not know which technology it relates to. It should only provide the context to the techonogy so it can use its canvas. This will be done in the next step, when the Vulkan technology will start to be discussed.
So, without further ado...

________________________________________________________________________________
### Cmake Improvements

This series (the set of texts in the ReadMe along with code within all branches, let's call it series), in the end, will have a lot of projects.
There are 20 projects planned for OpenGL. I'm not sure how much for Vulkan yet.
And, there are at least 4 lines at CMakeLists that configure the libs for each project. This will provide us with 80 lines, at least, just for the OpenGL code!
Unfortunately linux systems demand the explicit instruction to include the OpenGL library to the project, but Windows do not.
Even worse, Windows complain when we do it! That's why there is a `if` clause for it.
So, at least 20 lines of instruction to include OpenGL we will have, but the others can be cut to just one line, writing each lib right next to the previous, with spaces between them, and the three lines: 

```
target_link_libraries(00_OpenGL_app_framework PRIVATE GLEW::GLEW)
target_link_libraries(00_OpenGL_app_framework PRIVATE glfw)
target_link_libraries(00_OpenGL_app_framework PRIVATE glm)
```

Becomes one:

```
target_link_libraries(00_OpenGL_app_framework PRIVATE GLEW::GLEW glfw glm)
```

Just make sure that this is the order of setting:
* first OpenGL, in the separate instruction within the if statement;
* then the other libraries starting with GLEW.

And the (at least) 80 lines of library configurations for the linker, in the end of this series, will become 20 for the OpenGL, one for each project, and 20 for the other libraries, also one for each project, reducing to 40.

________________________________________________________________________________
### Clearing to Other than Black and a bit of State Machine

Ok, I have no idea what fuchsia is. I know it is a color, but no idea which one. So, I will clear to cyan.
Putting it simple, OpenGL is a state machine that operates on vertex, colors and texture coordinates. It is a little more complex than that, but this is the basis to get started well with OpenGL concepts.
This means that you configure the OpenGL state, send in the vertexes, colors, texture coordinates and the texture, set what you want to draw and tell it to do it.
The color to clear the screen is no different. It is just one command that will set it to the configuration until it is changed (usually it is not!)
It is advisable to adjust basic configurations outside the main loop. Setting it within the main loop is waste of resources. No one likes to be remembered about how to do one action every time she needs to do the action! So, right before the `do` statement, within the `mainLoop` method, all you need to do is write the instruction:

```
glClearColor(0.0f,1.0f,1.0f, 1.0f);
```

This instruction tells the OpenGL to configure the color to clear the screen considering no `red` component, full `green` component, full `blue` component and full `alpha` component.
`Alpha` states for opacity, so 1.0 means full opacity.
The color is constructed combining the red, green and blue components. In this example the combination of green and blue, with absence of red, produces cyan.
This color triplet is usual among several software, from OpenGL to painting, illustration or design softwares.
So, if you know the triplet combination to the color fuchsia, you can compute how much of each color component is needed and fill it yourself, configuring your screen to clear to fuchsia.

______________________________________________________________________________________________

## Vulkan

Before start writing graphics instructions and watching their results appear on screen, some more actions need to be taken.
The first feature that will be discussed in this document is an optional one, so you can skip it if you do not want to enable debug callbacks.
Debug Callbacks are important to catch errors or warnings as they occurr, since Vulkan does not provide a way to catch and treat them, as OpenGL does, so they need to be implemented by the application developer and deal with the errors as intended by the application context.
The next step is to deal with the hardware device. What have done so far is the creation of an instance for the Vulkan API, which is responsible to communicate with the Vulkan capable devices and the configuration of such instance with the Windows Interface System (WSI), retrieved by using the GLFW routines.

### Debugging and Message Callback

Unlike OpenGL, Vulkan does not caught and treat errors, it does not implement a mechanism of silent failure, where the error is registered but the overall process keep working. Your application may pass through or fail miserably if not properly engineered.
In order to offer the engineer or developer the hability to check for errors, some layers are usually available to catch errors or informations within runtime and provide means for the error to be circumvented or the application state probed for debugging purposes. This is done via layers.
Layers were briefly introduced in the previous section. Within instance creation the validation layer availability was probed and the layer name was set up within the createInfo structute telling it that it is supposed to be used.
However, it was not properly configured.
To configure a layer the instance must be created with its extension in its extension list and the instance created in the previous section contains only the surface extensions retrieved by GLFW helper routine.
In this situation, let's create a routine to provide us with the extensions to be configured, considering, inclusive, the validation layer extension and debug extensions.
As an example, my own extensions and layer (with their extensions) is included for reference. If a layer has no extensions, what happens? Is it implicit?
Last section discussed how to retrieve layers and how to retrieve extensions... as well as layer specific extensions, feel free to look at the code that generated my list, it will not be discussed here.

So, the routine to retrieve the extensions shall be called `getRequiredExtensions`, and returns a vector of strings (const char*). As usual, let's create it as a private method of the class WindowAppWrapper, providing the code:

```C++
std::vector<const char*> getRequiredExtensions(){
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    // Retrieve the required surface extensions and the extensioncount
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    // Instantiate and initialize the return variable
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if(enableValidationLayers){
        // Only include the debug extension when debugging!
        extensions.push_back("VK_EXT_debug_utils");
    }

    return extensions;
}
```

This code can immediately replace the extensions retrieved in the source, between the end of the `appInfo` setup and the start of the `createInfo` declaration, providing the code:

```C++
// Retrieve the required extensions. Auto alloc the type
auto extensions = getRequiredExtensions();
```

And replacing the related assignments to `createInfo` with:

```C++
// Assign the number of extensions
createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
// Assign the extensions names
createInfo.ppEnabledExtensionNames = extensions.data();
```

You can put them together, right after the `createInfo` declaration if you want.

Compile and run to check if you don't receive a `VK_ERROR_EXTENSION_NOT_PRESENT` error. This extension is implied by the availability of the validation layers, so, it is already being checked.

The extensions configuration is coded, now it is time to create the debug callback function.
A debug callback function must be a static function. It still will be put it in the class as a private method (easy software engineers, later the arrangements will be done!) and it need to have a specific prototype, the `PFN_vkDebugUtilsMessengerCallbackEXT` prototype.
This prototype demands the following parameters:
- messageSeverity: Specifies the [VkDebugUtilsMessageSeverityFlagBitsEXT](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDebugUtilsMessageSeverityFlagBitsEXT.html) that triggered the callback. It is like the logging severity (in python logging, for instance.). This parameter have the property of comparison, so, it can be checked to identify the level of severity;
- messageTypes: A bitmask of [VkDebugUtilsMessageTypeFlagBitsEXT](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDebugUtilsMessageTypeFlagBitsEXT.html) specifying the kind (or context) of event that triggered the callback. This identifies if the error is unrelated to Vulkan (in computing algorithm, maybe!), if it violates the spec or if it indicates potential non optimal usage;
- pCallbackData: contains all the callback related data composed of:
    - pMessage: The debug message as a null terminated string;
    - pObjects: Array of Vulkan object handles related to the message;
    - objectCount: Number of objects in the array
- pUserData: is the user data provided when the VkDebugUtilsMessengerEXT was created.

The only thing coded in the callback will be the output to standard error stream and the return of VK_FALSE, so:

```C++
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
```

VKAPI_ATTR and VKAPI_CALL are modifiers that ensure the function has the right signature for Vulkan to call it. VkBool32 is the return type and indicate if the Vulkan Call that triggered it should be aborted (VK_TRUE) and issues the VK_ERROR_VALIDATION_FAILED_EXT error. For now, just return VK_FALSE.

This callback need to be set up in Vulkan, otherwise it would never be called.
As much everything in Vulkan, callbacks are managed with a handle that needs to be created and destroyed explicitly. They are part of a _debug messengeg_ and as many as needed can be created. They are stored in an identifier of type `vkDebugUtilsMessengerEXT`, so, one more private attribute for the `WindowAppWrapper` class:

```C++
VkDebugUtilsMessengerEXT debugMessenger;
```

And it needs to be setup. Two methods, private, of course, shall be created, one to properly setup and other to configure the structure.
The structure configuration method will receive a reference for a `VkDebugUtilsMessengerCreateInfoEXT` variable and set up the proper values. These values will be fixed for now. Note that the callback is set up in the last instruction.


```C++
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }
```

For the setup, if the system is not in debug mode, it will not be set up, otherwise, it instantiate and populate the information for the callback creation.
Finally, create the binding with `CreateDebugUtilsMessengerEXT` passing to which Vulkan instance the debug is attached, the debug info, the allocation callbacks (null for the moment) and the debug messenger address so it be assigned. This instruction returns VK_SUCCESS or error and it is pertinent to check if everything worked accordingly.

```C++
    void setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }
```

Call the debugger setup within the `initVulkan` method. You can compile and run the example for the moment.
Be sure to have called cleanup in the method `run` (after the `mainLoop`). And call the executable via terminal. Nothing happens. No big deal, we have created nothing except the instance to communicate with Vulkan, so, close the windows...
...
And witness some messages.
...

Those error messages occurs because the `VkDebugUtilsMessengerEXT` object is not cleaned up. A call to `vkDestroyDebugUtilsMessengerExt` is needed befor destrying Vulkan instance in order to properly close the system, and the function needs to be explicitly loaded, as it was done with `vkCreateDebugUtilsMessengerEXT`.

```C++
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}
```

Now just update the cleanup method and all is set:

```C++
void cleanup() {
    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}
```

And the setup is done. Compile and execute again to validate. Upon closing the application those error messages no longer appear.

#### Debugging Instance Creation and Destruction

If you followed up everything up to this point you should have the debugging callback ready for almost anything.
That is because to create and bind the callback a valid Vulkan instance is needed, and to destroy the callback also. With this process there is no way to debug the Vulkan instance creation or deletion.
That is the reason the `VkInstanceCreateInfo` structure, the structure set (or configured) to define the instance to be created has a field that stores a pointer to a `VkDebugUtilsMessengerCreateInfoEXT` and it is named pNext.
Let's use the same callback configured previously, so, the basic procedures are:
* Define the identifier to store the VkDebugUtilsMessengerCreateInfoEXT data;
* Populate the identifier with the DebugMessageCreateInfo (there is a method ready for that);
* Assign the address of the identifier to the createInfo structure field.

Of course, all this is the validation layers are enabled. Look out for the excerpt that assign the layer count and names to createInfo in the method `createInstance` and perform the required changes:

```C++
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }
```

And we are done for Basic Window, Layers and Extensions probing and enabling, Instance Creation and Validation Layer (basic/default) Debug Configuration, for now.
There are many more settings for validation layers. The ones we specified are just an example and it is advisable (TODO:) to check out validation layers settings by checking out documentation and the vk_layer_settings file at Vulkan_SDK/config directory.
Now, we need to check out properly what cards and what features they provide in order to start drawing.

Next: 
    OpenGL: Let's put some graphics on the screen!
    Vulkan: Probing, Defining and configuring the Device for Graphics.

