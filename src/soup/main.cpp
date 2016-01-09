//////////////
// Includes //
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <functional>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "../lentil/core.h"
#include "../lentil/rend.h"
#include "../lentil/reso.h"

#include "translation.hpp"

//////////
// Code //

////
// Some config values.
const static double ROT_SPEED   = M_PI / 3;
const static double MAX_SPEED   = 1;
const static double ACCEL_SPEED = 1;
const static double MIN_SPEED   = 0.1;

const static glm::vec3 ACCEL_VECTOR = glm::vec3(ACCEL_SPEED, ACCEL_SPEED, ACCEL_SPEED);

////
// Handling program flags.

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

////
// Asset management.

// A template-library asset wrapper.
template <typename T>
class AssetWrapper {
private:
    std::function<void(T)> destructor;

    int* count;
    T t;

public:
    AssetWrapper(std::function<T(std::string)> constructor,
                 std::function<void(T)> destructor,
                 std::string path) {
        count = new int;
        (*count) = 1;
        t = constructor(path);
        this->destructor = destructor;
    }

    AssetWrapper(const AssetWrapper<T>& aw) {
        t = aw.getValue();
        count = aw.count;
        (*count)++;
    }

    virtual ~AssetWrapper() {
        if (--(*count) == 0) {
            destructor(t);
            delete count;
        }
    }

    // Getting the value.
    T getValue() const { return t; }
};

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
// Game updating and rendering stuff.

// Representing a model to render.
class ModelRender {
private:
    glm::vec3 position;
    glm::vec3 rotation;
    Texture texture;
    Shader shader;
    Model model;

    GLenum bufferType;

    std::vector<int> lengths;
    GLuint* vaos;
    GLuint* vbos;
    GLuint* ebos;

    // Buffering the model, position, and rotation into the OpenGL bufers.
    void buffer() throw(std::runtime_error) {
        Lentil_Core_Error err = Lentil_Core_defaultError();
        for (int i = 0; i < model.getValue()->groupsLength; i++) {
            lengths.push_back(Lentil_Rend_fillBuffers(model.getValue(), i, vaos[i], vbos[i], ebos[i], bufferType, &err));
            if (Lentil_Core_isError(err))
                throw std::runtime_error("Failed to buffer model group.");
        }
    }

public:
    // Constructing a new ModelRender.
    ModelRender(float x , float y , float z ,
                float xr, float yr, float zr,
                Texture texture, Shader shader, Model model,
                GLenum bufferType) :
            position(x , y , z),
            rotation(xr, yr, zr),
            texture(texture),
            shader(shader),
            model(model) {
        this->bufferType = bufferType;

        vaos = new GLuint[model.getValue()->groupsLength];
        vbos = new GLuint[model.getValue()->groupsLength];
        ebos = new GLuint[model.getValue()->groupsLength];

        glGenVertexArrays(model.getValue()->groupsLength, vaos);
        glGenBuffers(model.getValue()->groupsLength, vbos);
        glGenBuffers(model.getValue()->groupsLength, ebos);

        buffer();
    }

    // Constructing a ModelRender from another ModelRender.
    ModelRender(const ModelRender& mr) :
            position(mr.position),
            rotation(mr.rotation),
            texture(mr.texture),
            shader(mr.shader),
            model(mr.model) {
        vaos = new GLuint[model.getValue()->groupsLength];
        vbos = new GLuint[model.getValue()->groupsLength];
        ebos = new GLuint[model.getValue()->groupsLength];

        glGenVertexArrays(model.getValue()->groupsLength, vaos);
        glGenBuffers(model.getValue()->groupsLength, vbos);
        glGenBuffers(model.getValue()->groupsLength, ebos);

        buffer();
    }

    // Destroying a ModelRender.
    ~ModelRender() {
        glDeleteVertexArrays(model.getValue()->groupsLength, vaos);
        glDeleteBuffers(model.getValue()->groupsLength, vbos);
        glDeleteBuffers(model.getValue()->groupsLength, ebos);
    }

    // Performing a render with the buffers.
    void render(GLFWwindow* window, glm::vec3 position, glm::vec3 rotation) {
        // Binding the shader.
        glUseProgram(shader.getValue());

        // Binding the texture.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getValue());
        glBindFragDataLocation(shader.getValue(), 0, "out_color");

        // Setting the scale and texture location.
        glUniform3f(glGetUniformLocation(shader.getValue(), "camera_position"),
                    position.x, position.y, position.z);

        glUniform2f(glGetUniformLocation(shader.getValue(), "cam_rotation"),
                    rotation.x, rotation.y);

        // THIS IS SO INEFFICIENT WHY DON'T I JUST DO THIS IN A RESIZE CALLBACK
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glUniform2f(glGetUniformLocation(shader.getValue(), "window_size"), width, height);

        glUniform1f(glGetUniformLocation(shader.getValue(), "scale"), 0.5);

        glUniform1i(glGetUniformLocation(shader.getValue(), "tex"), 0);

        // Iterating through the (array) buffer objects and performing renders.
        GLuint pattr, tattr;
        for (int i = 0; i < model.getValue()->groupsLength; i++) {
            glBindVertexArray(vaos[i]);
            glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);

            pattr = glGetAttribLocation(shader.getValue(), "pvert");
            glEnableVertexAttribArray(pattr);
            glVertexAttribPointer(pattr, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

            tattr = glGetAttribLocation(shader.getValue(), "tvert");
            glEnableVertexAttribArray(tattr);
            glVertexAttribPointer(tattr, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));

            glDrawElements(
                GL_TRIANGLES,
                lengths[i],
                GL_UNSIGNED_INT,
                (void*)0
            );
        }
    }
};

// Representing a game to update.
class Game {
private:
    std::vector<ModelRender*> renders;
    TransVec position, rotation;

public:
    // Constructing a new game.
    Game() :
            position(ACCEL_SPEED,
                     MAX_SPEED, MIN_SPEED,
                     false,
                     GLFW_KEY_D, GLFW_KEY_A,
                     GLFW_KEY_Q, GLFW_KEY_E,
                     GLFW_KEY_S, GLFW_KEY_W),

            rotation(ROT_SPEED,
                     ROT_SPEED, MIN_SPEED,
                     true,
                     GLFW_KEY_UP,   GLFW_KEY_DOWN,
                     GLFW_KEY_LEFT, GLFW_KEY_RIGHT,
                     -1,            -1) { }

    // Destroying a game.
    ~Game() {
        for (auto it = renders.begin(); it != renders.end(); it++)
            delete (*it);
    }

    // Adding a given model render to the game.
    void addModelRender(ModelRender* mr) { renders.push_back(mr); }

    // Updating the game.
    void update(GLFWwindow* window, double dt) {
        position.update(window, dt, rotation.getVec());
        rotation.update(window, dt);
    }

    // Rendering the game.
    void render(GLFWwindow* window) {
        for (auto it = renders.begin(); it != renders.end(); it++)
            (*it)->render(window, position.getVec(), rotation.getVec());
    }
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
    double lt = 0.0, ct = 0.0, dt = 0.0;
    Game game;
    game.addModelRender(new ModelRender(0, 0, 0,
                                        0, 0, 0,
                                        texture, shader, model,
                                        GL_STATIC_DRAW));

    while (!glfwWindowShouldClose(window)) {
        lt = ct;
        ct = glfwGetTime();
        dt = ct - lt;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.update(window, dt);
        game.render(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Closing everything down.
    destroy(window);

    return 0;
}
