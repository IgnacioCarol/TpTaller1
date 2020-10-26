//
// Created by Daniel Bizari on 25/10/2020.
//

#ifndef TPTALLER1_FACTORY_H
#define TPTALLER1_FACTORY_H

#include <vector>
#include "../GameObject.h"
#include "../config/Config.h"

class Factory {
    public:
        static Factory * getInstance();
        std::vector<GameObject> createGameObjectsFromLevelConfig(Level levelConfig);
        virtual ~Factory();

    private:
        static Factory * instance;

        Factory();

};


#endif //TPTALLER1_FACTORY_H
