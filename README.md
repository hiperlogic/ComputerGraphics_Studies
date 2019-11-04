# ComputerGraphics_Studies
- Systems Covered (So Far): OpenGL, Vulkan
________________________________________________________________________________
## [Initial Setup](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/master/README.md)

________________________________________________________________________________
## [Initial Configuration: Application Framework and CMake Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_CMake_Project_Create/README.md)

________________________________________________________________________________
## [CMake Specific Configuration, Project Folder Structure and Code](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_a_Project_Structure_And_Setup/README.md)

________________________________________________________________________________
## [Finishing the Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_b_app_framework/README.md)

________________________________________________________________________________
## [App Framework Extras](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_c_app_framework_extras/README.md)

## Creating the First Graphics and A New Project with OpenGL

Now we improve our code, and let's do that by adding some basic graphics in the screen.
Since the first project is the main app framework, which will suffer improvements pretty soon, let's create another project in order to set that one as main reference.
The first thing is to create the folder. Let's create it within the folder `OpenGL` and name it `01_plain_triangle`, copying the `main.cpp` file from the folder `00_app_framework`.

### Updating CMake

A new project was created and it need to be configured in CMake in order to the executable to be created.
The basis of the CMakeLists file is done, there are very little to be added. In summary, just another executable and the configuration for its libraries.
In the future documents this won't be demonstrated, so follow this guideline, understand what is going on and how things work in order to proceed in the next sections.

The actions to be made with CMakeLists is to add the executable and set up the libraries to the target.
Adding the executable is already done for project 00_app_framework in the line with the instruction

```
add_executable(00_OpenGL_app_framework OpenGL/00_app_framework/main.cpp)
```

To add anothe executable just write a new add_executable underneath it. Pay attention for the executable name (first parameter) and the path of the project.
so, your new instruction would be:

```
add_executable(01_OpenGL_simple_triangle OpenGL/01_simple_triangle/main.cpp)
```

And it is almost set. All that remains is to adjust the libraries to be linked in the executable generation.
All the needed libraries, so far, are already indicated via `find_package` instruction. There is no need to call these instructions anymore.
What needs to be done is to set the `target_link_libraries` to the new project.
The OpenGL configuration for linux needs to be explicitly instructed and it was coded within a conditional clause, so it must be included for this project as well.
So just duplicate the OpenGL `target_link_libraries` instruction, maintaining the new one within the conditional clause, and adjusting the target name, that is, the project name, for the name set in the `add_executable` created, that is `01_OpenGL_simple_triangle`.

Finally, to configure the other libraries to be linked with the new project, the procedure is similar, just with the libraries instruction.

In the end the CMakeLists file will contain the following content:

```
cmake_minimum_required(VERSION 3.0.0)
project(CG_Studies VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(00_OpenGL_app_framework OpenGL/00_app_framework/main.cpp)
add_executable(01_OpenGL_simple_triangle OpenGL/01_simple_triangle/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)

# Linux and Unix systems demands the explicit linking of OpenGL
if(UNIX)
    target_link_libraries(00_OpenGL_app_framework PRIVATE GL)
    target_link_libraries(01_OpenGL_simple_triangle PRIVATE GL)
endif(UNIX)

target_link_libraries(00_OpenGL_app_framework PRIVATE GLEW::GLEW glfw glm)
target_link_libraries(01_OpenGL_simple_triangle PRIVATE GLEW::GLEW glfw glm)
```

Recapturing:
To Include a new project:
1. create the folder within the technology folder to be used (OpenGL in this case)
2. copy the main.cpp from the last project, since we are building this set incrementally
3. add the executable to the CMakeLists, pay attention to the project (executable) name and the path to main.cpp
4. set the OpenGL library to be included in the executable generation for the project
5. set the remainder libraries to be included in the executable generation for the project

You can now build and check if the executable for the new project was created and is working properly.
And yes, you will see nothing. The source was not modified from the previous section yet. It will be done next.

## Probing, Detecting, Configuring and Logically representing Devices and Creating a New Project for Vulkan

Now we improve our code, and let's do that by adding some basic graphics in the screen.
Since the first project is the main app framework, which will suffer improvements pretty soon, let's create another project in order to set that one as main reference.
The first thing is to create the folder. Let's create it within the folder `Vulkan` and name it `01_plain_triangle`, copying the `main.cpp` file from the folder `00_app_framework` defined in the last section (branch `00_c_*`).

Configure it in CMake, by adding an executable and setting up the includes and libraries

```
add_executable(01_Vulkan_simple_triangle Vulkan/01_simple_triangle/main.cpp)
```

It is assumed you are continuing from the previous project, there should already have a `find_package(Vulkan)` in your CMakeLists file. All needed to be done is to include the Vulkan Directory to the target and and needed libraries (glfw and glm).

### Probing the physical devices

