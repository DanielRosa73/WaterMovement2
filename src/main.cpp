#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <camera.h>
#include <graphics/shader.h>


#define WINDOW_HEIGHT 900
#define WINDOW_WITDTH 1440


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera& cam, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


void generateGrid(int gridSize, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
    // Générer les vertices pour la grille
    for (int y = 0; y <= gridSize; ++y) {
        for (int x = 0; x <= gridSize; ++x) {
            float xPos = (float)x - 0.5f;
            float yPos = (float)y - 0.5f;

            vertices.push_back(xPos); // Coordonnée X
            vertices.push_back(x == gridSize / 2 && y == gridSize / 2 ? 10.0f : 0.0f); // Coordonnée Y
            vertices.push_back(yPos); // Coordonnée Z (tous les points sont sur un plan)
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
    generateGrid(100, vertices, indices);

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

    Shader shaderProgram("..\\..\\shaders\\grid.vtx.glsl", "..\\..\\shaders\\grid.frg.glsl");

    int height, width;
    glfwGetWindowSize(window, &width, &height);
    Camera cam(height, width);
    cam.setPosition(glm::vec3(0.0, 0.0, 2.0));
    glfwSetWindowUserPointer(window, &cam);
    
    shaderProgram.use();
    glBindVertexArray(VAO);

    double lastTime = glfwGetTime();
    double lastFPSTime = lastTime; // Variable séparée pour le compteur de FPS
    int nbFrames = 0;
    // Boucle de rendu
    while (!glfwWindowShouldClose(window)) {
        // Calculer le delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Mise à jour du compteur de FPS
        nbFrames++;
        if (currentTime - lastFPSTime >= 1.0) {
            std::stringstream ss;
            ss << "POGLA - " << nbFrames << " FPS";
            glfwSetWindowTitle(window, ss.str().c_str());

            nbFrames = 0;
            lastFPSTime = currentTime;
        }

        // Process input en utilisant deltaTime
        processInput(window, cam, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Entrée
        processInput(window, cam, deltaTime);

        shaderProgram.setMat4("view", cam.getViewMatrix());
        shaderProgram.setMat4("projection", cam.getProjectionMatrix());

        // Rendu
        // (Ici, vous pouvez ajouter le code de rendu)
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        // Échange des tampons et sondage des événements d'IO

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
