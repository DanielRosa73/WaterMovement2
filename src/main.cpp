#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <vector>

#include <camera.h>
#include <graphics/shader.h>
#include <graphics/texture.h>

#include <cmath>


#define WINDOW_HEIGHT 900
#define WINDOW_WITDTH 1440
#define WATERSIZE 50 //il faut laisser ces tailles
#define GRIDSIZE 50 //ici aussi

bool reset = false;

bool noColor = false;
bool normalColor = false;
bool marbleBind = false;

int gridPosX = WATERSIZE / 2;
int gridPosY = WATERSIZE / 2;
bool createDisturbance = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera& cam, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


void generateGrid(int gridSize, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
    for (int y = 0; y <= gridSize; ++y) {
        for (int x = 0; x <= gridSize; ++x) {
            float xPos = (float)x - 0.5 * gridSize;
            float yPos = 0.0f;
            float zPos = (float)y - 0.5 * gridSize;

            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
        }
    }

    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            GLuint topLeft = y * (gridSize + 1) + x;
            GLuint topRight = topLeft + 1;
            GLuint bottomLeft = (y + 1) * (gridSize + 1) + x;
            GLuint bottomRight = bottomLeft + 1;

            // Premier triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Deuxième triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

float halfGridSize = (GRIDSIZE) / 2.0;


GLfloat wallLow = -50.0, wallUp = 2.0;
GLfloat offset = 0.0;//abs(wallLow / 20.0);
GLfloat wallVertices[] = {
    // Front face
    -halfGridSize + offset, wallLow, halfGridSize - offset,      0.0,  0.0,  1.0,      0.0, 0.0,
    -halfGridSize,  wallUp, halfGridSize,      0.0,  0.0,  1.0,      0.0, 1.0,
     halfGridSize,  wallUp, halfGridSize,      0.0,  0.0,  1.0,      1.0, 1.0,
     halfGridSize - offset, wallLow, halfGridSize - offset,      0.0,  0.0,  1.0,      1.0, 0.0,

    // Left face
    -halfGridSize + offset, wallLow,  halfGridSize - offset,    -1.0,  0.0,  0.0,      1.0, 0.0,
    -halfGridSize,  wallUp,  halfGridSize,    -1.0,  0.0,  0.0,      1.0, 1.0,
    -halfGridSize,  wallUp, -halfGridSize,    -1.0,  0.0,  0.0,      0.0, 1.0,
    -halfGridSize + offset, wallLow, -halfGridSize + offset,    -1.0,  0.0,  0.0,      0.0, 0.0,

    // Right face
     halfGridSize - offset, wallLow,  halfGridSize - offset,     1.0,  0.0,  0.0,      0.0, 0.0,
     halfGridSize,  wallUp,  halfGridSize,     1.0,  0.0,  0.0,      0.0, 1.0,
     halfGridSize,  wallUp, -halfGridSize,     1.0,  0.0,  0.0,      1.0, 1.0,
     halfGridSize - offset, wallLow, -halfGridSize + offset,     1.0,  0.0,  0.0,      1.0, 0.0,

    // Back face
    -halfGridSize + offset, wallLow, -halfGridSize + offset,      0.0,  0.0, -1.0,      0.0, 1.0,
    -halfGridSize,  wallUp, -halfGridSize,      0.0,  0.0, -1.0,      1.0, 1.0,
     halfGridSize,  wallUp, -halfGridSize,      0.0,  0.0, -1.0,      1.0, 0.0,//
     halfGridSize - offset, wallLow, -halfGridSize + offset,      0.0,  0.0, -1.0,      0.0, 0.0,

    // Bottom face
    -halfGridSize, wallLow,  halfGridSize,      0.0, -1.0,  0.0,      0.0, 1.0,
    -halfGridSize, wallLow, -halfGridSize,      0.0, -1.0,  0.0,      0.0, 0.0,
     halfGridSize, wallLow, -halfGridSize,      0.0, -1.0,  0.0,      1.0, 0.0,
     halfGridSize, wallLow,  halfGridSize,      0.0, -1.0,  0.0,      1.0, 1.0
};

GLuint wallIndices[] = {
    // Front face
    0, 1, 2, // Première moitié du rectangle (triangle)
    2, 3, 0, // Deuxième moitié du rectangle (triangle)

    // Left face
    4, 5, 6, // Première moitié du rectangle (triangle)
    6, 7, 4, // Deuxième moitié du rectangle (triangle)

    // Right face
    8, 9, 10, // Première moitié du rectangle (triangle)
    10, 11, 8, // Deuxième moitié du rectangle (triangle)

    // Back face
    12, 13, 14, // Première moitié du rectangle (triangle)
    14, 15, 12,  // Deuxième moitié du rectangle (triangle)

    // Bottom face
    16, 17, 18,
    18, 19, 16
};


GLuint createWall() {
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    return VAO;
}

void drawWall(const Camera &camera, Shader &shaderProgram, GLuint wallVAO) {
    shaderProgram.use();
    glBindVertexArray(wallVAO);
    shaderProgram.setMat4("projection", camera.getProjectionMatrix());
    shaderProgram.setMat4("view", camera.getViewMatrix());
    glDrawElements(GL_TRIANGLES, sizeof(wallIndices) / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "POGLA", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    GLuint wallVAO = createWall();

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    generateGrid(GRIDSIZE, vertices, indices);

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


    Shader wallShader("../../shaders/wall.vtx.glsl", "../../shaders/wall.frg.glsl");
    Shader shaderProgram("../../shaders/grid.vtx.glsl", "../../shaders/grid.frg.glsl", "../../shaders/grid.tess.ctrl.glsl", "../../shaders/grid.tess.eval.glsl");
    Shader shaderCompute("../../shaders/ripple.comp.glsl");

    Texture marble("../../textures/marble.png");
    marble.bind();

    int height, width;
    glfwGetWindowSize(window, &width, &height);
    Camera cam(height, width);
    cam.setPosition(glm::vec3(0.0, 10.0, 0.0));
    glfwSetWindowUserPointer(window, &cam);
    
    shaderProgram.use();
    shaderProgram.setInt("WATERSIZE", WATERSIZE);
    

    glPatchParameteri(GL_PATCH_VERTICES,3);

    glBindVertexArray(VAO);

    double lastTime = glfwGetTime();
    double lastFPSTime = lastTime;
    int nbFrames = 0;

    int currentTextureIndex = 0;
    GLuint waterTextures[2];
    glGenTextures(2, waterTextures);
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, waterTextures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        float spikeyData[WATERSIZE][WATERSIZE] = {0};
        /*
        for (int y = 0; y < WATERSIZE; y+=3) {
            for (int x = 0; x < WATERSIZE; x+=3) {
                spikeyData[x][y] = 1.0f;
            }
        }
        */
        //spikeyData[WATERSIZE / 2][WATERSIZE / 2] = -10.0f;
        // Initialize textures with zeros
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WATERSIZE, WATERSIZE, 0, GL_RED, GL_FLOAT, spikeyData);
    }

    int wantedFPS = 100;
    const double frameTime = 1.0 / wantedFPS;

    double totalTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        double frameStartTime = glfwGetTime();
        if (reset) {
            glGenTextures(2, waterTextures);
            for (int i = 0; i < 2; ++i) {
                glBindTexture(GL_TEXTURE_2D, waterTextures[i]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                float spikeyData[WATERSIZE][WATERSIZE] = {0};
                glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WATERSIZE, WATERSIZE, 0, GL_RED, GL_FLOAT, spikeyData);
            }
            reset = false;
        }
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        nbFrames++;
        if (currentTime - lastFPSTime >= 1.0) {
            std::stringstream ss;
            ss << "POGLA - " << nbFrames << " FPS";
            glfwSetWindowTitle(window, ss.str().c_str());

            nbFrames = 0;
            lastFPSTime = currentTime;
        }

        processInput(window, cam, deltaTime);

        if (createDisturbance) {
            glBindTexture(GL_TEXTURE_2D, waterTextures[currentTextureIndex]);
            float disturbanceValue = -0.0001f; 
            glTexSubImage2D(GL_TEXTURE_2D, 0, gridPosX, gridPosY, 1, 1, GL_RED, GL_FLOAT, &disturbanceValue);
            createDisturbance = false;
        }

        shaderCompute.use();
        shaderCompute.setFloat("deltaTime", static_cast<float>(deltaTime * 2));
        shaderCompute.setInt("WATERSIZE", WATERSIZE);
 
        glBindImageTexture(0, waterTextures[currentTextureIndex], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
        glBindImageTexture(1, waterTextures[1 - currentTextureIndex], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

        glDispatchCompute((WATERSIZE + 15) / 16, (WATERSIZE + 15) / 16, 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        shaderProgram.setFloat("deltaTime", static_cast<float>(deltaTime));
        shaderProgram.setMat4("view", cam.getViewMatrix());
        shaderProgram.setMat4("projection", cam.getProjectionMatrix());
        shaderProgram.setVec3("viewPos", cam.getPosition());
        shaderProgram.setVec3("lightPos", glm::vec3(0.0, 10.0, 0.0));
        shaderProgram.setVec3("lightColor", glm::vec3(1.0));
        shaderProgram.setBool("noColor", noColor);
        shaderProgram.setBool("normalColor", normalColor);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterTextures[currentTextureIndex]);
        shaderProgram.setInt("waterHeightMap", 0);

        glBindVertexArray(VAO);
        glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);

        if (marbleBind)
            marble.bind();
        drawWall(cam, wallShader, wallVAO);

        currentTextureIndex = 1 - currentTextureIndex;

        glfwSwapBuffers(window);
        glfwPollEvents();

         double frameEndTime = glfwGetTime(); // Mesurer la fin de la frame
        double elapsedTime = frameEndTime - frameStartTime; // Calculer le temps écoulé pour le rendu de la frame

        // Si la frame a été rendue plus vite que le temps alloué, attendre le reste du temps
        if (elapsedTime < frameTime) {
            double remainingTime = frameTime - elapsedTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(remainingTime * 1000)));
        }
    }

    glBindVertexArray(0);
    glfwTerminate();
    return 0;
}

