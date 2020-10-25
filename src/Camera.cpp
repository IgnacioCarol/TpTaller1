//
// Created by nacho on 25/10/20.
//

#include "Camera.h"

Camera::Camera(int x, int y, int width, int height) {
    camera = {x, y , width, height};
}

void Camera::render(int playerXPos, int borderLimit) {
    camera.x = ( playerXPos  + 200) - camera.w;
    lastValue = camera.x > lastValue ? camera.x : lastValue;
    if( camera.x < lastValue )
    {
        camera.x = lastValue;
    }
    if( camera.x > borderLimit - camera.w )
    {
        camera.x = borderLimit - camera.w;
    }
}

SDL_Rect *Camera::getCamera() {
    return &camera;
}

int Camera::getXpos() {
    return camera.x;
}
