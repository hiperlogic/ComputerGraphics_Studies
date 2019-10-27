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

### Retained Mode - Programmable Pipeline and More Colors

Since the fixed function pipeline had already everything coded to treat vertices, lighting and colors, programming those effects was easy as calling the command.
With the programmable pipeline the colors need to be processed by the fragment shader, but the fragment shader get their data from the vertex shader. Or can the fragment shader be informed without the vertex shader?

As you've may have wondered, there were no shaders coded in the examples. The retained mode coded in the previous project hasn't specified shaders. It used the most basic operations.

Shaders are source code, are string files that are sent to the OpenGL state machine and compiled prior to their use.
OpenGL does not deal with string or file loaders, this must be done via applications and will be done within the application in this section, but will be improved in the future.

Let's add a new method to our class so it can load a string file and configure the proper parameters for the shaders.
There are 4 kind of shaders since OpenGL 3.0: Vertex, Fragment (or Pixel), Geometric and Computational. We will deal with just the first two.

#### The loadShader Util

The loadShader Util will be a simple string file loader that will return a string object with the file content.
Since it is my intention to use it in several projects and improve it properly, then let's code it as an utility library.
This utility library will be improved during the project.
First of all, we need to make changes to our CMakeFiles.
Since we are adding a library of utilities, and building it, you should know that such kind of project will have a set of include files of its own.
But first, it is important to add the `utils` directory to the list of directories in the compiler. So, open the command pallete and look for `C/C++: Edit Configuration (UI)`, seek the place where it configures the paths to the include files and add the following line:

```
${workspaceFolder}/
```

Now, the CMake needs to find the header to be used. This is done similarly to the library and it will be needed for each project that will use the utils library.
So, in the CMake, right before the specifying the libraries to the projects, add the following instruction:

```CMake
    target_include_directories(02_OpenGL_plain_colored_triangle PRIVATE utils)
```

The library needs to be configured and compiled, this means, we need to add a library project, but also, we need to configure where are the include files for that library, and what files needs to be included.
The following instructions will provide the desired effect, add them just before the add_executables instructions:

```CMake
set(HEADER_FILES utils/loadShader.hpp) # Set a list of files to be used in the future

add_library(cgst_utils utils/loadShader.cpp ${HEADER_FILES}) # Configures a library, generating a cgst_utils.lib file
# It uses the files specified in the variable HEADER_FILES
```
The library project is completed. It will be improved with as this series proceeds.

Finally, the last update is to the libraries linked to the project, include the generated library `cgst_utils` to the list.

Great! Now you know how to create a static library! And how to use it! Now, for the loader!

### The Shader Loader

The shader loader will contain only one function and its responsibility is to return a string.
In the moment the only thing to be verified is if the file exists. It will not validate if it is a string file, nor if it is a proper shader.
So, the loadShader.cpp is created and there is the need to add the includes to it. The code will deal with strings, file streams, string streams and let's add some message to the console as well, so, io stream. Set up the std namespace as well, just for the sake of simplicity.

```C++
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
```

Yes, including its own header is important, so it knows how to bind the binary to the codes that uses it:

```C++
#include "loadShader.hpp"
```

For the code, it is straightforward. It receives a string indicating the filepath. A filestream will be opened for reading (in) using the filepath and it is read by a string stream.
The content of the string stream will be dumped to a string, the filestream will close and the string is returned.
If the filestream could not be opened, it will print to the console (I know... logging is the way! But we are trying to be simple!) and throw an error. Just as the following code demonstrates:

```C++
string LoadShader(string file_path){
    std::string ShaderSource;
    std::ifstream ShaderStream(file_path.c_str(), std::ios::in);
    if(ShaderStream.is_open()){
        std::stringstream sstr;
        sstr << ShaderStream.rdbuf();
        ShaderSource = sstr.str();
        ShaderStream.close();
    } else {
        cout <<"File I/O Error. " << file_path << " Not Found";
        throw "File I/O Error. ";
    }
    return ShaderSource;
}
```

That is it for the source, now, for the header, the code is simple. Check if the header is defined, if not defined, define it and expose the function header, as shown in the following snippet:

