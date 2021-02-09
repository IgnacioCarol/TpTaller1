#ifndef TPTALLER1_PLATFORMSURPRISE_H
#define TPTALLER1_PLATFORMSURPRISE_H
#include "GameObject.h"
#include "Player.h"

class Player;

class PlatformSurprise : public GameObject {
public:
    ~PlatformSurprise() override = default;
    void init(int x, int y, std::string textureID) override;
    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
    void move() override {};
    int getHeight() override;
    //void collideWith(Player* player);

private:
    //Image related definitions
    static const int SURPRISE_BLOCK_WIDTH = 256;
    static const int SURPRISE_BLOCK_HEIGHT = 256;
    //Animation related definitions
    static const int SURPRISE_BLOCK_DELAY = 10;
    static const int SURPRISE_BLOCK_FRAMES = 5;
    size_t delayCounter;
    bool containsItem = true;
};


#endif //TPTALLER1_PLATFORMSURPRISE_H
