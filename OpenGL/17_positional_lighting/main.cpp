#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;
/*
    Includes the Glew (OpenGL Extension Wrangler) in order to verify and use the most recent OpenGL
    implementations. This must be included prior to any library that uses OpenGL.
*/
#include <GL/glew.h>

/*
    Includes the GLFW to manage the windows calls, surface rendering and events independent of
    operating system.
*/
#include <GLFW/glfw3.h>

/*
    Includes the routines for matrices manipulation. These are essential for computer graphics
    operations.
*/
#include <glm/glm.hpp>
using namespace glm;

#include <loadShader.hpp>

/*
    WindowAppWrapper implements the basic system to initialize resources, process calls (or idle) 
    and perform the cleanup of the resources.
    This class will be improved within the course of the tutorials.
    Improvements Added:
        Class created
*/
class WindowAppWrapper {
    public:

        WindowAppWrapper(){
            this->is_running = 0;
        }

        ~WindowAppWrapper(){
        }

        void run(){
            this->is_running = 1;
            int result = initSystem();
            if( result != EXIT_FAILURE )
                mainLoop();
            cleanup();
            this->is_running = 0;
        }

        void set_height(int new_height){
            if(this->is_running) return;
            this->win_height = new_height;
        }

        void set_width(int new_width){
            if(this->is_running) return;
            this->win_width = new_width;
        }

    private:
        GLFWwindow* window;
        int win_width = 1024;
        int win_height = 768;
        string win_title = "First OpenGL Window\n";
        bool is_running;

        static void error_callback(int error, const char* description){
            fprintf(stderr, "Error: %s\n", description);
        }

        int initSystem(){
            glfwSetErrorCallback( WindowAppWrapper::error_callback );
            if(!glfwInit()){
                std::cerr << "GLFW Failed to initialize" <<std::endl;
                return EXIT_FAILURE;
            }
            
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            this->window = glfwCreateWindow(this->win_width, this->win_height, this->win_title.c_str(), NULL, NULL);
            if( this->window == NULL ){
                std::cerr << "GLFW Failed to initialize" << std::endl;
                glfwTerminate();
                return EXIT_FAILURE;
            }

            glfwMakeContextCurrent(this->window);
            glewExperimental = true;

            if (glewInit() != GLEW_OK) {
                std::cerr << "GLEW Failed to initialize" <<std::endl;
                glfwTerminate();
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        }

        // Create the shader type, load the source and compile it.
        GLuint createShader(GLchar* shader_name, GLenum shaderType){
            GLuint shader = glCreateShader(shaderType);
            string shaderSource = LoadShader(string(shader_name));
            const char* str_p = shaderSource.c_str();
            const char* const* sources = &str_p;
            glShaderSource(shader, 1, sources, nullptr);
            glCompileShader(shader);
            return shader;
        }

/*
        GLuint createShaders(GLchar** shader_names, GLuint numFiles, GLenum shaderType){
            GLuint shader = glCreateShader(shaderType);
            vector <string> shaderSources;
            for(int i=0; i<numFiles; i++){
                string shaderSource = LoadShader(string(shader_names[0]));

            }
            glShaderSource(shader, 1, &shaderSource.c_str, nullptr);
            glCompileShader(shader);
            return shader;
        }
*/

        void mainLoop(){
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);   

            // The triangle vertices to be drawn. They are placed in a contiguous memory area
            GLfloat triangle_buffer_data[]={
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f, 1.0f, 0.0f
            };

            GLuint vertexShader = createShader("vertex1.glsl", GL_VERTEX_SHADER);
            GLuint fragmentShader = createShader("pixel1.glsl", GL_FRAGMENT_SHADER);

            GLuint shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);

            glLinkProgram(shaderProgram);
            glUseProgram(shaderProgram);

            glDetachShader(shaderProgram, vertexShader);
            glDetachShader(shaderProgram, fragmentShader);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            GLuint recColor = glGetUniformLocation(shaderProgram, "recColor");

            // Stores the identifier of the vertex array in the OpenGL state machine
            GLuint vertexArrayID;
            // Generates and stores the vertex array index (or name)
            glGenVertexArrays(1, &vertexArrayID);
            // Sets the generated vertex array as the current in order to receive the data
            glBindVertexArray(vertexArrayID);

            // Store the vertex buffer identifier
            GLuint vertexbuffer;

            // Generate 1 buffer and store its identifier in the proper id
            glGenBuffers(1, &vertexbuffer);

            // Bind the current buffer to the buffer generated whose ID is stored in vertexbuffer
            // This buffer is an array buffer
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            // Set the current buffer data.
            // The data is provided as an array buffer
            // The size of the data (buffer) in memory
            // The memory address where the buffer starts
            // The data is static (not streamed nor dynamic. It does not change) and to be drawn (not to be read or copy)
            cout<<"Size: "<<sizeof(triangle_buffer_data)<<"divided by floar: "<<sizeof(triangle_buffer_data)/sizeof(GLfloat);
            glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_buffer_data), triangle_buffer_data, GL_STATIC_DRAW);

            float colors[] = {1.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.8f, 1.0f, 1.0f, 0.0f}; //Red, somewhat violet and yellow
            int corrCol = 1; // The somewhat violet is the color of the previous code

            // Configure the clear color as we already have learned
            glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

            do{
                // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
                glClear( GL_COLOR_BUFFER_BIT );

                glUniform4f(recColor, colors[corrCol*3], colors[corrCol*3+1],colors[corrCol*3+2], 1.0f);

                // Draw a White (current color) triangle!
                //Enable the vertex attribute array of index 0
                glEnableVertexAttribArray(0);
                // Bind the current buffer to the vertex buffer
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                // Set the buffer attributions
                glVertexAttribPointer(
                    0,          // Whith vertex attrib array index to use
                    3,          // How many data does the buffer holds
                    GL_FLOAT,    // What kind of data the buffer holds
                    GL_FALSE,    //  Is the data normalized?
                    0,           // Do not consider stride
                    (void*)0    // No offset to be used
                );
                // Send the command to draw the data from the configured buffers
                // Consider the triangle as the primitive to process
                // Starts with the data at position 0
                // Consider 3 data to use
                glDrawArrays(GL_TRIANGLES, 0, 3);

                // The OpenGL resource for the vertex attrib array with index 0 has already been processed
                // It can be deactivated for future uses.
                glDisableVertexAttribArray(0);
                // Swap buffers
                glfwSwapBuffers(window);
                glfwPollEvents();

                if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
                    corrCol = (corrCol+1)%3;
                    cout<<"Space pressed"<<std::endl;
                }


            } // Check if the ESC key was pressed or the window was closed
            while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                glfwWindowShouldClose(window) == 0 );
        }

        void cleanup(){

        }
};

int main() {
    WindowAppWrapper *app = new WindowAppWrapper();

    try {
        app->run();
    } catch (const std::exception& e){
        std::cerr <<e.what() <<std::endl;
        delete app;
        return EXIT_FAILURE;
    }

    delete app;
    return EXIT_SUCCESS;
}