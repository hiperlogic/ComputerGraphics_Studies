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

## 01 - B - Sending Data to OpenGL - Vertex Buffer Objects

From OpenGL 2.0 onward a new pipeline was introduced, the `Programable Pipeline`, an improvement and simplification of the fixed function pipeline that added a generic processor to the `Transform and Lighting` stage, now called `Vertex Shading` and to the section related to the visual processing of the result sent by the rasterizer, that is the processes of `Texture Environment`, `Color Sum`, `Fog` and `Alpha Test`, which was labeled `Fragment Shading`.

So, we had the following in the fixed pipeline:

API->Primitive Processing->**_Transform and Lighting_**->Primitive Assembly->Rasterizes->**_Texture Environment->Color Sum->Fog->Alpha Test_**->Depth and Stencil->Color Buffer Blend->Dither->Frame Buffer.

And got the following in the programmable pipeline:

API->Primitive Processing->**_Vertex Shader_**->Primitive Assembly->Rasterizes->**_Fragment Shader_**->Depth and Stencil->Color Buffer Blend->Dither->Frame Buffer.

### The Vertex Shader

At first glance it seems that little to no change was made to the transformation and lighting processing, since one stage was replaced by another, but this is a wrong perception.
First of all, the `Transform and Lighting` stage is limited.
The OpenGL matrices operation needed to be called with OpenGL specific operations like `glRotate` and `glTranslate`, this made the programming easier, but the matrices data could not be changed once the vertices were sent. They were fixed.
Also, OpenGL had only 8 configurable light sources and like the matrices, they could be changed only via application.
With the Vertex Shader we have new possibilities. We can change the transformation matrix per vertex sent, if we want, producing several advanced effects like water refraction, for instance, in the vertex domain, not on the fragment domain, considerably speeding up the process.
The illumination model may have become a little bit more complex (and we will see it when talking about lighting), but the 8 light sources limit is no longer valid, nor the fact that they are static, but the Vertex shader is the one responsible to provide data related to surface normals to the next pipeline stage in order to the illumination be properly computed. So, the programmer has to permorm a little extra effort and have a little extra knowlegde about illumination, but gains a lot more liberty to perform the effects he wants.
To make things better, extra information can be used to process lighting parameters to be used by the fragment shader, such as Normal Maps, Bump Maps or Speculation Maps, producing really impressive graphics.
These features will be detailed in their respective section.

### The Fragment Shader

The fragment shader is the one responsible to indicate how the pixel will look like. It will retrieve data such as color to be used, texture vertices or normal from the vertex shader and use them to proceed with the computation.
The features related to the fragment shader will be detailed in the projects that will use them.

### Vertex Buffer Object

The vertex buffer object is what in the old computer graphics jargon would be called `retained mode`, in opposition to the immediate mode.
In immediate mode, as discussed in the previous section, the mechanism process at each vertex sent and the process to send the vertex was configured in code, in the application.
Within the retained mode the application configure objects once and send them to the graphics processing with all information ready, that means, all vertices are sent at once.
In OpenGL the vertex buffer object can store more than vertices, but let's keep it simple for the moment and keep the concept to the name.
The very first thing that needs to be done is to uncomment the OpenGL configuration for GLFW. Those commented block that contains the hint instructions in the intitSystem routine.
In the mainLoop routine, the instructions from glMatrixMode to glColor3f are no longer valid, they can be removed as well.
And, of course, the instructions from glBegin and glEnd need to go. Now you should have the same code as in the first project, it just opens the window.
The vertex buffer object is configured with data that are in contiguous memory area. Thus, the triangle to be displayed needs to be configured this way, with its vertices placed in an array as demonstrated in the snippet bellow:

```
            // The triangle vertices to be drawn. They are placed in a contiguous memory area
            GLfloat triangle_buffer_data[]={
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f, 1.0f, 0.0f
            };
```

This snippet was coded within the mailLoop routine, but outside the loop instruction, since it is there where the drawings are done. But it should not be coded there properly... we will look out to understand where better to code it in the future.

