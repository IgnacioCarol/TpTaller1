#ifndef TPTALLER1_COIN_H
#define TPTALLER1_COIN_H
#include "GameObject.h"
#include "Player.h"

#define POINTS_COIN 50

class Coin : public GameObject {
    public:
        ~Coin() override;
        void init(int x, int y, std::string textureID) override;
        void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;
        void move() override;
        void collideWith(GameObject *go) override;
        void hide() override;
        void unhide() override;
        std::string getState() override;
        void setState(std::string newState) override;
        void changeState(std::string newState);
        void initInPosition(int x, int y, std::string textureID);
        int getFloorPosition() override;
        int centerXPos() override;
        void collideWith(Pipe *pipe) override;
    private:
        //Related to the image
        static const int COIN_WIDTH = 161;
        static const int COIN_HEIGHT = 164;
        static const int COIN_DELAY = 4;
        static const int COIN_FRAMES = 4;
        std::string stateType = "UNCATCHED";
        size_t delayCounter;
        void setYPosition(GameObject *go, int height);
};


#endif //TPTALLER1_COIN_H
