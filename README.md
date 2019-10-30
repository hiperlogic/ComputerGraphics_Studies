# ComputerGraphics_Studies
- Systems Covered (So Far): OpenGL, ~~Vulkan~~
________________________________________________________________________________
## [Initial Setup](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/master/README.md)

________________________________________________________________________________
## [Initial Configuration: Application Framework and CMake Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_CMake_Project_Create/README.md)

________________________________________________________________________________
## [CMake Specific Configuration, Project Folder Structure and Code](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_a_Project_Structure_And_Setup/README.md)

________________________________________________________________________________
## [Finishing the Project Configuration](https://github.com/hiperlogic/ComputerGraphics_Studies/blob/00_b_app_framework/README.md)

________________________________________________________________________________
## App Framework Extras

There are a couple of things to discuss about the application framework that we will improve during this process.
The first is the clear color. This is a very easy task to do and will be discussed here.
Then there is improvements to CMakeFiles in order to make our lives easier. Since CMake is fundamental, here this will be presented as well.
Finally there is the issue about the class that represents the app. It uses GLFW, but it is also calling OpenGL commands (I'm talking about you, glClear... and the command to set the clear color that will be presented here as well!).
The AppFramework class should not know which technology it relates to. It should only provide the context to the techonogy so it can use its canvas. This will be done in the next step, when the Vulkan technology will start to be discussed.
So, without further ado...

________________________________________________________________________________
### Cmake Improvements

This series (the set of texts in the ReadMe along with code within all branches, let's call it series), in the end, will have a lot of projects.
There are 20 projects planned for OpenGL. I'm not sure how much for Vulkan yet.
And, there are at least 4 lines at CMakeLists that configure the libs for each project. This will provide us with 80 lines, at least, just for the OpenGL code!
Unfortunately linux systems demand the explicit instruction to include the OpenGL library to the project, but Windows do not.
Even worse, Windows complain when we do it! That's why there is a `if` clause for it.
So, at least 20 lines of instruction to include OpenGL we will have, but the others can be cut to just one line, writing each lib right next to the previous, with spaces between them, and the three lines: 

```
target_link_libraries(00_OpenGL_app_framework PRIVATE GLEW::GLEW)
target_link_libraries(00_OpenGL_app_framework PRIVATE glfw)
target_link_libraries(00_OpenGL_app_framework PRIVATE glm)
```

Becomes one:

```
target_link_libraries(00_OpenGL_app_framework PRIVATE GLEW::GLEW glfw glm)
```

Just make sure that this is the order of setting:
* first OpenGL, in the separate instruction within the if statement;
* then the other libraries starting with GLEW.

And the (at least) 80 lines of library configurations for the linker, in the end of this series, will become 20 for the OpenGL, one for each project, and 20 for the other libraries, also one for each project, reducing to 40.

________________________________________________________________________________
### Clearing to Other than Black and a bit of State Machine

Ok, I have no idea what fuchsia is. I know it is a color, but no idea which one. So, I will clear to cyan.
Putting it simple, OpenGL is a state machine that operates on vertex, colors and texture coordinates. It is a little more complex than that, but this is the basis to get started well with OpenGL concepts.
This means that you configure the OpenGL state, send in the vertexes, colors, texture coordinates and the texture, set what you want to draw and tell it to do it.
The color to clear the screen is no different. It is just one command that will set it to the configuration until it is changed (usually it is not!)
It is advisable to adjust basic configurations outside the main loop. Setting it within the main loop is waste of resources. No one likes to be remembered about how to do one action every time she needs to do the action! So, right before the `do` statement, within the `mainLoop` method, all you need to do is write the instruction:

```
glClearColor(0.0f,1.0f,1.0f, 1.0f);
```

This instruction tells the OpenGL to configure the color to clear the screen considering no `red` component, full `green` component, full `blue` component and full `alpha` component.
`Alpha` states for opacity, so 1.0 means full opacity.
The color is constructed combining the red, green and blue components. In this example the combination of green and blue, with absence of red, produces cyan.
This color triplet is usual among several software, from OpenGL to painting, illustration or design softwares.
So, if you know the triplet combination to the color fuchsia, you can compute how much of each color component is needed and fill it yourself, configuring your screen to clear to fuchsia.


Next: 
    OpenGL: Let's put some graphics on the screen!
    Vulkan: Defining and configuring the Device for Graphics.

