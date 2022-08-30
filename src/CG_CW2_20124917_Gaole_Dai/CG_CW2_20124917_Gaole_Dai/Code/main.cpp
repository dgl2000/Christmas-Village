/** 
* coursework entry x64
* @file main.cpp 
* @source https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/6.2.cubemaps_environment_mapping/cubemaps_environment_mapping.cpp
* @author Gaole Dai (20124917)
* @date 30/11/21
**/

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <cmath>
#include <iostream>
#pragma warning(disable: 4244)

// include the header for audio
#include <irrKlang/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")


// define functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);
void LightEffect(Shader shader, glm::vec3 pointLightPositions[], glm::vec3 pointLightColors[]);
void baseLightEffect(Shader shader, glm::vec3 lightPos);
void LightEffectNight(Shader shader, glm::vec3 pointLightPositions[], glm::vec3 pointLightColors[]);
void renderBase();

// set the basement VAO & VBO
unsigned int baseVAO = 0;
unsigned int baseVBO;
// set window width and height
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1000;
// set camera position
glm::vec3 initCameraPos = glm::vec3(0.0f, 8.0f, 65.0f);
Camera camera(initCameraPos);
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
// init variable for camera moving speed
float cameraSpeed = 1.0;
// init variable for timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float santaAnimationStart = 0.0f;
// init variable for whether the santa animation is playing
bool isPlaySantaAnimation = true;
int santaAnimationCounter = 0;
int santaMode = 0;
// init variables for camera auto moving mode
bool autoMode = false;
float autoModeDuration = 16.0f;
float autoModeTimer = 0.0f;
// init variables for night mode
bool isNight = false;

