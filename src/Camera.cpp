#include "Camera.h"

Camera::Camera(int x, int y, int width, int height) {
    camera = {x, y , width, height};
}

void Camera::update(int playerXPos, int borderLimit) {
    camera.x = ( playerXPos  + 200) - camera.w;
    lastValue = camera.x > lastValue ? camera.x : lastValue;
    if( camera.x < lastValue )
    {
        camera.x = lastValue;
    }
    if( camera.x > borderLimit - camera.w )
    {
        //TODO ver como manejar next stage
    }
}

SDL_Rect *Camera::getCamera() {
    return &camera;
}

int Camera::getXpos() {
    return camera.x;
}

void Camera::restartPos() {
    camera = {0, 0, camera.w, camera.h};
    lastValue = 0;
}

void Camera::setXPos(int newX) {
    camera.x = newX;
}