```C++
#ifndef LOADSHADER_HPP
#define LOADSHADER_HPP

string LoadShader(string);

#endif
```

You can even compile and generate the library to use it wherever you like. It reads a text file and returns a string with its content! Neat!

### The Shader Configurator

In the main file we will need to include the loadShader header. Since we defined in CMakeLists that the utils directory is to be considered as an inclusion dir, just add the last inclusion:

```C++
#include <loadShader.hpp>
```

And let's code the shader configuration routine.
There are certains specificities to shaders within OpenGL. There is the type, as we already discussed, there are four: Vertex, Fragment, Geometry and Compute. They need to be created, its source loaded, specified and compiled.
After that a program needs to be created, the shaders attached to it and then linked.
Once linked, the shaders can be detached and deleted, since the program will contain and execute their code.

One interesting bit is that the shader can have multiple sources, so, you can create a composite effect with just one shader.

First, let's create the shader.
As mentioned, for now, a private method will be created in the WindowAppWrapper Class. It is not meant to be there, but let's do this just for now! And it will be called `createShader`, returning a GLuint.
It receives a string composed by GLchar and an enumerator indicating the shader type.
The enumerator is used within the instruction `glCreateShader` to specify the shader to create and return it to be stored and used.
This is where the source is loaded, so, it is where the recently created library will be called. The result will be stored as well, but it is a string and we need an array of string pointers to be specified in the shader source instruction, so, a set of operations is needed.
First the c_str() result of the source string is stored in a const char* variable. Then, the address of that variable is stored in a const char* const* variable. Confuse? I'm a little bit too.
This is needed because the instruction to specify the source can receive more than one source code. But we are considering just one for the moment, so, just follow along the code and be happy. If you understand this all, the better! But just know what is needed and go for it! So, the code to "treat" the string is:

```C++
string shaderSource = LoadShader(string(shader_name));  // Load the string
const char* str_p = shaderSource.c_str();  // Retrieves the string as a C string. Assign it to str_p variable
const char* const* sources = &str_p; // The address of str_p is assigned to the sources
```

With the shader created and with the source properly adjusted, it is time to assign the source to the shader and compile it.
The assignment is done via instruction `glShaderSource`, with the following arguments:
* shader: The shader created;
* num_srcs: The number of sources to the shader. In our current case it is 1
* An array of pointers to strings. Each string is a source for the shader.
* Array of int indicating the length of each source string, mapped by position. In our case it is a null pointer because we are using only one string.

Finally, compile the shader with glCompileShader and return the shader.
So, the following code shall be easy to read by now:

```C++
        GLuint createShader(GLchar* shader_name, GLenum shaderType){
            GLuint shader = glCreateShader(shaderType);
            string shaderSource = LoadShader(string(shader_name));
            const char* str_p = shaderSource.c_str();
            const char* const* sources = &str_p;
            glShaderSource(shader, 1, sources, nullptr);
            glCompileShader(shader);
            return shader;
        }
```

### Creating the program

Now that we have the routine to create the shader, the next steps can be performed.
The steps consists in creatiung the program, attaching the shaders, linking the program, detach the shaders, delete the shaders and use the program.
This will be done in the mainLoop method by now, just before the vertex array generation.

First, we need to create the shaders. Let's call them `vertex1.glsl` and `pixel1.glsl`, the first is a Vertex Shader and the second, a Fragment Shader.

```C++
GLUint vertexShader = createShader("vertex1.glsl", GL_VERTEX_SHADER);
GLUint fragmentShader = createShader("pixel1.glsl", GL_FRAGMENT_SHADER);
```

Then, we retrieve the created program with the instruction glCreateProgram and attach both shaders to it:

```C++
GLUint shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
```

Linking the program means that the compiled code of the shaders are merged within the program. At this time the program is ready to be set to use. And also detach and delete the shaders.

```C++
glLinkProgram(shaderProgram);
glUseProgram(shaderProgram);

glDetachShader(shaderProgram, vertexShader);
glDetachShader(shaderProgram, fragmentShader);

glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```

