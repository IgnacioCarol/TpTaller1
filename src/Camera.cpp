#include "Camera.h"

Camera::Camera(int x, int y, int width, int height) {
    camera = {x, y , width, height};
}

void Camera::update(std::vector<Player *> players) {
    Logger::getInstance()->debug("Updating camera...");
    static const int displacement = 200;
    int maxPos = 0;
    int minPos = 20000;
    for (Player* player: players){
        if (player->getState() != "PAUSED"){
            int playerPos = player -> getXPosition();
            maxPos = (playerPos > maxPos) ? playerPos : maxPos;
            minPos = (playerPos < minPos) ? playerPos : minPos;
        }
    }
    if (maxPos - minPos < camera.w - displacement){
        int diff = maxPos - camera.x;
        camera.x = diff > 610 ? minPos - camera.w + diff + (200 - displacement) : (maxPos  + displacement)  - camera.w;
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
