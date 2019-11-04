#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <exception>

using namespace std;
/*
    Includes the Vulkan API Headers.
*/
#include <vulkan/vulkan.h>

/*
    Includes the GLFW to manage the windows calls, surface rendering and events independent of
    operating system.
    Define the identifier to indicate GLFW to include Vulkan instructions
*/
#define GLFW_INCLUDE_VULKAN
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
            if( result == EXIT_FAILURE ){
                cleanup();
                return;
            }
            result = initVulkan();
            mainLoop();
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
        string win_title = "First Vulkan Window\n";
        bool is_running;
        VkInstance instance; // Stores the Vulkan Instance

        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        static void error_callback(int error, const char* description){
            fprintf(stderr, "Error: %s\n", description);
        }

        bool checkValidationLayerSupport(){
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            for (const char* layerName : validationLayers){
                bool layerFound = false;

                for(const auto& layerProperties : availableLayers){
                    if(strcmp(layerName, layerProperties.layerName)==0){
                        layerFound = true;
                        break;
                    }
                }
                if(!layerFound){
                    return false;
                }
            }
            return true;
        }

        int createInstance(){
            // Check if the validation layer is available
            if (enableValidationLayers && !checkValidationLayerSupport()){
                throw std::runtime_error("validation layers requested, but not available!");
            }

            // Structure to Identify the Application
            VkApplicationInfo appInfo = {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Vulkan App Framework";
            appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            // Retrieves the extension to interface with the windows system (glfw provides the tools we need)
            // Number of extensions
            uint32_t glfwExtensionCount = 0;
            // Array of extension names
            const char** glfwExtensions;

            // Call to get the name of extensions. If the count is 0, an error occurred
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            
            if(glfwExtensionCount == 0) return EXIT_FAILURE;

            std::cout << "\nGLFW EXTENSIONS: \n";

            for(uint32_t i=0; i<glfwExtensionCount; i++){
                std::cout << "\n" << glfwExtensions[i] << std::endl;
            }

            // Required Structure to Create an Instance
            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            // Adjust the data in the create structure
            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;

            if (enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            } else {
                createInfo.enabledLayerCount = 0;
            }

            // Create the instance with the info and no allocation callbacks, storing it in the instance handler
            VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

            // Stores how many extensions are avaible in the instance for the basic Vulkan implementation
            uint32_t extensionCount=0;
            // retrieves the extension count
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

            // Stores the extension details
            std::vector<VkExtensionProperties> extensions(extensionCount);
            // Query the extension details for the basic Vulkan implementation (nullptr)
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            std::cout << "available extensions: "<<std::endl;

            for(const auto& extension : extensions){
                std::cout << "\n" << extension.extensionName << std::endl;
            }

            uint32_t layerCount;

            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            std::cout << "available layer properties: "<<std::endl;

            for(const auto& layer : availableLayers){
                std::cout << "\n" << layer.layerName << std::endl;
                std::cout << "----------------------------------------------------------"<<std::endl;
                std::cout << layer.description<<std::endl;
                std::cout << "=========================================================="<<std::endl;
            }


            return EXIT_SUCCESS;
        }

        int initVulkan(){
            int result;
            try{
                result = createInstance();
            } catch(std::exception e){
                std::cout << e.what() << std::endl;
                return EXIT_FAILURE;
            }
            return result;
        }

        int initSystem(){
            glfwSetErrorCallback( WindowAppWrapper::error_callback );
            if(!glfwInit()){
                std::cerr << "GLFW Failed to initialize" <<std::endl;
                return EXIT_FAILURE;
            }

            // Set no API to the glfw window
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            this->window = glfwCreateWindow(this->win_width, this->win_height, this->win_title.c_str(), NULL, NULL);
            if( this->window == NULL ){
                std::cerr << "GLFW Failed to initialize" << std::endl;
                glfwTerminate();
                return EXIT_FAILURE;
            }
            std::cout<<"Window Created"<<std::endl;
            return EXIT_SUCCESS;
        }

        void mainLoop(){
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);   

            do{
                 // Draw nothing, see you in tutorial 2 !

                 glfwPollEvents();

            } // Check if the ESC key was pressed or the window was closed
            while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                glfwWindowShouldClose(window) == 0 );
        }

        void cleanup(){
            vkDestroyInstance(instance, nullptr);
            glfwDestroyWindow(window);
            glfwTerminate();
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