The program is ready to be compiled without errors, but try and execute it!... yes, Error!
This is because the shaders we are telling to load (`vertex1.glsl` and `pixel1.glsl`) do not exist.

So, let's create them.

### The Shaders

Let's start with the two shader files empty. Create them in the `build` folder, that is where your executables are generated. So, there is where they will be loaded according to what we have coded so far.

Execute the program and witness the result!...

It is the same as it was without the code for shaders! Of course, the shaders are empty, so, it is as if they are the base state!

Let's code them. First the Vertex, then the Fragment.

#### The Vertex Shader

Shaders evolved with time. They became easier. So, you may encounter shader codes that are old, from the initial versions, others that are new. How to differentiate them? Some keywords are enough for this, but it is advisable to indicate the shader version in the code, in the very first line. Like the following:

```C++
#version 150 core
```

Done, this vertex shader is indicated to be version 150, core set.

This vertex shader does nothing, really, it is a `nop`, but let's improve this `nop` to illustrate some concepts.
Vertex shaders receive... well... vertices. So, there are some things we did at the code that refer to vertices, and they need to bind with the shaders.

Shaders receive data from a source and sends data to a destination. Vertex shader source is the application, its destination, the fragment shader. 
The sources must be identified, yes, there can be more than one source. For the version we are working, the source identification is indicated with the keyword `in`. Previous versions may be different.

Vertex shaders receive vertices, at least. So, let's indicate that source as:

```C++
layout(location = 0) in vec3 pos;
```

The layout modifier indicates that the vertices are being defined at vertex attrib location 0. We have seen this in the application code before, right at the instructions:

```C++
glEnableVertexAttribArray(0);
```

Where 0 indicates the location of the attribute. And

```C++
glVertexAttribPointer(
    0,          // Whith vertex attrib array index to use
    3,          
    GL_FLOAT,   
    GL_FALSE,   
    0,          
    (void*)0    
);
```

The only line with comment relates to the attribute. Those are the instructions that bind the vertices in the application to the vertices in the shader. But, this happens because we are just configuring vertices, the glVertexAttribPointer instruction is right after a binding to the vertex buffer. The vertex buffer has no stride after the 3 information data (the vertex), nor offset.

The `in` keyword at the shader instruction indicates that this is a variable that receives data from the source (the application).

Right before the variable name is the variable type, `vec3` indicates that this is a vector with 3 positions, followed by its name: `pos`, labeled this way to indicate the representation of a position.

For now this is everything we need as identifiers, then come the main routine, which specifies that the built-in variable *gl_Position*, which represents the vertex, receives the vec3 position data, but, gl_Position is a vec4 data. This is easily solved by casting the convertion instruction (let's call it this way just for clarity) vec4:

```C++
void main()
{
    gl_Position = vec4(pos, 1.0);
}
```

And this is how we get a big `nop` code, coding.
So, you should know that main is the entrypoint, if you are to specify the main routine, you need to specify the gl_Position. And... if you have a vertex shader, you need a fragment shader, which we will discuss next.

#### Fragment Shader

Fragment shaders are not that much different from vertex shaders. You should indicate its version, the variables that receives data from the source, the variables that output data to the remaining stages of the pipeline and the code whose entrypoint is the `main` routine.

In this example we want a colored plain triangle, so, all we need to do is specify the draw color, not much.
So, after the version, just declare the outgoing variable (use the keyword `out`) of type `vec4` (consider the 4th color an alpha. Put 1.0 in it for now and you won't miss!) and name it... currColor, like that:

```C++
    #version 150 core

    out vec4 currColor;
```

Neat! Now, we just need to code the main routine.

```C++
    void main()
    {
        currColor = vec4(1.0, 0.1, 0.8, 1.0);
    }
```
We set up the color to be that same pinkish color for the immediate mode we did in this section.
If you have already built the project, like at that time when no shaders existed and an error occurred, you do not need to build it again. These, the shaders, are just text files.
You can go and execute the program to witness the pinkish triangle in the blue background.
You can play with the shader as well... change the color, easier than having to recompile again, huh!?
But what if... I want to change the color from the application? Using the keyboard like before?

Next: Using the Application to change the triangle color.