float lastX = WINDOW_WITDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
bool xKeyPressed = false;
bool wireframeMode = false;
bool spaceKeyPressed = false;
bool kKeyPressed = false;
bool tKeyPressed = false;
bool nKeyPressed = false;

void processInput(GLFWwindow* window, Camera& cam, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.moveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.moveDown(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.moveUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
        gridPosY = std::max(0, gridPosY - 1);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
        gridPosY = std::min(WATERSIZE - 1, gridPosY + 1);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        gridPosX = std::max(0, gridPosX - 1);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        gridPosX = std::min(WATERSIZE - 1, gridPosX + 1);

    bool xKeyDown = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
	if (xKeyDown && !xKeyPressed) {
		wireframeMode = !wireframeMode;
		if (wireframeMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            noColor = true;
        }
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            noColor = false;
        }
	}
	xKeyPressed = xKeyDown;

    bool spaceKeyDown = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (spaceKeyDown && !spaceKeyPressed) {
        createDisturbance = !createDisturbance;
    }
    spaceKeyPressed = spaceKeyDown;

    bool kKeyDown = glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS;
    if (kKeyDown && !kKeyPressed) {
        createDisturbance = false;
        reset = !reset;
    }
    kKeyPressed = kKeyDown;

    bool nKeyDown = glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS;
    if (nKeyDown && !nKeyPressed) {
        normalColor = !normalColor;
    }
    nKeyPressed = nKeyDown;

    bool tKeyDown = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    if (tKeyDown && !tKeyPressed) {
        marbleBind = !marbleBind;
    }
    tKeyPressed = tKeyDown;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->rotate(xoffset, yoffset);
}
