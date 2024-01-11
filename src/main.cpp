#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <graphics/shader.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Fonction pour générer les sommets de la grille
std::vector<float> generateGridVertices(int gridSize) {
    std::vector<float> vertices;
    for (int i = -gridSize; i <= gridSize; i++) {
        // Ligne verticale
        vertices.push_back((float)i); vertices.push_back(0.0f); vertices.push_back((float)-gridSize);
        vertices.push_back((float)i); vertices.push_back(0.0f); vertices.push_back((float)gridSize);

        // Ligne horizontale
        vertices.push_back((float)-gridSize); vertices.push_back(0.0f); vertices.push_back((float)i);
        vertices.push_back((float)gridSize); vertices.push_back(0.0f); vertices.push_back((float)i);
    }
    return vertices;
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

    // Enregistrer la fonction de rappel pour le redimensionnement de la fenêtre
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLfloat vertices[] {
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    GLuint indices[] {
        0, 1, 2,
        2, 0, 3
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    Shader shaderProgram("C:\\Users\\justi\\Desktop\\OGLA\\POGLA\\shaders\\grid.vtx", "C:\\Users\\justi\\Desktop\\OGLA\\POGLA\\shaders\\grid.frg");

    // Boucle de rendu
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Entrée
        processInput(window);

        // Rendu
        // (Ici, vous pouvez ajouter le code de rendu)
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // Échange des tampons et sondage des événements d'IO

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Libération des ressources GLFW
    glfwTerminate();
    return 0;
}

// Fonction pour gérer les entrées clavier
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Fonction de rappel appelée à chaque fois que la fenêtre est redimensionnée
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
