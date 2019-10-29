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

## Vulkan Code for App Framework

For OpenGL we now know how to create a window, set its context to OpenGL and clear the screen, but what about for Vulkan?
Vulkan is a recent development in computer graphics APIs that provides more ownership to the management processes to the developers.

But, this series is about coding, so, how to code for Vulkan?

### The CMake Project

You already know how to create a project (executable) for OpenGL, let's do it for Vulkan.

First, let's create the Vulkan folder, the 00_app_framework in it and put up an empty main.cpp to start.

Now it is time to configure the project in CMake. You know: add_executable and add_target_library. Let's name the project `00_Vulkan_app_framework`, pointing the file to `Vulkan/00_app_framework/main.cpp`.

### Vulkan API

Vulkan is a recent development in computer graphics APIs, in a sense. Vulkan, in fact, is more close to a Graphics Card Driver API than to a Graphics API, meaning, it will provide the developer with the routines to access the hardware capabilities, giving the developer the freedom to use it as desired, within the hardware constraints.
Unlike OpenGL, or as the Vulkan documentation states, a set of initializations, validations and verifications are not handled by the API, they must be handled via application, according to the availability of the features provided by the graphic cards hardware implementation.

So far, for OpenGL all the resources, the window, the context configuration and the openGL context, were set up in the initSystem method. For Vulkan let's create another method for now named `initVulkan` in the private section and leave it empty for now.

#### Initial Configurations

In initSystem we need to remove all OpenGL related instructions, so, all `glfwWindoHint*` and the `glew` instructions need to be removed. The `glfwMakeContextCurrent` also needs to be removed, the context will be created elsewhere.

In the place of the `glfwWindowHint` instructions, only one instruction is needed: 

```C++
glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
```

But, since we are still to operate on graphics, let's just set the window to be of specific dimensions.

```C++
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
```

And that is it for the intitSystem method. Remove the former OpenGL window configurations, the (OpenGL) context setting and the former GLEW initializations and add the configuration indicating no api.
By now you should be aware that the glfwWindowHint instruction configures the window to be created, glfwMakeContextCurrent will expose the window drawing surface to the OpenGL API, but in this context, it is Vulkan, instead of OpenGL API that is being used and glfw provides only an easy context binding for OpenGL because OpenGL API provides all the necessary initializations for this to happen. In Vulkan this must be properly configured and requested to the Vulkan API (the driver).

Now that the window configuration is set, it is time to configure the driver and bind the graphics surface to the window surface.

#### Vulkan Configurations

Vulkan has a set of configuration stages in order to initialize the graphics context. Those stages are needed in order to probe, check and set the desired features or capabilities available in the graphics cards. They resume to:

1. Enumerate Instance Layer Properties: although the instance is not created, the available layers provided by the card can be enumerated so their extensions can be retrieved. So far, only one layer will be checked: the validation layer;

2. Create and set up the application info: it is usefud to identify the application in specific scenarios and identify the Vulkan Version used for the driver. Other uses may apply. Are we talking about debugging and profiling?... probably descentralized, distributed applications? This demands more investigation;

3. Create and set up the structure that holds the information to create the instance: This structure is the one passed to the instruction responsible to create the graphics driver instance and need to be set up properly. The following information must be configured in them;

    - Configure the extensions and their quantity to interface with the windows system: in order to bind the driver instance and the application, the proper extensions need to be specified so the window and the driver can exchange information. GLFW provides instructions to retrieve such extensions;
    - Enable the Layers, and their quantity: as mentioned, in this example only a validation layer, responsible to provide debug information, will be used;
4. Create the instance: Effectively create the driver instance to communicate with the device(s);

So far, this example, for Vulkan, is complete. But since all we made was create an instance to the driver, there are more to be done, like create the handler and initialize the physical device, that means, the graphics card itself. Let's to the code.

First, let's create the vulkan instance private attibute, the list of layers to be enabled and whether they will be enabled.
Since only the validation layer is to be enabled, and it is mainly used to verify or validate queried information, it will be enabled only on debugging:

```C++
private:
    (...)
        VkInstance instance; // Stores the Vulkan Instance

        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

```

Since the validation layer is being queried, if the driver does not support it, it should throw an error. A method to check if the validation layer is available is needed. So, let's create a private method to return a boolean and call it `checkValidationLayerSupport`.
The method will probe two things:
* The number of available layers;
* The list of available layers.

