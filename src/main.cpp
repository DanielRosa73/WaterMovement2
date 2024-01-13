#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <camera.h>
#include <graphics/shader.h>

#include <cmath>


#define WINDOW_HEIGHT 900
#define WINDOW_WITDTH 1440
#define WATERSIZE 50
#define GRIDSIZE 50




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera& cam, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


void generateGrid(int gridSize, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
    // Générer les vertices pour la grille
    for (int y = 0; y <= gridSize; ++y) {
        for (int x = 0; x <= gridSize; ++x) {
            float xPos = (float)x - 0.5f * gridSize;
            float yPos = 0.0f;
            float zPos = (float)y - 0.5f * gridSize;

            vertices.push_back(xPos); // Coordonnée X
            vertices.push_back(yPos); // Coordonnée Y
            vertices.push_back(zPos); // Coordonnée Z (tous les points sont sur un plan)
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




int main() {
    // Initialisation de GLFW
    glfwInit();

    // Configuration de GLFW
    // Spécifiez que vous voulez utiliser OpenGL version 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // Utiliser le profil Core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Création de la fenêtre
    GLFWwindow* window = glfwCreateWindow(800, 600, "POGLA", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialisation de GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Définir la taille de la zone de rendu
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    // Enregistrer la fonction de rappel pour le redimensionnement de la fenêtre
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    // Générer une grille de taille 10x10
    generateGrid(GRIDSIZE, vertices, indices);

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Configurer le buffer de vertex
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Configurer le buffer d'indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Définir le format des données de vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    Shader shaderProgram("/home/daniel.rosa/afs/pogla/POGLA/shaders/grid.vtx.glsl", "/home/daniel.rosa/afs/pogla/POGLA/shaders/grid.frg.glsl", "/home/daniel.rosa/afs/pogla/POGLA/shaders/grid.tess.ctrl.glsl", "/home/daniel.rosa/afs/pogla/POGLA/shaders/grid.tess.eval.glsl");
    Shader shaderCompute("/home/daniel.rosa/afs/pogla/POGLA/shaders/ripple.comp.glsl");


    int height, width;
    glfwGetWindowSize(window, &width, &height);
    Camera cam(height, width);
    cam.setPosition(glm::vec3(0.0, 10.0, 2.0));
    glfwSetWindowUserPointer(window, &cam);
    
    shaderProgram.use();
    shaderProgram.setInt("WATERSIZE", WATERSIZE);
    

    glPatchParameteri(GL_PATCH_VERTICES,3);

    glBindVertexArray(VAO);

    double lastTime = glfwGetTime();
    double lastFPSTime = lastTime; // Variable séparée pour le compteur de FPS
    int nbFrames = 0;

    int currentTextureIndex = 0;

        // Texture creation for water simulation
    GLuint waterTextures[2];
    glGenTextures(2, waterTextures);
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, waterTextures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


        // Create gradient data
        float spikeyData[WATERSIZE][WATERSIZE] = {0};
        for (int y = 0; y < WATERSIZE; y+=3) {
            for (int x = 0; x < WATERSIZE; x+=3) {
                spikeyData[x][y] = 1.0f;
            }
        }
        // Initialize textures with zeros
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WATERSIZE, WATERSIZE, 0, GL_RED, GL_FLOAT, spikeyData);
    }

    // Boucle de rendu
    while (!glfwWindowShouldClose(window)) {
        // Calculate the delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the FPS counter
        nbFrames++;
        if (currentTime - lastFPSTime >= 1.0) {
            std::stringstream ss;
            ss << "POGLA - " << nbFrames << " FPS";
            glfwSetWindowTitle(window, ss.str().c_str());

            nbFrames = 0;
            lastFPSTime = currentTime;
        }

        // Process input using deltaTime
        processInput(window, cam, deltaTime);

        // Bind the textures for the compute shader
        shaderCompute.use();
        shaderCompute.setInt("WATERSIZE", WATERSIZE);
 
        glBindImageTexture(0, waterTextures[currentTextureIndex], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
        glBindImageTexture(1, waterTextures[1 - currentTextureIndex], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

        // Dispatch the compute shader
        glDispatchCompute((WATERSIZE + 15) / 16, (WATERSIZE + 15) / 16, 1); // Adjust the dispatch size as needed

        // Synchronize to ensure texture writes are finished before reading in tessellation shader
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view and projection matrices in the shader
        shaderProgram.use();
        shaderProgram.setMat4("view", cam.getViewMatrix());
        shaderProgram.setMat4("projection", cam.getProjectionMatrix());

        // Bind the current water texture for rendering
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterTextures[currentTextureIndex]);
        shaderProgram.setInt("waterHeightMap", 0);  // Set the uniform

        // Render the grid
        glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);

        // Swap the texture index for the next frame
        currentTextureIndex = 1 - currentTextureIndex;

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindVertexArray(0);

    // Libération des ressources GLFW
    glfwTerminate();
    return 0;
}

float lastX = WINDOW_WITDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
bool xKeyPressed = false;
bool wireframeMode = false;

// Fonction pour gérer les entrées clavier
void processInput(GLFWwindow* window, Camera& cam, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 initialCamPosition = cam.getPosition();
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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.moveUp(deltaTime);
    glm::vec3 camPosition = cam.getPosition();

    bool xKeyDown = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
	if (xKeyDown && !xKeyPressed) {
		wireframeMode = !wireframeMode;
		if (wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	xKeyPressed = xKeyDown;
}

// Fonction de rappel appelée à chaque fois que la fenêtre est redimensionnée
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
    float yoffset = lastY - ypos; // Inversé puisque y-coordonnées vont de bas en haut
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->rotate(xoffset, yoffset);
}
