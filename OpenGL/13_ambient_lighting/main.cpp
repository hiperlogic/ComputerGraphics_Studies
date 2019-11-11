#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <string>
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

/*             
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 */
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

        void mainLoop(){
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);   

            // Tell OpenGL state machine which matrix we will configure
            glMatrixMode(GL_PROJECTION);
            // Set the matrix to the identity
            glLoadIdentity();
            // Configure the matrix acording to the orthographic view
            // Homogeneous coordinates: Horizontal, Vertical and Depth (X,Y,Z) from -1.0 to 1.0
            glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0); 
            // Tell OpenGL we are no longer modifying the projection matrix, but the modelview matrix.
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            // Configure the clear color as we already have learned
            glClearColor(0.0, 0.0, 0.0, 0.0);

            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            float colors[] = {1.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.8f, 1.0f, 1.0f, 0.0f}; //Red, somewhat violet and yellow
            int corrCol = 1; // The somewhat violet is the color of the previous code
            glColor3f(colors[corrCol*3], colors[corrCol*3+1],colors[corrCol*3+2]);

            do{
                // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
                glClear( GL_COLOR_BUFFER_BIT );
                // Draw a White (current color) triangle!

                glColor3f(colors[corrCol*3], colors[corrCol*3+1],colors[corrCol*3+2]);

                glBegin(GL_TRIANGLES);
                    glVertex3f(-1.0,-1.0,0.0);
                    glVertex3f(1.0,-1.0,0.0);
                    float top[3] = {0.0,1.0,0.0};
                    glVertex3fv(top);
                glEnd();

                glFlush();
                // Swap buffers
                glfwSwapBuffers(window);
                glfwPollEvents();

                if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
                    corrCol = (corrCol+1)%3;
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