There are some required steps to configure a vertex buffer object in OpenGL. This means basically allocate resources in the OpenGL state machine and set up the state to be configured prior to sending the data to configure it.
Those steps are:
* Generate the Vertex Arrays *resources*, also called by *names* or *objects* in some literature. This is done with the command glGenVertexArrays;
* Bind a vertex array resource to the current OpenGL vertex array with the command glBindVertexArray;
* Generate the buffer *resources*, also called by *names* or *objects* in some literature. This is done with the command glGenBuffers;
* Bind a buffer resource to the current OpenGL buffer with the command glBindBuffer, specifying that it is an array buffer using the constant GL_ARRAY_BUFFER;
* Specify the buffer where the data are contained using glBufferData using the following parameters:
    * Buffer Type: This indicates to OpenGL what is the buffer type to be informed. It is desirable that this buffer type be the same as the current bound buffer, GL_ARRAY_BUFFER, in this example;
    * Number of bytes: How many bytes are there in the buffer. In this case there are 36 bytes, because each vertex is a float (4 bytes) and there are 9 data, 3 components (x, y, z) in each of the 3 vertices (9*4=36). In the code a shortcut was used with the instruction sizeof(buffer), that gives us exactly how many bytes in the buffer there is. This is useful since sometimes there are no other ways to know how many bytes there are in a buffer;
    * The buffer: This is the proper buffer;
    * Processing Type: This is a constant indicating to OpenGL how to better treat that data. In this example the buffer is used to pass information to the processing pipeline (draw) and does not change (static), so it is configured as GL_STATIC_DRAW.

All this is configured in the mainLoop, previous to the loop start.
Now, the vertex buffer is configured and bound to a vertex array. It can be used to draw.

### Drawing with vertex buffer objects

There are some steps to proceed to the drawing of the vertex buffer:

* Enable a vertex attrib array in the OpenGL state machine so it can proceed with processing a vertex array. For now we will use the instruction glEnableVertexAttribArray(0), where 0 is the index of the vertex attrib array;
* Bind the buffer to be used. Although in this example this instruction should not be necessary, since the last bound vertex buffer is the only vertex buffer used, it is present just for sake of safety and education. If another vertex buffer had been configured and that was the last one, this instruction would be necessary, so, the instruction is exactly the same as the previous one written: glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
* Configure how the vertices in the buffer are to be processed: This is quite a tricky part, simple in this example, but will get some complexity in the next ones. The configuration is done with the commang *glVertexAttribPointer* with the following parameters:
    * The index of the enabled vertex attrib array to be used (*0*, in our case);
    * The number of data the buffer holds: there are 3 vertices, so this is the value to be informed in this parameter;
    * The type of data in the buffer (GL_FLOAT in the example);
    * Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed;
    * Stride specifies a byte offset after the consecutive number of data to be processed. In this case it is 0, since we have only the vertices;
    * Pointer specifies where in the buffer the data starts. In the example is 0.

The last two parameters will be better covered in the next example, the fourth parameter will be discussed later.

So, this is the configuration stage. The vertex attrib array is enabled, the vertex buffer is bound and the configurations for the vertex attrib to process the vertex buffer is done, all that remains is to draw the data. This is done calling glDrawArrays with the primitive, the start position and the number of data to be processed:

```
glDrawArrays(GL_TRIANGLES, 0, 3);
```

Right after the drawing, the VertexAttribArray was already used, so, it can be disabled to release the resource for other ones:

```
glDisableVertexAttribArray(0);
```

Some questions for you to experiment:

1. If the data we are setting is a static and to draw, do I really need to configure the vertex attrib array and set the vertex attrib pointer at each loop pass?
2. If such configuration (questioned in 1) can be done outside the loop, how to proceed with disabling the vertex attrib array? Does it need to be within the loop? Or outside? If outside, where?


Next: Coloring the plain triangle Immediate Mode
