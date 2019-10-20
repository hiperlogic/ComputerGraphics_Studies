# ComputerGraphics_Studies


## [Initial Setup](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/master/README.md)


## [Initial Configuration: Application Framework and CMake Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_CMake_Project_Create/README.md)


## CMake Specific Configuration, Project Folder Structure and Code
- Systems Covered (So Far): OpenGL, ~~Vulkan~~


Since we are covering some technologies (OpenGL, and ~~Vulkan~~), along with creating one executable per main feature, it is interesting to specify some structure to our process.
The very first one is related to the technology, so I'd advice creating two folders, one for OpenGL code and other for Vulkan. But some code will need to use common routines or objects, which would require duplicated files.
In order to avoid file duplicates that are common to both technologies, it is advisable to create a third folder to store such routines, let's call it `utils` and put up a dummy source file just for start.


Now there are four folders in the project: build, OpenGL, Vulkan and utils. (there is also the images folder, used to store the images to be displayed in this document and the “.vscode” folder if you've done everything correctly.)


Within the OpenGL folder let's create the features folders and move the `main.cpp` file in the root folder, created by the `CMake Quickstart` command in order to use it.

In order to create the features folder we need to know which features we will cover. The very first one is the app_framework, reponsible to create a window that will display the results of each of the following codes.
There are 20 folders in total. It is a lot to cover! But we will use the previous code to build the next one, so the steep won't be that high!

So, create the first folder within the OpenGL folder, naming it `00_app_framework` and move the `main.cpp` file created by CMake Quickstart to it.

Now, the main file specified in the `add_executable`, at CMakeLists.txt, was moved. Considering that the first build generated an executable that printed the “Hello, World!” in the command line, try to build and check the error generated. It will have an error! Your source code is no longer in the place it says it is in the CMaleLists.txt.


To fix this is easy, change the add_executable instruction at CMakeLists.txt from `main.cpp` to `OpenGL\00_app_framework\main.cpp`. Now you can build and check the result.


A tip of advice: Since we will produce a configuration to generate build files for different projects, it would be interesting if instead of `CG_Studies`, the name of the project was more specific.
This project will use the following terminology for this process: XX_Tech_FeatureExplored, where:


* XX is the branch number order (00, in this case)
* Tech is either OpenGL or Vulkan
* Feature Explored is the name of the intended feature to explore (app_framework, in this case)

So, the new CMakeLists.txt would be:
```
cmake_minimum_required(VERSION 3.0.0)
project(CG_Studies VERSION 0.1.0)


include(CTest)
enable_testing()


add_executable(00_OpenGL_app_framework OpenGL/00_app_framework/main.cpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
```
