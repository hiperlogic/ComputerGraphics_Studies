# ComputerGraphics_Studies
- Systems Covered (So Far): OpenGL, Vulkan
________________________________________________________________________________
## [Initial Setup](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/master/README.md)

________________________________________________________________________________
## [Initial Configuration: Application Framework and CMake Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_CMake_Project_Create/README.md)

________________________________________________________________________________
## [CMake Specific Configuration, Project Folder Structure and Code](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_a_Project_Structure_And_Setup/README.md)

________________________________________________________________________________
## Finishing the Project Configuration

Cmake and VSCode now are set up and compiling the source within the specific folder for the project, we are ready to end up the first set of code and move on to the next ones.

Since the project uses external libraries, such as glew, glm and glfw, it is important to configure CMake to use those libraries, appending them to the project and configure the build files so they can be linked (statically or dinamycally) in the final executable.

Assuming the resources are already installed in the system, the `CMakeLists.txt` need to be updated by including the following instructions at the end of the file:

```
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)

# Linux systems demands the explicit inclusion of libGL.
if(UNIX)
  target_link_libraries(00_OpenGL_app_framework PRIVATE GL)
endif(UNIX)
target_link_libraries(00_OpenGL_app_framework PRIVATE GLEW::GLEW)
target_link_libraries(00_OpenGL_app_framework PRIVATE glfw)
target_link_libraries(00_OpenGL_app_framework PRIVATE glm)
```

The `find_package` instruction tells CMake to search for the installed package and set all the variables related to them in order to properly compile the project.
Two flags are specified in the instruction: REQUIRED and CONFIG.
Required tells the system to halt the process of creating the building files in case the package was not found. CONFIG tells the process to attempt to locate a configuration file provided by the package to be found.

The last three instructions are responsible to provide the data needed to link the libraries to the project. The first parameter is the project name, this will be important when we add another project to the set.
If the library linked will be provided for other resources other than the project, say, our project is a library that uses it, then the second parameter would have to be PUBLIC. Since we are providing executables, it needs to be PRIVATE.
The third parameter specifies the library to be linked. It also specifies where the build system is supposed to find the “include” files for that set of instructions.

If you groked everything mentioned about CMake and how its configuration is used to create the build files to compile the project, congratulations, you can manage about 80% of project creation with CMake.

Now, to the specifics, the application.

________________________________________________________________________________
## Coding the Application Framework

In order to simplify the process of creating the windows, considering the purpose of the code compiling and running in Windows and Linux systems without (too many) tweaks, the framework GLFW will be used.
This one can be replaced by other frameworks, such as SDL or SFML, for instance, in case you know them better.

The base code implemented in these projects are derived from the Base Code in the [Vulkan Tutorial site](https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Base_code).

The first application will put the app window framework class and the main instructions in the same file for simplicity, but we will improve this along the experimentations.

All code will be generated with an attempt to follow (or pursue) the best principles of software design patterns, even when things seem messy, they will be improved to fit into any good cathegory, so we can recapture all those lessos taught in software engineering.

________________________________________________________________________________
### Principal Class
The following image presents the idea of the first class to be coded, and the main one in the projects:

<img src="images/WindowAppWrapperFirstDraft.png">

________________________________________________________________________________
### WindowAppWrapper Class

Considering the class representation, WindowAppWrapper will be the name of the class to be created in the application.
Applications can have multiple windows, so, in the name of experimentation, this class will not be a `singleton`.

________________________________________________________________________________
#### Private Attributes
It has five private attributes:
* win_width: specifies the width of the window to be created;
* win_height: specifies the height of the window to be created;
* win_title: is the title `string` to appear on top of the window;
* is_running: indicates if the system is running;
* *window: The pointer to the graphical window.

________________________________________________________________________________
#### Private Methods
Along with that, four private methods:
* initSystem: returns false if the system failed to initialize;
* mainLoop: will run until the window is closed, a key to close is pressed or the process is killed;
* cleanup: perform the resources cleanup upon closing the application.
* error_callback: a static method to provide GLFW error information.

________________________________________________________________________________
#### Public Methods
There are no public attributes, but there are four public methods:
* WindowAppWrapper: is the constructor. There are no parameters to be passed in this moment (this will be improved in the future);
* run: this is the method to be called to start the application;
* set_width: adjusts the width of the window to be created. If the window has been created, this method does nothing;
* set_height: asjusts the height of the window to be created. If the window has been created, this method does nogthing.

================================================================================
### Implementations
Let's look at the methods to be implemented. Let's look at them in their order of execution.

