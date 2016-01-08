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

// Representing a camera in 3d space.
struct Camera {
    glm::vec3 position;
    glm::vec3 rotation;
};

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
    void render(GLFWwindow* window, Camera cam) {
        // Binding the shader.
        glUseProgram(shader.getValue());

        // Binding the texture.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getValue());
        glBindFragDataLocation(shader.getValue(), 0, "out_color");

        // Setting the scale and texture location.
        glUniform3f(glGetUniformLocation(shader.getValue(), "camera_position"),
                    cam.position.x, cam.position.y, cam.position.z);

        glUniform2f(glGetUniformLocation(shader.getValue(), "cam_rotation"),
                    cam.rotation.x, cam.rotation.y);

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
    Camera cam;

    glm::vec3 speed;

    // Constructing a single direction.
    double direction(GLFWwindow* window, double axis, int pkey, int nkey) const {
        bool p = glfwGetKey(window, pkey) == GLFW_PRESS,
             n = glfwGetKey(window, nkey) == GLFW_PRESS;

        if (p && n) { }
        else if (p) {
            if (axis < -MIN_SPEED)
                return 1.7;
            return 1.0;
        } else if (n) {
            if (axis >  MIN_SPEED)
                return -1.7;;
            return -1.0;
        }

        if (axis >  MIN_SPEED)
            return -1.0;
        if (axis < -MIN_SPEED)
            return  1.0;

        return 0.0;
    }

    // Constructing a direction vector.
    glm::vec3 directionVector(GLFWwindow* window, glm::vec3 speed) const {
        return glm::vec3(direction(window, speed.x, GLFW_KEY_D, GLFW_KEY_A),
                         direction(window, speed.y, GLFW_KEY_Q, GLFW_KEY_E),
                         direction(window, speed.z, GLFW_KEY_W, GLFW_KEY_S));
    }

    // Constructing the rotational matrix for movement.
    glm::mat3 rotationMatrix(glm::vec3 rotation) const {
        return glm::mat3(


        );
    }

    // Capping the speed (and providing a minimum).
    glm::vec3 capSpeed(glm::vec3 speed, glm::vec3 dir) const {
        glm::vec3 capped;

        if (speed.x > -MIN_SPEED && speed.x < MIN_SPEED)
            capped.x = 0;
        else if (speed.x >  MAX_SPEED)
            capped.x =  MAX_SPEED;
        else if (speed.x < -MAX_SPEED)
            capped.x = -MAX_SPEED;
        else
            capped.x = speed.x;

        if (speed.y > -MIN_SPEED && speed.y < MIN_SPEED)
            capped.y = 0;
        else if (speed.y >  MAX_SPEED)
            capped.y =  MAX_SPEED;
        else if (speed.y < -MAX_SPEED)
            capped.y = -MAX_SPEED;
        else
            capped.y = speed.y;

        if (speed.z > -MIN_SPEED && speed.z < MIN_SPEED)
            capped.z = 0;
        else if (speed.z >  MAX_SPEED)
            capped.z =  MAX_SPEED;
        else if (speed.z < -MAX_SPEED)
            capped.z = -MAX_SPEED;
        else
            capped.z = speed.z;

        return capped;
    }

public:
    // Constructing a new game.
    Game() {
        cam.position = glm::vec3(0);
        cam.rotation = glm::vec3(0);
    }

    // Destroying a game.
    ~Game() {
        for (auto it = renders.begin(); it != renders.end(); it++)
            delete (*it);
    }

    // Adding a given model render to the game.
    void addModelRender(ModelRender* mr) { renders.push_back(mr); }

    // Updating the game.
    void update(GLFWwindow* window, double dt) {
        // Translating the camera.
        glm::vec3 dir = directionVector(window, speed);
        speed += ACCEL_VECTOR * dir * glm::vec3(dt) * rotationMatrix(cam.rotation);
        speed = capSpeed(speed, dir);

        cam.position += speed;

        // Rotating the camera.
        glm::vec3 drotation;
        bool u = glfwGetKey(window, GLFW_KEY_UP)   == GLFW_PRESS,
             d = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
        if (u && d) { }
        else if (u)
            drotation.x = -ROT_SPEED;
        else if (d)
            drotation.x =  ROT_SPEED;

        bool l = glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS,
             r = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
        if (l && r) { }
        else if (l)
            drotation.y = -ROT_SPEED;
        else if (r)
            drotation.y =  ROT_SPEED;

        cam.rotation += drotation * (float)dt;
        if (cam.rotation.x > 2 * M_PI)
            cam.rotation.x -= 2 * M_PI;
        if (cam.rotation.x < 0)
            cam.rotation.x += 2 * M_PI;
    }

    // Rendering the game.
    void render(GLFWwindow* window) {
        for (auto it = renders.begin(); it != renders.end(); it++)
            (*it)->render(window, cam);
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
