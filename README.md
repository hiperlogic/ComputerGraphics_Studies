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

## 02 - Plain Color Triangle

So far the instructions discussed how to configure CMake, create a window app (framework) and cconfigure OpenGL for immediate or retained mode printing a plain triangle.
In this project it will be shown how to change the color of the triangle, still maitaining it plain, in both modes: Immediate Mode and Retained Mode.
So, create a new project in CMakeLists. I would name it 02_OpenGL_plain_colored_triangle. I won't create two different projects for immediate and retained mode.

### Immediate Mode

The immediate mode commands are processed as soon as they are sent. This means, they set the state machine right away.
Simply put, the command glColor will set the current drawing color, so each vertex sent will be considered to be drawn with that color.
Considering the code written for the immediate mode in the previous project, let's write the command right before the start of the loop in the mainLoop method.

```C++
glColor3f(1.0, 0.1, 0.8);
```

And we are set. You have a quite pinkish triangle. Want to change its color, change the values and recompile.

Question: Can I put the color changing within the loop?

Let's take a look at that in the next section.

Next: Changing Color with the App in the Immediate Mode
