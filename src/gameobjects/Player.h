#ifndef TPTALLER1_MARIO_H
#define TPTALLER1_MARIO_H


#include <string>
#include "../logger/logger.h"
#include "../TextureManager.h"
#include "GameObject.h"

#define playerID "mario" //Principal Player
#define imgPlayer "Sprites/mario.png"
#define defaultPlayer "Sprites/Default/defaultPlayer.png"

//Image related
#define pWidth 600
#define pHeight 600
const int playerVelocity = 2;
class CharacterState;

class Player : public GameObject {
public:
    Player(SDL_Rect *camera);
    ~Player();
    void init(size_t x, size_t y, std::string fileName, std::string defaultImg, std::string textureID, int currentFrame, SDL_Rect *camera, int framesAmount);
    void jump(int yMove);
    void run(int direction);

    int getXPosition(){return xPosition;}

    void draw(SDL_Renderer *renderer, int cameraX, int cameraY) override;

    bool isJumping();

    bool finishJump();

    void restartPos(int i, int i1);

    void changeState(CharacterState* newState);

    void move() override;

private:
    bool xDirection; //Despues hay que guiarse por otra cosa, bien hardcodeado. True = +x False = -x

    CharacterState* characterState;
    bool jumping;
    bool canJump() const;
    int initialJumpingPosition;
    int maxYPosition;
    SDL_Rect *cam;
    Logger* logger = Logger::getInstance(); //ToDo Volar esto de aca, looger no deberia ser un atributo de ninguna clase ya que es un singleton.
};


#endif //TPTALLER1_MARIO_H
