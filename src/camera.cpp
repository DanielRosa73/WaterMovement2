#include <camera.h>


Camera::Camera() {
    _position = glm::vec3(0.0f);
    _yaw = -90.0f; 
    _pitch = 0.0f;
    _speed = 5.0f;
    _sensitivity = 0.1f;
    _zoom = 45.0f;
    _aspectRatio = 4.0 / 3.0f;
    _nearPlane = 0.1f;
    _farPlane = 1000.0f;

    updateCameraVectors();
}

Camera::Camera(unsigned windowHeight, unsigned windowWidth) {
    _position = glm::vec3(0.0f);
    _yaw = -90.0f; 
    _pitch = 0.0f;
    _speed = 5.0f;
    _sensitivity = 1.0f;
    _zoom = 45.0f;
    _aspectRatio = static_cast<float>(windowWidth) / windowHeight;
    _nearPlane = 0.1f;
    _farPlane = 1000.0f;

    updateCameraVectors();
}


Camera::Camera(glm::vec3 position, unsigned windowHeight, unsigned windowWidth) {
    _position = position;
    _yaw = -90.0f; 
    _pitch = 0.0f;
    _speed = 5.0f;
    _sensitivity = 0.1f;
    _zoom = 45.0f;
    _aspectRatio = windowWidth / windowHeight;
    _nearPlane = 0.1f;
    _farPlane = 1000.0f;

    updateCameraVectors();
}


Camera::Camera(glm::vec3 position, float yaw, float pitch, float speed, float sensitivity, float zoom) {
    _position = position;
    _yaw = yaw;
    _pitch = pitch;
    _speed = speed;
    _sensitivity = sensitivity;
    _zoom = zoom;
    _aspectRatio = 4.0 / 3.0f;
    _nearPlane = 0.1f;
    _farPlane = 1000.0f;

    updateCameraVectors();
}


Camera::Camera(glm::vec3 position, float yaw, float pitch, float speed, float sensitivity, float zoom, unsigned windowHeight, unsigned windowWidth) {
    _position = position;
    _yaw = yaw;
    _pitch = pitch;
    _speed = speed;
    _sensitivity = sensitivity;
    _zoom = zoom;
    _aspectRatio = windowWidth / windowHeight;
    _nearPlane = 0.1f;
    _farPlane = 1000.0f;

    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}


glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(_zoom), _aspectRatio, _nearPlane, _farPlane);
}


glm::mat4 Camera::getFrustrumMatrix() const {
    return getViewMatrix() * getProjectionMatrix();
}


glm::vec3 Camera::getPosition() const {
    return _position;
}


glm::vec3 Camera::getViewDirection() const {
    return _front;
}


void Camera::moveForward(float deltaTime) {
    _position += _speed * deltaTime * _front;
}


void Camera::moveBackward(float deltaTime) {
    _position -= _speed * deltaTime * _front;
}


void Camera::moveRight(float deltaTime) {
    _position += glm::normalize(glm::cross(_front, _up)) * _speed * deltaTime;
}


void Camera::moveLeft(float deltaTime) {
    _position -= glm::normalize(glm::cross(_front, _up)) * _speed * deltaTime;
}


void Camera::moveDown(float deltaTime) {
    _position -= _speed * deltaTime * _worldUp;
}


void Camera::moveUp(float deltaTime) {
    _position += _speed * deltaTime * _worldUp;
}


void Camera::rotate(float yaw, float pitch) {
    _yaw += yaw * _sensitivity;
    _pitch += pitch * _sensitivity;

    if (_pitch > 89.0f)
        _pitch = 89.0f;
    if (_pitch < -89.0f)
        _pitch = -89.0f;

    updateCameraVectors();
}

void Camera::setAspectRatio(float aspectRatio) {
    _aspectRatio = aspectRatio;
}

void Camera::setSpeed(float speed) {
    _speed = speed;
}


void Camera::setPosition(const glm::vec3& position) {
    _position = position;
}


void Camera::setZoom(float zoom) {
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f; 
    _zoom = zoom;
}


float Camera::getZoom() const {
    return _zoom;
}


void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));  
    _up = glm::normalize(glm::cross(_right, _front));
}