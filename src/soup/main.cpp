//////////////
// Includes //
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/vec3.hpp>
#include <functional>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "../lentil/core.h"
#include "../lentil/reso.h"

//////////
// Code //

// Attempting to parse a single flag into a key/value pair.
std::tuple<std::string, std::string> processFlag(std::string flag) throw(std::logic_error) {
    if (flag[0] != '-' || flag[1] != '-')
        throw std::logic_error("Invalid flag prefix.");

    int seam;
    for (seam = 2; seam < flag.size(); seam++)
        if (flag[seam] == '=')
            break;

    if (seam == flag.size() - 1)
        throw std::logic_error("No flag value.");

    return std::make_tuple(flag.substr(1, seam - 2), flag.substr(seam, flag.size() - seam));
}

// A config to manage resource loading.
struct ResConfig {
    std::string texturePath;
    std::string shaderPath;
    std::string modelPath;

    // Constructing a ResConfig from the 
    ResConfig(int argc, const char** argv) {
        texturePath = "res/imgs/test.png";
        shaderPath = "res/shaders/test";
        modelPath = "res/models/test.obj";

        std::tuple<std::string, std::string> pair;
        std::string flag;
        for (int i = 1; i < argc; i++) {
            flag = std::string(argv[i]);
            try { pair = processFlag(flag); }
            catch (std::logic_error e) {
                std::cout << "Invalid flag, \"" << e.what() << "\":" << flag << std::endl;
                continue;
            }

            std::string key = std::get<0>(pair),
                        val = std::get<1>(pair);

            if (key == "texture") { texturePath = val; }
            if (key == "shader") { shaderPath = val; }
            if (key == "model") { modelPath = val; }
        }
    }
};

// A template-library asset wrapper.
template <typename T>
class AssetWrapper {
private:
    std::function<void(T)> destructor;

    T t;
    bool owns;

public:
    AssetWrapper(std::function<T(std::string)> constructor,
                 std::function<void(T)> destructor,
                 std::string path) {
        t = constructor(path);
        this->destructor = destructor;
        owns = true;
    }

    virtual ~AssetWrapper() {
        if (owns)
            destructor(t);
    }

    AssetWrapper(const AssetWrapper& aw) {
        t = getValue();
        owns = false;
    }

    // Getting the value.
    T getValue() { return t; }

    // Checking if this class is the owner.
    bool isOwner() { return owns; }
};

////
// Applying the AssetWrapper to a number of types within the program.

// Textures
class Texture : public AssetWrapper<GLuint> {
public:
    Texture(std::string path) :
            AssetWrapper([](std::string path) -> GLuint {
                Lentil_Core_Error err = Lentil_Core_defaultError();
                GLuint texture = Lentil_Reso_loadTexture(path.c_str(), &err);
                if (Lentil_Core_isError(err))
                    throw std::runtime_error("Failed to load texture: " + path);

                return texture;
            },

            [](GLuint texture) {
                glDeleteTextures(1, &texture);
            }, path) { }
};

// Shaders
class Shader : public AssetWrapper<GLuint> {
public:
    Shader(std::string path) :
            AssetWrapper([](std::string path) -> GLuint {
                Lentil_Core_Error err = Lentil_Core_defaultError();
                GLuint shader = Lentil_Reso_loadShaderProgram(path.c_str(), &err);
                if (Lentil_Core_isError(err))
                    throw std::runtime_error("Failed to load shader: " + path);

                return shader;
            },

            [](GLuint shader) {
                glDeleteProgram(shader);
            }, path) { }
};

// Models
class Model : public AssetWrapper<Lentil_Reso_Model*> {
public:
    Model(std::string path) :
            AssetWrapper([](std::string path) -> Lentil_Reso_Model* {
                Lentil_Reso_Model* model = Lentil_Reso_Model_new();

                Lentil_Core_Error err = Lentil_Core_defaultError();
                Lentil_Reso_loadObjModelStr(path.c_str(), model, &err);
                if (Lentil_Core_isError(err))
                    throw std::runtime_error("Failed to load model: " + path);

                return model;
            },
            
            [](Lentil_Reso_Model* m) {
                Lentil_Reso_Model_destroy(m);           
            }, path) { }
};

////
// Actually running things.

// Initializing GLFW and OpenGL.
GLFWwindow* init() {
    // Initializing GLFW.
    if (!glfwInit()) {
        std::cout << "Initializing GLFW." << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Testing", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to construct GLFW window." << std::endl;

        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    // Initializing OpenGL.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW." << std::endl;

        glfwTerminate();
        return nullptr;
    }

    glClearColor(0.3, 0.3, 0.3, 1.0);
    
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glAlphaFunc(GL_NEVER, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glDepthFunc(GL_LEQUAL);

    Lentil_Core_debugLevel(3);

    return window;
}

// Destroying the GLFW window and terminating the program.
void destroy(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// The entry point to the program.
int main(int argc, const char** argv) {
    ResConfig rc(argc, argv);

    // Loading the window.
    GLFWwindow* window = init();

    // Loading the resources.
    Texture texture(rc.texturePath);
    Shader shader(rc.shaderPath);
    Model model(rc.modelPath);

    // Game setup & loop.
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Closing everything down.
    destroy(window);

    return 0;
}
