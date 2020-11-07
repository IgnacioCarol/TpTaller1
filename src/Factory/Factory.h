#ifndef TPTALLER1_FACTORY_H
#define TPTALLER1_FACTORY_H

#include <vector>
#include "../gameobjects/GameObject.h"
#include "../config/Config.h"

class Factory {
public:
    static Factory * getInstance();
    std::vector<GameObject*> createGameObjectsFromLevelConfig(Level levelConfig);
    virtual ~Factory();

private:
    static Factory * instance;

    Factory();

    //Images ID
    const std::string MARIO_ID = "mario";
    const std::string SURPRISE_BLOCK_ID = "surpriseBlockSprites";
    const std::string NORMAL_BLOCK_ID = "normalBlock";
    const std::string KOOPA_GREEN_ID = "koopaGreen";
    const std::string KOOPA_RED_ID = "koopaRed";
    const std::string KOOPA_DARK_ID = "koopaDark";
    const std::string FLYING_KOOPA_ID = "flyingKoopa";
    const std::string GOOMBA_ID = "goomba";
    const std::string GOOMBA_DARK_ID = "goombaDark";
};


#endif //TPTALLER1_FACTORY_H
