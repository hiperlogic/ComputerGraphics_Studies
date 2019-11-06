#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <string>
#include <vector>
#include <optional.hpp>
#include <cstring>
#include <exception>
#include <algorithm>


using namespace std;

namespace tr2 = std::experimental;
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



typedef struct QueueFamilyIndices {
    tr2::optional<uint32_t> graphicsFamily;

    bool isComplete(){
        return graphicsFamily.has_value();
    }
} QueueFamilyIndices;

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
            cleanup();
        }

        void set_height(int new_height){
            if(this->is_running) return;
            this->win_height = new_height;
        }

        void set_width(int new_width){
            if(this->is_running) return;
            this->win_width = new_width;
        }

        void listExtensionsAndLayers(){

            // Stores how many extensions are avaible in the instance for the basic Vulkan implementation
            uint32_t extensionCount=0;
            // retrieves the extension count
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

            // Stores the extension details
            std::vector<VkExtensionProperties> extensions2(extensionCount);
            // Query the extension details for the basic Vulkan implementation (nullptr)
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions2.data());

            std::cout << "available extensions: "<<std::endl;

            for(const auto& extension : extensions2){
                std::cout << "\n" << extension.extensionName << std::endl;
            }

            uint32_t layerCount;

            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            std::cout << "available layer properties: "<<std::endl<<std::endl;

            for(const auto& layer : availableLayers){
                std::cout << "##########################################################"<<std::endl;
                std::cout << "\n" << layer.layerName << std::endl;
                std::cout << "----------------------------------------------------------"<<std::endl;
                std::cout << layer.description<<std::endl;
                std::cout << "==========================Extensions======================"<<std::endl;
                uint32_t layerExtensionCount = 0;
                std::vector<VkExtensionProperties> layerExtensions(layerExtensionCount);
                vkEnumerateInstanceExtensionProperties(layer.layerName, &layerExtensionCount, nullptr);
                vkEnumerateInstanceExtensionProperties(layer.layerName, &layerExtensionCount, layerExtensions.data());
                for(const auto& layerExt: layerExtensions){
                    std::cout << layerExt.extensionName << std::endl;
                }
            }

        }

    private:
        GLFWwindow* window;
        int win_width = 1024;
        int win_height = 768;
        string win_title = "First Vulkan Window\n";
        bool is_running;
        VkInstance instance; // Stores the Vulkan Instance
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice;
        VkDevice    device;
        VkQueue graphicsQueue;
        VkSurfaceKHR surface;

        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif


        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr) {
                return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
            } else {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
        }

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance, debugMessenger, pAllocator);
            }
        }

        static void error_callback(int error, const char* description){
            fprintf(stderr, "Error: %s\n", description);
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

        int initVulkan(){
            int result;
            try{
                result = createInstance();
            } catch(std::exception e){
                std::cout << e.what() << std::endl;
                return EXIT_FAILURE;
            }
            std::cout<<"Starting the Debugger"<<std::endl;
            setupDebugMessenger();
            setPhysicalDevice();
            createLogicalDevice();
            return result;
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

            if (enableValidationLayers) {
                DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
            }
            vkDestroyDevice(device, nullptr);
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);
            glfwDestroyWindow(window);
            glfwTerminate();
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

            // Required Structure to Create an Instance
            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            // Retrieve the required and (debug) available extensions
            auto extensions = getRequiredExtensions();

            // Adjust the data in the create structure
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
            if (enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();

                populateDebugMessengerCreateInfo(debugCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;


            } else {
                createInfo.enabledLayerCount = 0;
                createInfo.pNext = nullptr;
            }

            // Create the instance with the info and no allocation callbacks, storing it in the instance handler
            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                throw std::runtime_error("failed to create instance!");
            }

            listExtensionsAndLayers();

            return EXIT_SUCCESS;
        }

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
            createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
        }

        void setupDebugMessenger() {
            if (!enableValidationLayers) return;
            std::cout<<"Debug Started"<<std::endl;

            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);

            if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
                throw std::runtime_error("failed to set up debug messenger!");
            }
        }        


        std::vector<const char*> getRequiredExtensions(){
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            // Retrieve the required surface extensions and the extensioncount
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            // Instantiate and initialize the return variable
            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            if(enableValidationLayers){
                // Only include the debug extension when debugging!
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
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




    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }


    void setPhysicalDevice(){
        // Store the number of physical devices
        uint32_t deviceCount=0;
        // Retrieves the number of physical devices available via instance
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        // Throw exception if no Vulkan supported devices were found
        if (deviceCount == 0) {
            throw std::runtime_error("No Vulkan supported GPU found!");
        }

        // Store the devices
        std::vector<VkPhysicalDevice> devices(deviceCount);
        // Retrieve the devices enumeration within the instance and stores them in the address pointed by devices.data()
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        // Will continue
        for (const auto& device : devices) {
            if (isDeviceSuitable(device)){  // How to know it is suitable?
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE){
            throw std::runtime_error("Failed to retrieve a suitable Graphics device!");
        }
    }

    void createLogicalDevice() {
        QueueFamilyIndices indices = probeQueueFamilies(physicalDevice);
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo createInfo = {}; // Yes, it is the same structure as used in createInstance!
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        // Set the address of the start of the contiguous memory queues definitions
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        // And how many queues are specified
        createInfo.queueCreateInfoCount = 1;

        // No extension count enabled
        createInfo.enabledExtensionCount = 0;

        // And let's check out the validation layers
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS){
            throw std::runtime_error("Failed to create the logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }

    void createSurface() {
        // Luckily glfw configures everything needed for cross platform Vulkan Surface creation
        if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface!");
        }
    }

    bool isDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        QueueFamilyIndices indices = probeQueueFamilies(device);
        return indices.isComplete();
    }

    QueueFamilyIndices probeQueueFamilies(VkPhysicalDevice device){
        QueueFamilyIndices indices;

        // How many families are there for the device?
        uint32_t queueFamilyCount=0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        // Retrieve the QueueFamilies properties so they can be checked for the needed features
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for(const auto& queueFamily : queueFamilies){
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = i;
            }
            if(indices.isComplete()){
                break;
            }
            i++;
        }

        return indices;
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
