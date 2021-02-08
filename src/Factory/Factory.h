#ifndef TPTALLER1_FACTORY_H
#define TPTALLER1_FACTORY_H

#include <vector>
#include "../gameobjects/Player.h"
#include "../gameobjects/GameObject.h"
#include "../config/Config.h"
#include "../Server/PlayerClient.h"

//Images ID
static const std::string MARIO_ID = "mario";
static const std::string LUIGI_ID = "luigi";
static const std::string WARIO_ID = "wario";
static const std::string WALUIGI_ID = "waluigi";
static const std::string SURPRISE_BLOCK_ID = "surpriseBlockSprites";
static const std::string NORMAL_BLOCK_ID = "normalBlock";
static const std::string KOOPA_GREEN_ID = "koopaGreen";
static const std::string KOOPA_RED_ID = "koopaRed";
static const std::string KOOPA_DARK_ID = "koopaDark";
static const std::string FLYING_KOOPA_ID = "flyingKoopa";
static const std::string GOOMBA_ID = "goomba";
static const std::string GOOMBA_DARK_ID = "goombaDark";
static const std::string COIN_ID = "coinsSprites";
static const std::string HOLE_ID = "hole";
static const std::string PIPE_ID = "pipe";

//Players images paths
static const std::string PLAYER_0_IMG_PATH = "Sprites/Players/mario.png";
static const std::string PLAYER_1_IMG_PATH = "Sprites/Players/luigi.png";
static const std::string PLAYER_2_IMG_PATH = "Sprites/Players/wario.png";
static const std::string PLAYER_3_IMG_PATH = "Sprites/Players/waluigi.png";

//Default images paths
static const std::string DEFAULT_PLAYER_PATH = "Sprites/Default/defaultPlayer.png";
static const std::string DEFAULT_TURTLE_PATH = "Sprites/Default/defaultKoopa.png";
static const std::string DEFAULT_MUSHROOM_PATH = "Sprites/Default/defaultGoomba.png";
static const std::string DEFAULT_PLATFORM_PATH = "Sprites/Default/defaultBlock.png";
static const std::string DEFAULT_PIPE_PATH = "Sprites/Default/defaultBlock.png"; // ToDo DB agregar path default para pipe
static const std::string DEFAULT_COIN_PATH = "Sprites/Default/defaultCoins.png";


class Factory {
public:
    static Factory * getInstance();
    std::vector<GameObject*> createGameObjectsFromLevelConfig(Level levelConfig);
    std::vector<Player*>  createPlayers(std::vector<PlayerClient*> clients);
    virtual ~Factory();

private:
    static Factory * instance;

    Factory();
};


#endif //TPTALLER1_FACTORY_H
