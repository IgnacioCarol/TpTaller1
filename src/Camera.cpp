#include "Camera.h"

Camera::Camera(int x, int y, int width, int height) {
    camera = {x, y , width, height};
}

void Camera::update(std::vector<Player*> players, int borderLimit) {
    int maxPos = 0;
    int minPos = 20000;
    for (Player* player: players){
        if (player->getState() != "PAUSED"){
            int playerPos = player -> getXPosition();
            maxPos = (playerPos > maxPos) ? playerPos : maxPos;
            minPos = (playerPos < minPos) ? playerPos : minPos;
        }
    }
    if (maxPos - minPos < 600){
        int diff = maxPos - camera.x;
        camera.x = diff > 605 ? minPos - camera.w + diff : ( maxPos  + 200)  - camera.w;
        lastValue = camera.x > lastValue ? camera.x : lastValue;
    }

    if( camera.x < lastValue )
    {
        camera.x = lastValue;
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
