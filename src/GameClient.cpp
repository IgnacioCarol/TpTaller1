//
// Created by lisandro on 6/12/20.
//

#include <src/gameobjects/EnemyTurtle.h>
#include <src/gameobjects/PlatformNormal.h>
#include <src/gameobjects/PlatformSurprise.h>
#include <src/gameobjects/Coin.h>
#include <src/gameobjects/EnemyMushroom.h>
#include "GameClient.h"
#include "CharacterStates/EnemyMovement.h"
#include "CharacterStates/Normal.h"
#include "Camera.h"

GameClient* GameClient::instance = 0;

GameClient::GameClient() {
}

GameClient *GameClient::Instance() {
    if(instance == 0) instance = new GameClient();
    return instance;
}

//esto se hace antes de entrar al loop del run
bool GameClient::init(nlohmann::json message) { //aca el cliente lo que hace es llamar al init y le pasa lo que recibio el cliente
    this -> textureManager = TextureManager::Instance();
    int windowWidth = 800;  //TODO por ahora hardcodeado despues sacarlo del json
    int windowHeight = 600;
    camera = new Camera(0, 0, windowWidth, windowHeight);

    if(!this -> loadImages(message)){ //cargo las imagenes
        logger -> error("Cannot load the images in the client"); //TODO mejorar estos logs
        return false;
    }
    if(!this -> createGameObjects(message)){ //aca se crean todos los gameObjects y se los pone en el diccionario
        logger -> error("Cannot create the objects in the client");
        return false;
    }


    //SDL initializing
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    if (!SDL_Init(SDL_INIT_EVERYTHING)){
        logger -> info("SDL init success");
        window = SDL_CreateWindow("Mario Bros", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  windowWidth, windowHeight, 0);
        if (window){
            logger -> info("Window init success");
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer){
                stage = new FirstStage(textureManager, renderer); //TODO ver que onda esto
                logger -> info("Renderer init success");
            }

            else {
                logger -> error("Render init fail");
                return false;
            }
        }
        else{
            logger -> error("Window init fail");
            return false;
        }
    }
    else{
        logger -> error("SDL init fail");
        return false;
    }

    logger -> info("Init success");
    playing = true; //Ver que onda esto despues, como carajo te avisa el back que termino
    return true;
}

void GameClient::render() {
    //Drawing the players
    for (std::pair<std::string, Player*> element: playersMap){
        element.second -> draw(renderer, camera->getXpos(), 0);
    }
    //Drawing the other gameObjects
    for (std::pair<std::string, GameObject*> element: gameObjectsMap){
        element.second -> draw(renderer, camera->getXpos(), 0);
    }
}

void GameClient::update(nlohmann::json message) { //esto se hace dentro del loop del run
    //Recibo por parametro la info para updatear las posiciones de todas las cosas
    //faltan hacer los sets para todos los objetos, solo voy a recibir los que cambian de posicion (creo)

}

GameClient::~GameClient() {

}

bool GameClient::createGameObjects(nlohmann::json message) {
    Enemy* tmpEnemy;
    Player* tmpPlayer;
    GameObject * tmp;

    if (message.contains("enemyTurtle")){
        for(auto element: message["enemyTurtle"].get<std::map<std::string, std::vector<std::string>>>()){
            tmpEnemy = new EnemyTurtle();

            tmpEnemy->init(std::stoi(element.second[0]), std::stoi(element.second[1]),
                           element.second[2], camera ->getCamera(),
                           new EnemyMovement(0, std::stoi(element.second[3])));

            gameObjectsMap[element.first] = tmpEnemy;
        }
    }

    if (message.contains("enemyMushroom")){
        for(auto element: message["enemyMushroom"].get<std::map<std::string, std::vector<std::string>>>()){
            tmpEnemy = new EnemyMushroom();

            tmpEnemy->init(std::stoi(element.second[0]), std::stoi(element.second[1]),
                           element.second[2], camera ->getCamera(),
                           new EnemyMovement(0, std::stoi(element.second[3])));

            gameObjectsMap[element.first] = tmpEnemy;
        }
    }

    if (message.contains("players")){
        for(auto element: message["players"].get<std::map<std::string, std::vector<std::string>>>()){
            tmpPlayer = new Player(camera -> getCamera());

            tmpPlayer->init(std::stoi(element.second[0]), std::stoi(element.second[1]),
                           element.second[2], camera ->getCamera(), std::stoi(element.second[3]));

            playersMap[element.first] = tmpPlayer;
        }
    }

    if (message.contains("platformSurprise")){
        for(auto element: message["platformSurprise"].get<std::map<std::string, std::vector<std::string>>>()){
            tmp = new PlatformSurprise();

            tmp->init(std::stoi(element.second[0]), std::stoi(element.second[1]), element.second[2]);

            gameObjectsMap[element.first] = tmp;
        }
    }

    if (message.contains("normalPlatform")){
        for(auto element: message["normalPlatform"].get<std::map<std::string, std::vector<std::string>>>()){
            tmp = new PlatformNormal();

            tmp->init(std::stoi(element.second[0]), std::stoi(element.second[1]), element.second[2]);

            gameObjectsMap[element.first] = tmp;
        }
    }

    if (message.contains("Coins")){
        for(auto element: message["Coins"].get<std::map<std::string, std::vector<std::string>>>()){
            tmp = new Coin();

            tmp->init(std::stoi(element.second[0]), std::stoi(element.second[1]), element.second[2]);

            gameObjectsMap[element.first] = tmp;
        }
    }

}

bool GameClient::loadImages(nlohmann::json message) {//aca ya se cargaron los ids y toda la bola, por id me refiero a mario
    std::map<std::string, std::vector<std::string>> imagePaths = message["paths"].get<std::map<std::string, std::vector<std::string>>>();
    for (std::pair<std::string, std::vector<std::string>> element : imagePaths){
        textureManager -> addPath(element.first, element.second[0], element.second[1]);
    }

    if (!textureManager -> loadImages(renderer)) return false;

    //ToDo deberian cargarse los textos tmb

    return true;
}