// define a PI constant
#define PI 3.1415926


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create glfw window
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG CW2: Christmas Village - Gaole Dai (20124917)", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // set windows icon
    GLFWimage images[1];
    images[0].pixels = stbi_load("Resources/win-icon.png", &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // compile shaders
    // ---------------
    Shader shader("Code/Shader/objectShader.vs", "Code/Shader/objectShader.fs");
    Shader skyboxShader("Code/Shader/skybox.vs", "Code/Shader/skybox.fs");

    // compile model objects
    Model treeDecoraredModel("Resources/Models/Tree-Decorated/Tree-Decorated.obj");
    Model treeBareModel("Resources/Models/Tree-Bare/Tree-Bare.obj");
    Model treeRoundModel("Resources/Models/Tree-Round/Tree-Round.obj");
    Model castleModel("Resources/Models/Castle/Castle.obj");
    Model santaModel("Resources/Models/Santa/Santa.obj");
    Model santaBendModel("Resources/Models/Santa-Bend/Santa-Bend.obj");
    Model santaSitModel("Resources/Models/Santa-Sit/Santa-Sit.obj");
    Model baseModel("Resources/Models/Base/Base.obj"); 
    Model houseModel("Resources/Models/House/House.obj");
    Model houseRedModel("Resources/Models/House-Red/HouseRed.obj");
    Model cloudModel("Resources/Models/Cloud/Cloud.obj");
    Model houseLightModel("Resources/Models/House-Light/House-Light.obj");
    Model bridgeModel("Resources/Models/Bridge/Bridge.obj");
    Model snowmanModel("Resources/Models/Snowman/Snowman.obj");
    Model sleighModel("Resources/Models/Sleigh/Sleigh.obj");
    Model fenceModel("Resources/Models/Fence/Fence.obj");
    Model candyCaneModel("Resources/Models/Candy-Cane/Candy-Cane.obj");
    Model candyCaneGreenModel("Resources/Models/Candy-Cane-Green/Candy-Cane-Green.obj");
    Model giftBlueModel("Resources/Models/Gift/Gift-Blue/Gift-Blue.obj");
    Model giftGRModel("Resources/Models/Gift/Gift-GR/Gift-GR.obj");
    Model giftGreenModel("Resources/Models/Gift/Gift-Green/Gift-Green.obj");
    Model giftPurpleModel("Resources/Models/Gift/Gift-Purple/Gift-Purple.obj");
    Model giftRedModel("Resources/Models/Gift/Gift-Red/Gift-Red.obj");
    Model woodWellModel("Resources/Models/Wood-Well/Wood-Well.obj");
    Model xmasModel("Resources/Models/Xmas/Xmas.obj");
    Model rockModel("Resources/Models/Rock/Rock.obj");
    Model snowModel("Resources/Models/Snow/Snow.obj");
    Model smokeModel("Resources/Models/Smoke/Smoke.obj");

    // load the texture for the lake
    unsigned int whiteSnowTexture = loadTexture("Resources/Textures/lake.png");

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(20.0f,  10.0f,  0.0f),
        glm::vec3(-2.0f, 5.0f, 40.0f),
        glm::vec3(0.0f, 10.0f, 15.0f)
    };

    // color of the point lights
    glm::vec3 pointLightColors[] = {
        glm::vec3(0.4f, 0.7f, 0.1f)
    };

    // define the positions of different objects
    // -----------------------------------------
    glm::vec3 treeRoundPosition[]{
        glm::vec3(42, 1, -2),
        glm::vec3(30, 1, 0),
        glm::vec3(35, 1, 2),
        glm::vec3(40, 0.5, 2),
        glm::vec3(48, -0.5, 0),
        glm::vec3(45, -0.5, -4),
        glm::vec3(50, -0.5, -5),
        glm::vec3(34, 0.5, -4),
        glm::vec3(45, -1.0, -10),
        glm::vec3(40, -1.0, -8),
        glm::vec3(38, -1.0, -20),
        glm::vec3(30, -1.0, -15),
        glm::vec3(40, -0.5, 10),
        glm::vec3(45, -0.5, 6),
        glm::vec3(48, -0.5, 4),
        glm::vec3(48, -0.5, 48),
        glm::vec3(45, -0.5, 40),

        glm::vec3(-35, 1, 8),
        glm::vec3(-25, 2, 5),
        glm::vec3(-30, 0.5, 15)
    };

    glm::vec3 treeRoundScale[]{
        glm::vec3(1.5, 1.5, 1.5),
        glm::vec3(1.2, 1.2, 1.2),
        glm::vec3(1.2, 1.2, 1.2),
        glm::vec3(2.0, 2.0, 2.0),
        glm::vec3(1.4, 1.4, 1.4),
        glm::vec3(1.8, 1.8, 1.8),
        glm::vec3(1.3, 1.3, 1.3),
        glm::vec3(1.6, 1.6, 1.6),
        glm::vec3(1.8, 1.8, 1.8),
        glm::vec3(1.9, 1.9, 1.9),
        glm::vec3(2.4, 2.4, 2.4),
        glm::vec3(2.7, 2.7, 2.7),
        glm::vec3(1.8, 1.8, 1.8),
        glm::vec3(1.2, 1.2, 1.2),
        glm::vec3(1.4, 1.4, 1.4),
        glm::vec3(2.1, 2.1, 2.1),
        glm::vec3(2.5, 2.5, 2.5),

        glm::vec3(2.7, 2.7, 2.7),
        glm::vec3(1.8, 1.8, 1.8),
        glm::vec3(1.2, 1.2, 1.2)
    };

    glm::vec3 treePosition[] = {
        glm::vec3(-15, 1, 15),
        glm::vec3(-20, 0, 20),
        glm::vec3(-30, 3, 23),

        glm::vec3(30, 0, -10),
        glm::vec3(35, 3, -25),
        glm::vec3(28, 3, -20),
        glm::vec3(12, 0, -15),
        glm::vec3(6, 2, -10),
        glm::vec3(15, 2, -8),
        glm::vec3(20, 0, -20),

        glm::vec3(8, 0, 0),
        glm::vec3(13, 0.5, -3),
        glm::vec3(17, 0, 3),
        glm::vec3(24, 0, 7),
        glm::vec3(25, 0, -5),
        glm::vec3(30, 0, 10),
        glm::vec3(26, -0.5, 47),
        glm::vec3(20, 0, 28),
        glm::vec3(45, 0, -25),
        glm::vec3(38, 0, -15),
        glm::vec3(37, 0, 28),
        glm::vec3(35, 0, 35),
        glm::vec3(35, 0, 30),
        glm::vec3(20, 0, 43),

        glm::vec3(-30, 4, 30),
        glm::vec3(-40, 5, 32),
        glm::vec3(-45, 3, 35),

        glm::vec3(-30, 3, -30),
        glm::vec3(-35, 2.5, -25),
        glm::vec3(-40, 2.5, -32)
    };

    glm::vec3 fencePosition[]{
        glm::vec3(-7, 2, -14),
        glm::vec3(-15, 0.8, 28),
        glm::vec3(6, 0, 30)
    };

    float fenceRotation[]{
         -65.0f,
          65.0f,
          40.0f
    };

    glm::vec3 rockPosition[]{
        glm::vec3(6, 0.5, 0),
        glm::vec3(4, 0.5, -1),
        glm::vec3(3.5, 0.5, 1),
        glm::vec3(-5, 0.5, 3.5),
        glm::vec3(-6, 0.5, 3),
        glm::vec3(-7, 1.0, 4.5),
        glm::vec3(30, 1.0, 20),
        glm::vec3(45, 1.0, 15),
        glm::vec3(-38, 3.0, 39)
    };

    glm::vec3 rockScale[]{
        glm::vec3(0.6, 0.6, 0.6),
        glm::vec3(0.4, 0.4, 0.4),
        glm::vec3(0.5, 0.5, 0.5),
        glm::vec3(0.4, 0.4, 0.4),
        glm::vec3(0.4, 0.4, 0.4),
        glm::vec3(0.3, 0.3, 0.3),
        glm::vec3(5.0, 5.0, 5.0),
        glm::vec3(6.0, 6.0, 6.0),
        glm::vec3(6.0, 6.0, 6.0)
    };

    glm::vec3 snowSlowerPosition[]{
        glm::vec3(0, -5, 40),
        glm::vec3(0, 0, 0),
        glm::vec3(30, 0, 0),
        glm::vec3(0, 0, 30),
        glm::vec3(-30, 0, 0),
        glm::vec3(0, 0, -30),
        glm::vec3(30, 0, 30),
        glm::vec3(-30, 0, 30),
        glm::vec3(-15, 0, 40),
        glm::vec3(30, 0, -30),
        glm::vec3(-30, 0, -30),
        glm::vec3(0, 20, 0),
        glm::vec3(30, 20, 0),
        glm::vec3(0, 20, 30),
        glm::vec3(-30, 20, 0),
        glm::vec3(0, 20, -30),
        glm::vec3(30, 20, 30),
        glm::vec3(-30, 20, 30),
        glm::vec3(30, 20, -30),
        glm::vec3(-30, 20, -30)
    };

    glm::vec3 snowQuickerPosition[]{
        glm::vec3(40, 10, 0),
        glm::vec3(0, 30, 40),
        glm::vec3(0, 20, 30),
        glm::vec3(-20, 25, 0),
        glm::vec3(0, 18, -30),
        glm::vec3(0, 19, 0),
        glm::vec3(24, 30, 32),
        glm::vec3(30, 30, 30),
        glm::vec3(0, 23, 40),
        glm::vec3(20, 14, 40)
    };

    glm::vec3 smokePosition[]{
        glm::vec3(-15.7, 8.0, 34.8),
        glm::vec3(10.5, 7.0, 30.5),
    };

    glm::vec3 treeDecoratedPosition[]{
        glm::vec3(-10, 0.5, -4),
        glm::vec3(-12, 0, 42),
        glm::vec3(10, 1, 39)
    };

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // initialize the audio engine
    ISoundEngine* audioEngine = createIrrKlangDevice();

    // start the sound engine with default parameters
    if (!audioEngine)
    {
        // error starting up the engine
        printf("Could not startup engine\n");
        return 0; 
    }

    // play some sound stream, looped
    audioEngine->play2D("Resources/Audio/jingle-bells.mp3", true);
    audioEngine->play2D("Resources/Audio/footsteps-on-concrete.mp3", true);

    // load textures
    // -------------
    vector<std::string> faces
    {
        "Resources/Textures/Skybox/right.png",
        "Resources/Textures/Skybox/left.png",
        "Resources/Textures/Skybox/top.png",
        "Resources/Textures/Skybox/bottom.png",
        "Resources/Textures/Skybox/front.png",
        "Resources/Textures/Skybox/back.png",
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // define the cloud animation duration
    float cloudAnimationDuration = 60.0;
    // initialize the cloud animation rotate angle
    float cloudAnimationRotate = 0.0;
    
    // define variables for the snowman animation
    float snowmanAnimationDuration = 15.0;
    float snowmanAnimationRotate = 0.0;
    float snowmanAnimationTranslate = 4 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0));
    // define variables for the slow snow animation
    float snowSlowerAnimationDuration = 30.0;
    float snowSlowerAnimationTranslate = 0.0;
    // define variables for the quick snow animation
    float snowQuickerAnimationDuration = 15.0;
    float snowQuickerAnimationTranslate = 0.0;
    // define variables for the smoke animation
    float smokeAnimationDuration = 12.0;
    float smokeAnimationTranslate = 0.0;
    float smokeScale = 0.6;
    // define variables for the quick smoke animation
    float smokeQuickerAnimationDuration = 8.0;
    float smokeQuickerAnimationTranslate = 0.0;
    // define variables for the santa claus animation
    float santaAnimationDuration = 18.0;
    float santaAnimationTranslationX = 0.0;
    float santaAnimationTranslationZ = 0.0;
    float santaAnimationTranslationY = 0.0;
    float santaAnimationRotation = 0.0;
    float sleighAnimationTranslationX = 0.0;
    float sleighAnimationTranslationZ = 0.0;
    float sleighAnimationTranslationY = 0.0;
    float sleighAnimationRotation = 0.0;
    float giftAnimationTranslationX = 0.0;
    float giftAnimationTranslationY = 0.0;
    float giftAnimationTranslationZ = 0.0;
    float giftAnimationRotation = 0.0;


    float timer = 0.0;

    // render loop
    // --------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // calculate the time duration
        // ---------------------------------------------------------------------------------------
        // substruct the loading and render time
        if (deltaTime > 1.0) {
            deltaTime = 0.0;
        }
        timer += deltaTime;
        santaAnimationStart += deltaTime;
        
        // cloud animation
        // ---------------------------------------------------------------------------------------
        if (fmod(timer, cloudAnimationDuration) == 0.0) {
            cloudAnimationRotate = 0.0f;
        }
        else {
            float timeInterval = fmod(timer, cloudAnimationDuration);
            cloudAnimationRotate = 360 * timeInterval / cloudAnimationDuration;
        }
        // snowman animation
        // ---------------------------------------------------------------------------------------
        if (fmod(timer, snowmanAnimationDuration) < 0.01) {
            snowmanAnimationRotate = 0.0f;
            snowmanAnimationTranslate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0));
        }
        else {
            float timeInterval = fmod(timer, snowmanAnimationDuration);
            snowmanAnimationRotate = 360 * timeInterval / snowmanAnimationDuration;
        }
        // snow animation move slower
        // ---------------------------------------------------------------------------------------
        if (fmod(timer, snowSlowerAnimationDuration) == 0.0) {
            snowSlowerAnimationTranslate = 0.0;
        }
        else {
            float timeInterval = fmod(timer, snowSlowerAnimationDuration);
            snowSlowerAnimationTranslate = -30 * timeInterval / snowSlowerAnimationDuration;
        }

        // snow animation move quicker
        // ---------------------------------------------------------------------------------------
        if (fmod(timer, snowQuickerAnimationDuration) == 0.0) {
            snowQuickerAnimationTranslate = 0.0;
        }
        else {
            float timeInterval = fmod(timer, snowQuickerAnimationDuration);
            snowQuickerAnimationTranslate = -20 * timeInterval / snowQuickerAnimationDuration;
        }

        // smoke animation move slower
        // ---------------------------------------------------------------------------------------
        if (fmod(timer, smokeAnimationDuration) == 0.0) {
            smokeAnimationTranslate = 0.0;
            smokeScale = 0.6;
        }
        else {
            float timeInterval = fmod(timer, smokeAnimationDuration);
            smokeAnimationTranslate = 5.0 * timeInterval / smokeAnimationDuration;
            smokeScale = 0.6 - 0.5 * timeInterval / smokeAnimationDuration;
        }

        // smoke animation move quicker
        // ---------------------------------------------------------------------------------------
        if (fmod(timer, smokeQuickerAnimationDuration) == 0.0) {
            smokeQuickerAnimationTranslate = 0.0;
        }
        else {
            float timeInterval = fmod(timer, smokeQuickerAnimationDuration);
            smokeQuickerAnimationTranslate = 4.0 * timeInterval / smokeQuickerAnimationDuration;
        }

        // santa claus animation
        // ---------------------------------------------------------------------------------------
        if (isPlaySantaAnimation) {
            // santa jump up to the sleigh
            if (fmod(santaAnimationStart, santaAnimationDuration) <= 1.0) {
                float timeInterval = fmod(santaAnimationStart, santaAnimationDuration);
                sleighAnimationRotation = 0.0;
                santaAnimationTranslationZ = 2.0 * timeInterval / 1.0;
                santaAnimationTranslationY = 2.0 * timeInterval / 1.0;
                santaAnimationRotation = 45.0 * timeInterval / 1.0;
            }
            // santa sit on the sleigh
            else if (fmod(santaAnimationStart, santaAnimationDuration) <= 2.0) {
                float timeInterval = fmod(santaAnimationStart, santaAnimationDuration);
                santaAnimationTranslationZ = 2.0 + 2.0 * (timeInterval -1.0) / 1.0;
                santaAnimationTranslationY = 2.0 - 1.5 * (timeInterval - 1.0) / 1.0;
                santaAnimationRotation = 45.0 + 45.0 * (timeInterval - 1.0) / 1.0;
                santaMode = 1;
            }
            // santa and sleigh turn a circle and move up
            else if (fmod(santaAnimationStart, santaAnimationDuration) <= 7.0) {
                float timeInterval = fmod(santaAnimationStart, santaAnimationDuration);
                santaAnimationTranslationZ = 4.0;
                santaAnimationTranslationY = 0.5 + 2.0 * (timeInterval - 2.0) / 5.0;
                santaAnimationRotation = 90.0 + 270 * (timeInterval - 2.0) / 5.0;
                sleighAnimationRotation = 270 * (timeInterval - 2.0) / 5.0;
                sleighAnimationTranslationY = 2.0 * (timeInterval - 2.0) / 5.0;
                if (santaAnimationCounter == 0) {
                    giftAnimationTranslationY = 2.0 * (timeInterval - 2.0) / 5.0;
                    giftAnimationRotation = 270 * (timeInterval - 2.0) / 5.0;
                }
                santaMode = 2;
            }
            // santa and sleigh move forward
            else if (fmod(santaAnimationStart, santaAnimationDuration) <= 12.0) {
                float timeInterval = fmod(santaAnimationStart, santaAnimationDuration);
                santaAnimationTranslationX = 4.0 * (timeInterval - 7.0) / 5.0;
                santaAnimationTranslationZ = 4.0 + 8.0 * (timeInterval - 7.0) / 5.0;
                santaAnimationTranslationY = 2.5 + 3.0 * (timeInterval - 7.0) / 5.0;
                santaAnimationRotation = 360.0;
                sleighAnimationTranslationX = 4.0 * (timeInterval - 7.0) / 5.0;
                sleighAnimationTranslationZ = 8.0 * (timeInterval - 7.0) / 5.0;
                sleighAnimationRotation = 270.0;
                sleighAnimationTranslationY = 2.0 + 3.0 * (timeInterval - 7.0) / 5.0;
                // santa throw a gift if user first play the animation
                if (santaAnimationCounter == 0) {
                    if (timeInterval <= 8.0) {
                        giftAnimationTranslationY = 2.0 + 2.0 * (timeInterval - 7.0) / 1.0;
                    }
                    else if (timeInterval >= 8.5) {
                        giftAnimationTranslationY = 4.0 - 4.0 * (timeInterval - 8.5) / 3.5;
                    }
                    giftAnimationTranslationX = 12.0 * (timeInterval - 7.0) / 5.0;
                    giftAnimationTranslationZ = 8.0 * (timeInterval - 7.0) / 5.0;
                }

            }
            // santa and sleigh move back to the initial position
            else if (fmod(santaAnimationStart, santaAnimationDuration) <= 17.0){
                float timeInterval = fmod(santaAnimationStart, santaAnimationDuration);
                santaAnimationTranslationX = 4.0 - 4.0 * (timeInterval - 12.0) / 5.0;
                santaAnimationTranslationZ = 12.0 - 8.0 * (timeInterval - 12.0) / 5.0;
                santaAnimationTranslationY = 5.5 - 5.0 * (timeInterval - 12.0) / 5.0;
                santaAnimationRotation = 360.0 - 270.0 * (timeInterval - 12.0) / 5.0;
                sleighAnimationTranslationX = 4.0 - 4.0 * (timeInterval - 12.0) / 5.0;
                sleighAnimationTranslationZ = 8.0 - 8.0 * (timeInterval - 12.0) / 5.0;
                sleighAnimationRotation = 270.0 - 270.0 * (timeInterval - 12.0) / 5.0;
                sleighAnimationTranslationY = 5.0 - 5.0 * (timeInterval - 12.0) / 5.0;
            }
            // finish the animation
            else {
                isPlaySantaAnimation = false;
                santaAnimationCounter++;
            }
            
        }

        // process keyboard input
        // ----------------------
        processInput(window);

        // render
        // -----------------------------------------------
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene as normal
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        //glm::mat4 model(1.0);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.Position);

        // draw and render the base floor
        // --------------------------------------------------------------
        baseLightEffect(shader, pointLightPositions[2]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, whiteSnowTexture);
        renderBase();

        // set the light effect
        // --------------------------------------------------------------
        shader.use();
        if (isNight) {
            LightEffectNight(shader, pointLightPositions, pointLightColors);
        }
        else {
            LightEffect(shader, pointLightPositions, pointLightColors);
        }
        
        // start dispaly different objects
        // ------------------------------------------------------------------------------------------------
        // draw the basement floor        
        {
            glm::mat4 model(1.0f);
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            baseModel.Draw(shader);
        }
        // draw the light house
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-18, 5, -18));
            model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(5.0, 5.0, 5.0));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            houseLightModel.Draw(shader);
        }

        // draw the castle
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(15, 1, -35));
            model = glm::rotate(model, glm::radians(85.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.8, 0.8, 0.8));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            castleModel.Draw(shader);
        }

        // draw the decorated christmas tree objects according to the tree position
        for (int i = 0; i < 3; i++)
        {
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, treeDecoratedPosition[i]);
                shader.use();
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);
                shader.setMat4("model", model);
                shader.setVec3("viewPos", camera.Position);
                treeDecoraredModel.Draw(shader);
            }
        }

        // draw the snow tree object
        for (int i = 0; i < 30; i++)
        {
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, treePosition[i]);
                shader.use();
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);
                shader.setMat4("model", model);
                shader.setVec3("viewPos", camera.Position);
                treeBareModel.Draw(shader);
            }
        }

        // draw the round snow tree object according to the tree position
        for (int i = 0; i < 20; i++)
        {
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, treeRoundPosition[i]);
                model = glm::scale(model, treeRoundScale[i]);
                shader.use();
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);
                shader.setMat4("model", model);
                shader.setVec3("viewPos", camera.Position);
                treeRoundModel.Draw(shader);
            }
        }

        // draw the santa model, do the transformation operation with respect to animation time
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-4 + santaAnimationTranslationX, 0 + santaAnimationTranslationY, 28 + santaAnimationTranslationZ));
            model = glm::rotate(model, glm::radians(-santaAnimationRotation), glm::vec3(0.0, 1.0, 0.0));  
            model = glm::scale(model, glm::vec3(0.7, 0.7, 0.7));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            if (santaMode == 0) {
                santaModel.Draw(shader);
            }
            else if (santaMode == 1) {
                santaBendModel.Draw(shader);
            }
            else if (santaMode == 2) {
                santaSitModel.Draw(shader);
            }
        }

        // draw the sleigh model, do the transformation operation with respect to animation time
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-4 + sleighAnimationTranslationX, 0 + sleighAnimationTranslationY, 32 + sleighAnimationTranslationZ));
            model = glm::rotate(model, glm::radians(-sleighAnimationRotation), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            sleighModel.Draw(shader);
        }

        // draw the gift model, do the transformation operation with respect to animation time
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-3 + giftAnimationTranslationX, 1.0 + giftAnimationTranslationY, 31 + giftAnimationTranslationZ));
            model = glm::rotate(model, glm::radians(-giftAnimationRotation), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            giftGRModel.Draw(shader);
        }

        // draw the purple gift model
        {
            glm::mat4 model(1.0f);
            // move the gift object to the target position
            model = glm::translate(model, glm::vec3(2.0, -0.2, 40.0));
            // rotate the gift object
            model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0));
            // change the size of the gift object
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            giftPurpleModel.Draw(shader);
        }

        // draw the red house model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(28, -0.5, 30));
            model = glm::rotate(model, glm::radians(220.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            houseRedModel.Draw(shader);
        }

        // draw the house model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-25, 0.5, 20));
            model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(20.0, 20.0, 20.0));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            houseModel.Draw(shader);
        }

        // draw the cloud model
        {
            glm::mat4 model(1.0f);
            model = glm::rotate(model, glm::radians(cloudAnimationRotate), glm::vec3(0.0, 1.0, 0.0));
            model = glm::translate(model, glm::vec3(3, 0, 0));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            cloudModel.Draw(shader);
        }

        // draw the bridge model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-2, -0.5, -1));
            model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0, 1.0, 0.0));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            bridgeModel.Draw(shader);
        }

        // draw the snowman model, do the transformation with respect to animation time
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(6, 1, 15));
            model = glm::rotate(model, glm::radians(-90 + snowmanAnimationRotate), glm::vec3(0.0, 1.0, 0.0));
            model = glm::translate(model, glm::vec3(snowmanAnimationTranslate, 0, snowmanAnimationTranslate));
            model = glm::scale(model, glm::vec3(0.8, 0.8, 0.8));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            snowmanModel.Draw(shader);
        }

        // draw the fence according to the tree position
        for (int i = 0; i < 3; i++)
        {
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, fencePosition[i]);
                model = glm::rotate(model, glm::radians(fenceRotation[i]), glm::vec3(0.0, 1.0, 0.0));
                model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
                shader.use();
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);
                shader.setMat4("model", model);
                shader.setVec3("viewPos", camera.Position);
                fenceModel.Draw(shader);
            }
        }

        // draw the candy cane model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-11, 1, 35));
            model = glm::rotate(model, glm::radians(65.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.8, 0.8, 0.8));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            candyCaneModel.Draw(shader);
        }

        // draw another candy cane model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-11, 0, 36));
            model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.7, 0.7, 0.7));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            candyCaneGreenModel.Draw(shader);
        }

        // draw the blue gift model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-10, 0.2, 33));
            model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            giftBlueModel.Draw(shader);
        }

        // draw the green gift model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-10, 0.2, 37));
            model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            giftGreenModel.Draw(shader);
        }

        // draw the red gift model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-9, 0.2, 35));
            model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            giftRedModel.Draw(shader);
        }

        // draw the wooden well model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-12, 0.2, 30));
            model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0, 1.0, 0.0));
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            woodWellModel.Draw(shader);
        }

        // draw the xmas word sign model
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-5, -0.5, 42));
            model = glm::scale(model, glm::vec3(3.0, 3.0, 3.0));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            xmasModel.Draw(shader);
        }

        // draw the rock models according to their position
        for (int i = 0; i < 9; i++)
        {
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, rockPosition[i]);
                if (i == 7) {
                    model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0, 1.0, 0.0));
                }
                model = glm::scale(model, rockScale[i]);
                shader.use();
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);
                shader.setMat4("model", model);
                shader.setVec3("viewPos", camera.Position);
                rockModel.Draw(shader);
            }
        }

        // draw the snow models move slower, do the transformation with respect to the animation time
        for (int i = 0; i < 20; i++)
        {
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, snowSlowerPosition[i] + glm::vec3(0, snowSlowerAnimationTranslate, 0));
                shader.use();
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);
                shader.setMat4("model", model);
                shader.setVec3("viewPos", camera.Position);
                snowModel.Draw(shader);
            }
        }

        // draw the snow models move quicker, do the transformation with respect to the animation time
        for (int i = 0; i < 10; i++)
        {
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, snowQuickerPosition[i] + glm::vec3(0, snowQuickerAnimationTranslate, 0));
                shader.use();
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);
                shader.setMat4("model", model);
                shader.setVec3("viewPos", camera.Position);
                snowModel.Draw(shader);
            }
        }

        // draw the smoke model, do the transformation with respect to the animation time
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(10.5, 8.0, 30.5) + glm::vec3(0.0, smokeAnimationTranslate, 0.0));
            model = glm::scale(model, glm::vec3(smokeScale, smokeScale, smokeScale));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            smokeModel.Draw(shader);
        }

        // draw another model, do the transformation with respect to the animation time
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(10.5, 8.0, 30.5) + glm::vec3(0.0, smokeQuickerAnimationTranslate, 0.0));
            model = glm::scale(model, glm::vec3(0.6, 0.6, 0.6));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            smokeModel.Draw(shader);
        }

        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-15.7, 7.0, 34.8) + glm::vec3(0.0, smokeAnimationTranslate, 0.0));
            model = glm::scale(model, glm::vec3(smokeScale, smokeScale, smokeScale));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            shader.setVec3("viewPos", camera.Position);
            smokeModel.Draw(shader);
        }
        //------------------ end of model displaying -------------------------------

        // draw skybox as last
        // -------------------------------------------------------------------------
        glDepthFunc(GL_LEQUAL);  
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // set depth function back to default
        glDepthFunc(GL_LESS);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // delete audio engine
    audioEngine->drop(); 
    // terminate the rendering
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!isPlaySantaAnimation) {
            isPlaySantaAnimation = true;
            santaAnimationStart = 0.0;
            santaMode = 0;
        }
    }
    // if press key number 1, change to camera auto moving mode
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        autoMode = true;
    }
    // if press key number 2, change to light mode
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        isNight = true;
    }
    // if press key number 3, change back to day mode
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        isNight = false;
    }
    if (!autoMode) {
        // if press key Q, the camera move speed will accelerate
        if ((glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)) {
            cameraSpeed = 4.0;
        }
        // if press key R, the camera move speed will return to original speed
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)) {
            cameraSpeed = 1.0;
        }
        // if press key W or UP, the camera will move forward
        if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)) {
            camera.ProcessKeyboard(FORWARD, deltaTime * cameraSpeed);
        }
        // if press key S or DOWN, the camera will move backward
        if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)) {
            camera.ProcessKeyboard(BACKWARD, deltaTime * cameraSpeed);
        }
        // if press key A or LEFT, the camera will move left
        if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) {
            camera.ProcessKeyboard(LEFT, deltaTime * cameraSpeed);
        }
        // if press key D or RIGHT, the camera will move right
        if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
            camera.ProcessKeyboard(RIGHT, deltaTime * cameraSpeed);
        }
        // if press key number 0, the camera will be set to the original position
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            camera = initCameraPos;
        }
    }
    // camera changes to auto mode
    else {
        autoModeTimer += deltaTime;
        if (autoModeTimer >= autoModeDuration) {
            camera = initCameraPos;
            autoMode = false;
            autoModeTimer = 0.0;
        }
        else {
            const float radius = 25.0f;
            float timeInverval = fmod(autoModeTimer, autoModeDuration);
            float camX, camY, camZ;
            if (timeInverval <= 8) {
                 camX = -6.0 * timeInverval / 8.0;
                 camZ = -40.0 * timeInverval / 8.0;
                 camY = -3.0 * timeInverval / 8.0;
                 camera = initCameraPos + glm::vec3(camX, camY, camZ);
            }
            else if (timeInverval <= 12) {
                 camX = sin(PI * 2 * (timeInverval- 8.0) / 15.0) * 20;
                 camZ = cos(PI * 2 * (timeInverval - 8.0) / 15.0) * 20;
                 camera = glm::vec3(camX, 5.0, camZ);
            }
            else{
                camX = sin(-PI * 2 * (timeInverval - 12.0) / 40.0) * 50;
                camZ = cos(-PI * 2 * (timeInverval - 12.0) / 40.0) * 50;
                camera = glm::vec3(camX, 7.0, camZ);
            }
            
        }

    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    // reversed since y-coordinates go from bottom to top
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// loads a cubemap texture and add textures to six individual faces
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


// set day light effect to the object shader with point light position and color accordingly
// ------------------------------------------------------------------------------------------
void LightEffect(Shader shader, glm::vec3 pointLightPositions[], glm::vec3 pointLightColors[]) {
    shader.use();
    // directional light
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
    shader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

    // point light 
    shader.setVec3("pointLights[0].position", pointLightPositions[0]);
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.2f, 0.2f, 0.2f);
    shader.setVec3("pointLights[0].specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09);
    shader.setFloat("pointLights[0].quadratic", 0.032);

    // spotLight
    shader.setVec3("spotLight.position", camera.Position);
    shader.setVec3("spotLight.direction", camera.Front);
    shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09);
    shader.setFloat("spotLight.quadratic", 0.032);
}