Vulkan instance is not the Driver itself. It can be considered as the first layer of communication between the application and the hardware.
Graphic cards were very expensive in the past and just the idea of having one was heavy in the budget. That's why drivers configuration, or this layer that the Vulkan instance represents, were usually a 1:1 mapping to the hardware. And developers suffered from it! Usually they had to write one routine version for each driver. It was a mess.
OpenGL is not that different. Although it is usually generic, the availability of proprietary extensions, not only among companies, but among companies products, usually made things messy. But, at least the developers could code extensions use within code and not worry about drivers. Select a minimal and viable set of extensions, the cards they are all implemented and go for it.

How would this be with Vulkan?

Nowadays a system can have more than one, or two, graphic cards. And an application can use more than one!
Within OpenGL I know there are settings in operating systems to select which card would run which software.

Vulkan interface have instructions to probe the system for GPUs. And to configure them all.

This is the next step in setting up the Vulkan System (yes, this takes way longer than in OpenGL!) and will proceed with a new private, `pickPhysicalDevice` method to our `WindowAppWrapper` class (that is becoming huge... and quite specific! By the end of branch 03 let's make things more... professional... by applying Software Engineering Patterns).

For now let's focus on only one graphics card, so, let's create one more private attribute to store it in the class. The attribute is of type `VkPhysicalDevice` and, for simplicity, it is implicitly destroyed when VkInstance is destroyed, so, no need to code it on cleanup.

```C++
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
```

I'll consider you haven't paid too much attention to what I've been writing, so, let me re-state this subject: Vulkan have a pattern to retrieve information. Usually the API to enumerate features or capabilities have three parameters. Some parameters can be null pointers and the combination of null pointers and parameters retrieve different set of data.
There is one parameter that is mandatory, and that is the address to where the number of features available is (to be) stored. If all the other parameters are null, then this address will retrieve the number of probed features.
One parameter is where (also a memory address) the features names (or their proper enumerations) are (to be) stored. This parameter must be provided with the number of features to be retrieved (usually the same as the card provides).
The last parameter states to which specific information the count and the enumerations belong to. It can be a layer name, an extension name or a handler, for the instance, in this specific case.

After all, it is via created instance that we can probe the systems informations. So, let's declare two variables in the private method we just created, proceed to validations and probing. The variables, an unsigned integer named deviceCount, to store the number of graphics devices and a vector of vkPhysicalDevices, to store the physical devices available in the graphics system.
The instruction to probe them is `vkEnumeratePhysicalDevices`, which has the following signature:

```C++
    VkResult vkEnumeratePhysicalDevices(
        VkInstance instance,
        uint32_t* pPhysicalDeviceCount,
        VkPhysicalDevice* pPhysicalDevices);
```

Providing all needed to populate the method:

```C++
void setPhysicalDevice(){
    // Store the number of physical devices
    uint32_t deviceCount=0;
    // Retrieves the number of physical devices available via instance
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    // Throw exception if no Vulkan supported devices were found
    if (deviceCount == 0) {
        throw std::runtime_error("No Vulkan supported GPU found!");
    }

    // Store the devices
    std::vector<VkPhysicalDevice> devices(deviceCount);
    // Retrieve the devices enumeration within the instance and stores them in the address pointed by devices.data()
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Will continue

}
```

The environment probeb by Vulkan instance can contain several devices, not only graphics devices. This means that we need to check all the physical devices to select those for the desired needs. In this case, graphics.
Within Vulkan the devices provide elements called `queues`. Vulkan queues can be thought as data or command buses, so, let's consider them as that.

Within base device drivers the devices specifies to which addresses each bus can be acessed, which bus are for retrieval or for sending data, if the bus is bidiretional or onedirectional and most important, which kind of data to send or retrieve via that bus and what is the protocol (data patterns). This somewhat the same for Vulkan queues.

So, we need to check for the device properties and what kind of queues are available in the device. Let's traverse the list of the devices and select the very first device that is suitable:

```C++
    // Will Continue
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)){  // How to know it is suitable?
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("Failed to retrieve a suitable Graphics device!");
    }
```

So, the very first suitable device is stored in the class attribute and the loop exits. If the loop ends and the physicalDevice is not changed, it throws an exception. However, there is still the need to find the suitable device!

#### Retrieving the device properties, queues and memories information

There are three types of information needed to properly select (and configure) the physical device. The `queues` are the data transmission lines, where data are sent to the device or retrieved from it, the `memories` are where data are stored and the `properties` that describe the devices.
The suitability of a physical device is related so a set of these properties and features. They must be probed and analyzed in order to select the graphics card to be drawn.
Device properties have its own structure describing their capabilities, `VkPhisicalDeviceProperties`, as well as a structure to describe its features, `VkPhisicalDeviceFeatures`. Both can be retrieved by their related instructions: `vkGetPhysicalDeviceProperties(VkPhysicalDevice, VkPhisicalDeviceProperties*)` and `vkGetPhysicalDeviceFeatures(VkPhysicalDevice, VkPhisicalDeviceFeatures*)`



Next: Sending Data to OpenGL
