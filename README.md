# ComputerGraphics_Studies


## [Initial Setup](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/master/README.md)


## [Initial Configuration: Application Framework and CMake Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_CMake_Project_Create/README.md)


## [CMake Specific Configuration, Project Folder Structure and Code](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_a_Project_Structure_And_Setup/README.md)
- Systems Covered (So Far): OpenGL, ~~Vulkan~~

Cmake and VSCode now are set up and compiling the source within the specific folder for the project, we are ready to end up the first set of code and move on to the next ones.

Since the project uses external libraries, such as glew, glm and glfw, it is important to configure CMake to use those libraries, appending them to the project and configure the build files so they can be linked (statically or dinamycally) in the final executable.

Assuming the resources are already installed in the system, the `CMakeLists.txt` need to be updated by including the following instructions at the end of the file:

