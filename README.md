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

Next: Sending Data to OpenGL
