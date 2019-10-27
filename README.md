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

### [<p>   More Colors</p>](https://github.com/hiperlogic/ComputerGraphics_Studies/tree/02_a_Plain_Color_Triangle_Immediate_Mode_More_Colors)

### [Retained Mode - Programmable Pipeline and More Colors](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/02_b_Plain_Color_Triangle_Retained_and_Shaders/README.md)

### [Using the Application to Change the Color](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/02_c_Plain_Color_Shaders_Color_Change/README.md)

## Multiple Colored Triangle: Per Vertex Color

The triangle drawn in the previous examples were not fully specified by the application. That means, the application did not created each pixel so it could be drawn. It just informed the vertices and what to do with them. OpenGL took care of the rest, meaning, the rasterization and the computations so each pixel could be generated.
This set a question: Can OpenGL set different colors to pixels within the triangle?
The answer is: Yes, it can, and in several ways.
This section will discuss how OpenGL can interpolate color values by assigning a different color to each vertex, either within the fixed function and the programmable pipelines.

### Fixed Function Pipeline Multiple Colored Triangle

Fixed function pipeline is a state machine, so, when a color is set, it is the current color to be used. With this in mind it is easy to infer that for each vertex to be sent, all that is needed to be done is set a new color. OpenGL takes care of the rest.

This may be the shortes section in all this series!

Just set the color you want within the glBegin-glEnd block, one for each vertex, all different color and you're done.

Next: Programmable Pipeline Multiple Colored Triangle