The list of available layers will be traversed to check if the validation layers desired are present. If one is not, it will return false.
So far, only one validation layer was defined. Check the private attributes.

Vulkan driver engineers devised a clever and consistent way of probing quantities and values within the same contextual instruction.
For the layer support, the interest is to retrieve and check out the instance layer properties enumeration.
This is performed by the instruction vkEnumerateInstanceLayerProperties, which retrieves as parameters:
* Address to store the number of layers present on the system or that stores the number of layers present on the system to be retrieved;
* Array of type VkLayerProperties of size, at least, of the number queried, to store the layer properties in the system.

Basically the most enumerations routines in Vulkan works in similar way, some have more parameters, but these two parameters, the count and the list of enumeration, are present.

The inner workings of these kind of routines are the following:
* The address to the number of enumerations to be count or retrieved is mandatory.
* If the address for the array of names is null, the system will store the number of enumerations in the count address, but if the array of names is not null, the system will store the number of enumeration values retrieved in the array of names.
And the code for the verification of the validation layer is:

```C++
bool checkValidationLayerSupport(){
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers){
        bool layerFound = false;

        for(const auto& layerProperties : availableLayers){
            if(strcmp(layerName, layerProperties.layerName)==0){
                layerFound = true;
                break;
            }
        }
        if(!layerFound){
            return false;
        }
    }
    return true;
}
```

As mentioned, for vulkan to be initiated, it is important to create an instance to the vulkan drive handler. This is done in the private method `createInstance` which will return an integer value indicating success or failure, but also can throw an error if the validation layer is not available.

Since we are requesting the validation layer, it is pertinent to verify it at the begining of the instance creation.
In fact, each and every resource properly needed is important to be verified right in the beginning of the routines for instance or device. So far only the validation layers are demanded, so we check if they are enabled and their validation.
With a negative result the system throw a `runtime_error` exception, meaning a resource requested is not available. The positive result continues to the code:

```C++
if (enableValidationLayers && !checkValidationLayerSupport()){
    throw std::runtime_error("validation layers requested, but not available!");
}
```

As previously argued, there are several data to provide to the instance creation structure in order to properly create the Vulkan instance.

The application Infor (`VkApplicationInfo`) is a structure that describes application specific information. Some of them used in the instance creation.
In short:

```C++
// Structure to Identify the Application
VkApplicationInfo appInfo = {};
appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
appInfo.pApplicationName = "Vulkan App Framework";
appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
appInfo.pEngineName = "No Engine";
appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
appInfo.apiVersion = VK_API_VERSION_1_0;
```

* `sType`: stores the structure type;
* `pApplicationName` is a const char* that simply describes the application;
* `applicationVersions`: is an uint containing the developer supplied version of the application;
* `pEngineName`: is a const char* of the engine (if any) used to create the application;
* `engineVersion`: is an uint with the developer provided version of the engine used to create the application (or provide the applciation verion if no engine is supplied);
* `apiVersion`: Must be the higher version of Vulkan that the application is designed to use. If an application is marked to Vulkan 1.1, it cannot be executed in systems or devices that only implement Vulkan 1.0.

The last item is the only really important issue in the list. It tells the driver which version of Vulkan was the application destined to use. This must be set by the developer. The developer must keep track of which Vulkan feature is to be used in the application in order to properly adjust this value.

The next stage in the instance creation is the set up of the layers and extensions.
So far the only two extensions needed are `VK_KHR_surface` and one of the following: `VK_KHR_win32_surface`(for windows), `VK_KHR_xlib_surface`, `VK_KHR_xcb_surface` or `VK_KHR_wayland_surface` (linux), `VK_KHR_android_surface` (android), ? (mac) (this ones needs MoltenVK, an implementation ov Vulkan on top of Metal. TODO: To be investigated)

VK_KHR_surface is a generic surface to draw elements. It does not guarantee the mapping to the graphical window context, so, you will be able to draw, and maybe save the result, but not show on screen (TODO: is this right?). That's why you need the other extension, to set the window context, map what is drawn and show it in the screen.

The question is: How to know whish we have?
Luckily, GLFW implements instructions so we can retrieve these informations.
Recollecting how to retrieve the LayerProperties, let's proceed with the extensions. We need a 32 bit unsigned integer to count the extensions and an array of strings (ctype) to get the extensions.