________________________________________________________________________________
#### The Public Methods

________________________________________________________________________________
##### The Constructor
A special method within Object Oriented programming, the constructor only initiates the attribute `is_running` with false. So, `this->is_running;` is the only instruction found in it so far.

________________________________________________________________________________
##### Dimension methods: set_height and set_width
Once the object is instantiated, the coder can call set_width or set_height to adjust the desired window dimensions.
Those methods only set the object respective attributes if the `is_running` attribute is `false`, so both codes are somewhat the same:
```
void set_width(int new_width){
  if(!this->is_running) this->win_width=new_width;
}
```
and
```
void set_height(int new_height){
  if(!this->is_running) this->win_height=new_height;
}
```

________________________________________________________________________________
##### The run method
Finally, the only method the code can call: `run`.
`Run` performs basically three things: Initializes the system, calls the main loop and then call the cleanup to finish the application.
But since the system initialization can fail, returnin a false value, we can use this information to control if the main loop will be effectively called, rendering our code as: 
```
void run(){
    this->is_running = 1;
    int result = initSystem();
    if( result != EXIT_FAILURE )
        mainLoop();
    cleanup();
    this->is_running = 0;
}
``` 
Observe that the very first instruction in the run method it indicates that the system is running, reseting it in the last instruction.

================================================================================
#### The Private Methods
Now, for our three remaining private methods, in the respective order: error_callback, system initialization, main loop and cleanup.
________________________________________________________________________________
##### error_callback
Sometimes errors occur and the code logic have nothing to do with it. Sometimes it is due to hardware failure or resources constraints. When this happens, the system crashes and if nothing is presented, we are left wondering: What happened? What did I do wrong? Was it I?
There may be several places where things can go wrong, mainly with a complex system as the graphical management system, as Windows or X, and try to put error verification in every possible place is a huge task. That's why GLFW implemented a method to capture errors related to its instructions (the Window Graphical System) and provide information as it happens, with just a few lines of code.
First we need to write the instruction to return the error. Since this is a common instruction, not related to an object, but to a class, this will be a static method.
It returns no data, the error already occurred. It just prints what happened, but it needs two information: an integer representing the error code and a string, containing the error message, as depicted below:
```
static void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}
```
________________________________________________________________________________
##### initSystem
Graphics application deals with some resources that determine how they will be displayed, where they will be displayed, what resources do they use, what is the data format stored (or to store) in the computer memory, or in the GPU memory, how to use the GPU (I'm talking about you Vulkan!). All of these needs to be configured so the operating system can allocate the resources, inform where they are and how to use them and properly display the windows.
In this section we are dealing only with OpenGL, so let's stick to that and improve later to a more generic solution.
And let's use the GLFW framework to make this kind of configuration easier, and able to execute in several platforms without much trouble.
The main steps taken in the initialization system are:
1. (*) Initialize the framework;
2. (*) Configure the window and the canvas attributes
3. (*) Create the Window with: ```this->window = glfwCreateWindow(this->win_width, this->win_height, this->win_title.c_str(), NULL, NULL);```
4. Assign the window to the graphics processing making it the current context with ```glfwMakeContextCurrent(this->window);```
5. (*) Initializes GLEW

There are three main points of validation in this process.
* If the framework fails to initialize (1.), then there are nothing to be done, something is terribly wrong, check the error messages and seek or ask for solutions. But if happens, the initialization must return an EXIT_FAILURE value.

In order to verify the error, we need to setup the callback routine to be called when an error occurs. The routine was already presented, remaining only its configuration:

```
glfwSetErrorCallback( WindowAppWrapper::error_callback );
```

And the glfw initialization:

```
if(!glfwInit()){
    std::cerr << "GLFW Failed to initialize" <<std::endl;
    return EXIT_FAILURE;
}
```

* In the window creation (3.), if the value returned is NULL, the creation failed. When there are no windows to display the results, the system is useless, so, it must return an EXIT_FAILURE.

```
if( this->window == NULL ){
    std::cerr << "GLFW Failed to initialize" << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
}
```

* GLEW is the tool to set available the OpenGL extensions, much of which are used nowadays. If it fails to initialize (5.), much of the code will fail, so the system must return an EXIT_FAILURE.

```
glewExperimental = true;

if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW Failed to initialize" <<std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
}
```

All of these are easily read from the code, the only one that demands more details is the configuration of the windows and canvas attributes. Let's look at them:

```
glfwWindowHint(GLFW_SAMPLES, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

They all are called using the same instruction: `glfwWindowHint`, only varying their parameters.
`GLFW_SAMPLES` relates to antialiasing processes and tells the system to be configured using MSAA `4`, a four passes antialiasing.
`GLFW_CONTEXT_VERSION_MAJOR`/`MINOR` tells the system that the required OpenGL must not be less than the version 3.3.
`GLFW_OPENGL_FORWARD_COMPAT` tells the system if this code must be compatible with newer OpenGL versions. True in this case.
`GLFW_OPENGL_PROFILE` Tells the system to uyse the OpenGL Core Profile specification for the version specified.

For more information on these hits, consult the [GLFW Window Guide](https://www.glfw.org/docs/latest/window_guide.html)

If all 5 steps went well, then there is the sixth step:
6. return EXIT_SUCCESS;

________________________________________________________________________________
##### mainLoop
In graphical applications the main loop is basically an endless loop that is broken via command.
So, there is need to configure the system to identify where those commands come and which commands are responsible to break the endless loop. This is the main issue in this process.
But let's go a step ahead. In each loop execution a graphics application process the data, drawing them in a buffer and them clear the current buffer to display the updated one, swapping both (if a double buffer is specified... with GLFW it always is for OpenGL).
So, the first instruction is to setup the input methods, done with the following instruction:
```
glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
```

This instruction tells the GLFW process to check the keyboard state and allow it to be probed.

The loop is the second and last instruction. It is a do-while loop with the stop condition of the `ESC` key being pressed or the window being closed.
```
while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0 );
```

The magic happens in between the do-while instructions.
Since we are switching buffers to draw, the very first thing to do is to clear the color buffer. The color buffer is where pixels are drawn and if not cleared, previously drawn pixels can be carried out to future frames if they are not overwritten.
So, just call the clear command from OpengL for the color buffer and all the worries are done:

```
glClear( GL_COLOR_BUFFER_BIT );
```

In the future more information will be provided for this instruction, just be happy that this clear the buffer so OpenGL can draw in a clean canvas.

Then there is the graphics code... the next section (Branch 01) will start this story. So far, be happy for an empty black window...

Once the draws were drawn, they need to be displayed. All graphics instruction, when double buffered (the standard), happens outside view and need to be swapped to be displayed. There is a reason why this is used and I encourage you to look for it, but here is not the place to explain it (Spoiler Alert: Visual Glitches!). Just issue the buffer swap command for the window and be happy you hadn't to code it yourself! (Although it is not that difficult to swap pointers, is it?)

```
glfwSwapBuffers(window);
``` 

finally, the last command within the main loop is responsible to poll events so the data from mouse, keyboard or such can be processed. Easy as doing a:

```
glfwPollEvents();
```

All we need to do is a

________________________________________________________________________________
##### cleanup
When the application closes, the resources it uses must be freed. The frameworks used need to be told to clean up their mess and let room for other applications to come.
Gladly GLFW does that quite easily, two lines of code and you are done!
```
glfwDestroyWindow(window);
glfwTerminate();
```

There, done, our class is ready and can be used...
Hope you've followed the instructions to guide you and coded yourself instead of proceeding to the practice of CTRL+C-CRTL+V, or worse, copied the whole main code!... At least read it and try to understand... it will be good for you in the future!
But you've reached here, so... Kudos! You can write your

================================================================================
#### Main Application

Without further ado:

* Declare the pointer variable to store the address of you window object;
* Instantiate the object (you can do both in just one line, you know?);
* Try and run the app (call the `run` public method within a try-catch structure);
* Catch the error, delete the app and return failure if an error occurred;
* Delete the app and exit with success if no error occurred.

And that's it!

```
int main() {
    WindowAppWrapper *app=new WindowAppWrapper();

    try {
        app->run();
    } catch (const std::exception& e){
        std::cerr <<e.what() <<std::endl;
        delete app;
        return EXIT_FAILURE;
    }
    delete app;
    return EXIT_SUCCESS;
}
```

Build your application and execute it to witness THE... frustration!
If you do not have a library in your path (like glew32.dll) it will fail.
In windows it complained about `glew32d.dll`. That is the same as `glew32.dll`, just copy it and rename it and be happy!
If the error persists, check the lib compilation. 
* Is it x86 or x64?
* Is the lib dir in the path?
* How about copy the lib binary (dll) to the same folder as is the project executable (build)?

_________________________________________________________________________________________________________________________
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


Comming Next: 
    OpenGL: I don't want to clean to Black! I Want to Clean to Fuchsia! How to do that?
    Vulkan: Setting up the Debug application
