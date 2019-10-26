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

## 01 - C - Answers about configuring the Vertex Attrib Array

Before we go to proceed to the next project, let's answer those some questions for you to experiment:

1. If the data we are setting is a static and to draw, do I really need to configure the vertex attrib array and set the vertex attrib pointer at each loop pass?

    The data is static, meaning it does not change, the vertex attrib array is the same every loop, and, it is the only one. In this example it can be configured just once, outside the loop.

2. If such configuration (questioned in 1) can be done outside the loop, how to proceed with disabling the vertex attrib array? Does it need to be within the loop? Or outside? If outside, where?
    The vertex attrib array can be configured outside the loop, previous to it, but it must not be disabled within it, or prior to its start. It must be disabled after the loop ends or the triangle will be drawn only on the first frame and then never more. Since the screen is cleared each frame, it will appear that the triangle is never drawn.

The code implements the discussed issue.


Next: Coloring the plain triangle Immediate Mode