// set night light effect to the object shader with point light position and color accordingly
// -------------------------------------------------------------------------------------------
void LightEffectNight(Shader shader, glm::vec3 pointLightPositions[], glm::vec3 pointLightColors[]) {
    shader.use();
    // directional light
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.specular", 0.7f, 0.7f, 0.7f);

    // point light 
    shader.setVec3("pointLights[0].position", pointLightPositions[1]);
    shader.setVec3("pointLights[0].ambient", pointLightColors[0] * 0.05f);
    shader.setVec3("pointLights[0].diffuse", pointLightColors[0] * 1.0f);
    shader.setVec3("pointLights[0].specular", pointLightColors[0] * 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09);
    shader.setFloat("pointLights[0].quadratic", 0.032);


    // spotLight
    shader.setVec3("spotLight.position", camera.Position);
    shader.setVec3("spotLight.direction", camera.Front);
    shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09);
    shader.setFloat("spotLight.quadratic", 0.032);
}

// set day light effect to the basement shader with point light position and color accordingly
// -------------------------------------------------------------------------------------------
void baseLightEffect(Shader shader, glm::vec3 lightPos) {
    // directional light
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

    shader.setVec3("pointLights[0].position", lightPos);
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.5, 0.5, 0.5);
    shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.01);
    shader.setFloat("pointLights[0].quadratic", 0.007);

    // spotLight
    shader.setVec3("spotLight.position", camera.Position);
    shader.setVec3("spotLight.direction", camera.Front);
    shader.setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09);
    shader.setFloat("spotLight.quadratic", 0.032);
}