```C++
// Number of extensions
uint32_t glfwExtensionCount = 0;
// Array of extension names
const char** glfwExtensions;
```

Then we retrieve how many extensions using the `glfwGetRequeredInstanceExtensions`, passing as argument... just the address of the variable that counts it, but, it returns data! So, it goes to the array of strings.
Yes, this is not a typical Vulkan instruction, because it is not a Vulkan instruction, it is a GLFW instruction. Here is the result:

```C++
glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
```

To continue we need to check if there are extensions, at least the VK_KHR_surface must be present, so let's just use the number of extensions to chack it out:

```C++
if(glfwExtensionCount ==0) return EXIT_FAILURE;
```

If you want you can loop through the array of strings and print out the name of the extensions, just to check out.

Now that we got the extensions, it is time to set them in the structure to create the instance. For that, we need the structure.

The structure to create the instance is instanced with the type `VkInstanceCreateInfo` and have the following fields, for now:

* sType: The identification of the structure type;
* pApplicationInfo: The application information structure as was discussed;
* enabledLayerCount: How many layers are enabled;
* ppEnabledLayerNames: The layers enabled;
* enabledExtensionCount: How many extensions are enabled;
* ppEnabledExtensionNames: The enabled extentions;

The extensions were just processed and are ready to be set into the structure to be created.
So are the enabled layers, or should it be said, the enabled layer, since only one was configured, is also defined.
What remain is instantiating the instance creation information structure and set it up:

```C++
            // Required Structure to Create an Instance
            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            // Adjust the data in the create structure
            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;
```

And for the layer, the process is configured for the layer desired to be included in debug mode only:

```C++
if (enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
} else {
    createInfo.enabledLayerCount = 0;
}
```

And that's it, the instance can be created:

```C++
VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
```

Remembering that the variable `instance` is a private attribute of the AppWindowWrapper

Once the instance is created, let's probe the instance available extensions. And the instance layers as well, since we just checked for the desired validation layer!

The extensions are retrieved in the similar manner as the layer properties, with one difference, they can be retrieved for the Vulkan implementation (and implicit layers) or for a specific layer, so, one more parameter is present in the probing instruction `vkEnumerateInstanceExtensionProperties`.
The first parameter, if a null pointer, will proceed to the vulkan implementation probe, a C string, will probe the related layer.
The second parameter is the extensions count, and the last is the array of VkExtentionProperties.

```C++
            // Stores how many extensions are avaible in the instance for the basic Vulkan implementation
            uint32_t extensionCount=0;
            // retrieves the extension count
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

            // Stores the extension details
            std::vector<VkExtensionProperties> extensions(extensionCount);
            // Query the extension details for the basic Vulkan implementation (nullptr)
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            std::cout << "available extensions: "<<std::endl;

            for(const auto& extension : extensions){
                std::cout << "\n" << extension.extensionName << std::endl;
            }
```

For the layer properties the code was discussed in the checkValidationLayer method:

```C++
            uint32_t layerCount;

            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            std::cout << "available layer properties: "<<std::endl;

            for(const auto& layer : availableLayers){
                std::cout << "\n" << layer.layerName << std::endl;
                std::cout << "----------------------------------------------------------"<<std::endl;
                std::cout << layer.description<<std::endl;
                std::cout << "=========================================================="<<std::endl;
            }
```

In the extensions retrieval, to retrieve the extensions for a layer, the first parameter must be the `layerName`, so you can proceed to list all layers extensions.

With the instance created and the concept of extension and layers at least known, some things are still needed in order to setup Vulkan to process graphics.

Device enumeration, selection, set up and logical configuration is needed, and this takes some necessary steps like requiring or defining queues, memory and properties. 

This will be discussed in the next section.

For now, lets wrap up our code for compiling and window generation.
In the initVulkan private method the process is simple. First, create the instance. If successful, return successful.
But, the instance creation method can throw an exception! Let's treat it! and the code becomes:

```C++
        int initVulkan(){
            int result;
            try{
                result = createInstance();
            } catch(std::exception e){
                std::cout << e.what() << std::endl;
                return EXIT_FAILURE;
            }
            return result;
        }
```


Next: 
    OpenGL: Let's put some graphics on the screen!
    Vulkan: Defining and configuring the Device for Graphics.

