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

## 01 - B - Sending Data to OpenGL - Programmable Pipeline

From OpenGL 2.0 onward a new pipeline was introduced, the `Programable Pipeline`, an improvement and simplification of the fixed function pipeline that added a generic processor to the `Transform and Lighting` stage, now called `Vertex Shading` and to the section related to the visual processing of the result sent by the rasterizer, that is the processes of `Texture Environment`, `Color Sum`, `Fog` and `Alpha Test`, which was labeled `Fragment Shading`.

So, we had the following in the fixed pipeline:

> API->Primitive Processing->`Transform and Lighting`->Primitive Assembly->Rasterizes->`Texture Environment->Color Sum->Fog->Alpha Test`->Depth and Stencil->Color Buffer Blend->Dither->Frame Buffer.

And got the following in the programmable pipeline:

> API->Primitive Processing->`Vertex Shader`->Primitive Assembly->Rasterizes->`Fragment Shader`->Depth and Stencil->Color Buffer Blend->Dither->Frame Buffer.

