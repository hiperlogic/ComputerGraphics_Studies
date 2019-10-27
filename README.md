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

### Using the Application to Change the Color

Shader and application are somewhat decoupled. The application send its data to the pipeline, which, as we experienced, can or cannot have shaders, although without shaders the result may not be what we expect.
The application prepares the data, the shaders process the data and generate the result. This means that apart from the usual vertex attrib pointer in the application and inbound variables in the shader, there must have some other form to inform parameters to the shaders, this is where the `uniform` keyword is used.

The `uniform` keyword, indicate variables whose values are retrieved from the application. They can be defined either in vertex or fragment shader. So, this is what we are looking for.

In order to use a uniform variable, apart from defining it in the shader, the variable needs to be located in the shader program from within the application. A process that is done with the command `glGetUniformLocation`, which receives two arguments: the program and a string containing the name of the variable.

If we define the variable in the vertex shader, since in this particular case it is a value that indicates the color to be drawn, it also needs to be output to the fragment shader, but it can be written directly in the fragment. Let's use the easiest form for this project and write it in the fragment shader.

Just after currColor, write the following sentence:

```C++
uniform vec4 recColor;
```

This declares a vec4 variable with the name recColor whose value is retrieved from the application.
Now we need to map this color to the output color just rewriting the only instruction in the main routine:

```C++
currColor = recColor;
```

Just make sure to set the recColor prior to calling the draw instructions and we're set for the fragment shader.

#### Passing the values to the Uniform.

It is the application responsibility to probe for the information where to store (send) the value on the OpenGL state machine so it can be retrieved by the desired uniform variable.
In order to do it, the `glGetUniformLocation` must be provided the program and the name of the variable, returning an identification to be used as reference to set the value in future instructions. The instruction needs to be declared after the program is set to be used, so, after the shaders deletion just write the following:

```C++
GLuint recColor = glGetUniformLocation(shaderProgram, "recColor");
```

That retrieves an identification so that when there is the need to change the color in the fragment shader, it can be done within the application. The application variable name and the shader variable name are just incidental, they can be different. The decision to name them the same is just for sake of simplicity in knowing which context is being dealt with.

In the fixed function pipeline example we set up an array with 9 values representing 3 colors and used an offset and the specific positions with glColor3f instruction to change the draw color, but the value in the shader is a vector of 4 floats. Also, glColor won't work with programmable pipeline.

Since the reference for the variable in the shader was retrieved by the application, we can set its value directly just calling the instruction glUniform, wich has a naming construction similar to glColor or glVertex.

So, let's create the 9 values vector for the color, as did in the fixed function pipeline, the integer variable storing the current color and the keyboard processing instructions. 

All that is left to do is to code the instruction that would do the function of glColor, meaning, the instruction to effectively set the current draw color. In this example this is done by setting the values in the fragment shader, in the uniform variable, so, it is just a matter to call, within the loop, the instruction:

```C++
glUniform4f(recColor, colors[corrCol*3], colors[corrCol*3+1],colors[corrCol*3+2], 1.0f);
```

With that the project is ready to be compiled and tested.

Next: Multiple Colored Triangle