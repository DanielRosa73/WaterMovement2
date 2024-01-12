#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    Camera(unsigned windowHeight, unsigned windowWidth);
    Camera(glm::vec3 position, unsigned windowHeight, unsigned windowWidth);
    Camera(glm::vec3 position, float yaw, float pitch, float speed, float sensitivity, float zoom);
    Camera(glm::vec3 position, float yaw, float pitch, float speed, float sensitivity, float zoom, unsigned windowHeight, unsigned windowWidth);


    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getFrustrumMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getViewDirection() const;

    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveRight(float deltaTime);
    void moveLeft(float deltaTime);
    void moveDown(float deltaTime);
    void moveUp(float deltaTime);

    void rotate(float yaw, float pitch);

    void setAspectRatio(float aspectRatio);

    void setSpeed(float speed);
    void setPosition(const glm::vec3 &position);

    void setZoom(float zoom);
    float getZoom() const;

private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float _yaw;
    float _pitch;
    float _speed;
    float _sensitivity;
    float _zoom;
    float _aspectRatio;
    float _nearPlane;
    float _farPlane;

    void updateCameraVectors();
};