// render the basement floor
// ----------------------------------------------------------------
void renderBase() {
    if (baseVAO == 0) {
        float baseVertices[] = {
             20.0f, -0.0f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
            -20.0f, -0.0f,  30.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -20.0f, -0.0f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 20.0f,

             20.0f, -0.0f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
            -20.0f, -0.0f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 20.0f,
             20.0f, -0.0f, -20.0f,  0.0f, 1.0f, 0.0f,  20.0f, 20.0f,

             20.0f, -0.0f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
             20.0f, -0.0f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -0.0f,  -10.0f, -0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 20.0f,

             20.0f, -0.0f, 20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
            -20.0f, -0.0f, 20.0f,  0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
             0.0f, -10.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 20.0f,

            -20.0f, -0.0f, -20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
            -20.0f, -0.0f, 20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -0.0f, -10.0f, -0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 20.0f,

            -20.0f, -0.0f,  -20.0f,  0.0f, 1.0f, 0.0f,   20.0f,  0.0f,
             20.0f, -0.0f,  -20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
             0.0f, -10.0f,  -0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 20.0f,
        };

        glGenVertexArrays(1, &baseVAO);
        glGenBuffers(1, &baseVBO);
        glBindVertexArray(baseVAO);
        glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(baseVertices), baseVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(baseVAO);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    glBindVertexArray(0);
}