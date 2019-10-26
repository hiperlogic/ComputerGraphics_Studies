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

## [02 - Plain Color Triangle <br>  Immediate Mode](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/02_Plain_Color_Triangle/README.md)

#### More Colors

In the previous example the draw color was set prior to the start of the loop. This means that it is set only once.
If the draw color is set within the loop, each time the loop executes, the OpenGL state machine value for the current draw color is set. This can be a waste of resources if only one drawing is what is done, but can also provide some nice effects.
So, answering the question, yes, the glColor3f command can be written within the loop.
To show this let's provide a new example. Let's configure three colors within one vector and use an index and GLFW keyboard routines to change it.
First, the colors setup is written outside the loop, but within the mainLoop method:

```C++
float colors[] = {1.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.8f, 1.0f, 1.0f, 0.0f}; //Red, somewhat violet and yellow
```

We configure an index to indicate the current color:

```C++
int corrCol = 1; // The somewhat violet is the color of the previous code
```

And set the current color, all just prior to the loop:

```C++
glColor3f(colors[corrCol*3], colors[corrCol*3+1],colors[corrCol*3+2]);
``` 

That instruction also needs to go within the loop, because we want it to affect the drawing when we press a key.
If it is coded only outside the loop, it will not be processed again and the color will be just the same.
Where to put it within the loop is up to you. It can be before clearing the buffers, between clearing the buffers and the drawing instructions, in the beginning or in the end of the drawing instructions, after the drawing instructions, after swapping buffers, even in between the vertices within the drawing instructions, although this would result in a quite weird picture for the very first frame (so you wouldn't notice)!

Finally, the code for the color change. What it will do is just increment the corrCol value and modding it with 3.

```C++
                if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
                    corrCol = (corrCol+1)%3;
                }
```

This way, when you press space, the color will change.
It will go very, very fast... I know... I just show how to change colors, not how to handle key presses. Good luck with that!

Next: Plain Colored Triangle Retained Mode, Version 1 - The Fragment Shader Configuration
