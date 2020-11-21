#ifndef TPTALLER1_FACTORY_H
#define TPTALLER1_FACTORY_H

#include <vector>
#include "../gameobjects/GameObject.h"
#include "../config/Config.h"

//Images ID
static const std::string MARIO_ID = "mario";
static const std::string SURPRISE_BLOCK_ID = "surpriseBlockSprites";
static const std::string NORMAL_BLOCK_ID = "normalBlock";
static const std::string KOOPA_GREEN_ID = "koopaGreen";
static const std::string KOOPA_RED_ID = "koopaRed";
static const std::string KOOPA_DARK_ID = "koopaDark";
static const std::string FLYING_KOOPA_ID = "flyingKoopa";
static const std::string GOOMBA_ID = "goomba";
static const std::string GOOMBA_DARK_ID = "goombaDark";
static const std::string COIN_ID = "coinsSprites";

//Default images paths
static const std::string DEFAULT_MARIO_PATH = "Sprites/Default/defaultPlayer.png";
static const std::string DEFAULT_TURTLE_PATH = "Sprites/Default/defaultKoopa.png";
static const std::string DEFAULT_MUSHROOM_PATH = "Sprites/Default/defaultGoomba.png";
static const std::string DEFAULT_PLATFORM_PATH = "Sprites/Default/defaultBlock.png";
static const std::string DEFAULT_COIN_PATH = "Sprites/Default/defaultCoins.png";

class Factory {
public:
    static Factory * getInstance();
    std::vector<GameObject*> createGameObjectsFromLevelConfig(Level levelConfig);
    virtual ~Factory();

private:
    static Factory * instance;

    Factory();
};


#endif //TPTALLER1_FACTORY_H
