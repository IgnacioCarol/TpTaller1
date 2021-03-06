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
    GameObject* generateItem(int xPos, int yPos);
    void popItem() override;
    void setMushroom(bool hasMushroom);
    void collideWith(GameObject* go) override;
    void setState(std::string newState) override;
    std::string getState() override;
    int getFloorPosition() override;
    int centerXPos() override;
    int getWidth() override;


private:
    //Image related definitions
    static const int SURPRISE_BLOCK_WIDTH = 256;
    static const int SURPRISE_BLOCK_HEIGHT = 256;
    //Animation related definitions
    static const int SURPRISE_BLOCK_DELAY = 10;
    static const int SURPRISE_BLOCK_FRAMES = 5;
    const std::string COIN_ID = "coinsSprites";
    const std::string MUSHROOM_ID = "magicMushroom";
    std::string stateType = "FULL";

    size_t delayCounter;
    GameObject* innerItem;
    bool hasMushroom;
};


#endif //TPTALLER1_PLATFORMSURPRISE_H
