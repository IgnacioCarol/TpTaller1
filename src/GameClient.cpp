#include <src/gameobjects/EnemyTurtle.h>
#include <src/gameobjects/PlatformNormal.h>
#include <src/gameobjects/PlatformSurprise.h>
#include <src/gameobjects/Coin.h>
#include <src/gameobjects/EnemyMushroom.h>
#include <src/BackgroundStages/FirstStage.h>
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

bool GameClient::init(InitializeGameMsg initialize) {
    this -> textureManager = TextureManager::Instance();
    int cameraWidth = initialize.camera.width;
    int cameraHeight = initialize.camera.height;
    camera = new Camera(initialize.camera.xPos, initialize.camera.yPos, cameraWidth, cameraHeight);

    /*std::map<std::string, std::vector<std::string>> paths;
    CameraInit camera;
    StageInit stage;
    GameObjectsInit gameObjects;*/

    if(!this -> loadImages(initialize.paths)){ //cargo las imagenes
        logger -> error("Cannot load the images in the client"); //TODO mejorar estos logs
        return false;
    }

    if(!this -> createGameObjects(initialize.gameObjects)){ //ToDo volar despues estos ifs
        logger -> error("Cannot create the objects in the client");
        return false;
    }

    //SDL initializing
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    if (!SDL_Init(SDL_INIT_EVERYTHING)){
        logger -> info("SDL init success");
        window = SDL_CreateWindow("Mario Bros", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  cameraWidth, cameraHeight, 0);
        if (window){
            logger -> info("Window init success");
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer){
                //ToDo aca tengo que dibujar la cosa
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
    for (std::pair<int, Player*> element: playersMap){
        element.second -> draw(renderer, camera->getXpos(), 0);
    }
    //Drawing the other gameObjects
    for (std::pair<int, GameObject*> element: gameObjectsMap){
        element.second -> draw(renderer, camera->getXpos(), 0);
    }

    //ToDo falta dibujar los textos
}

void GameClient::update() { //ToDo por ahora solo actualizamos las cosas de los jugadores ya que no hay colisiones y esas cosas
    //Aca solo recibo las cosas que cambian de posicion



}

GameClient::~GameClient() {

}

bool GameClient::createGameObjects(GameObjectsInit gameObjectsInit) {
    for (GameObjectInit gameObject: gameObjectsInit.gameObjects){
        ObjectType type = gameObject.type;
        if (type == TURTLE || type == MUSHROOM){
            createEnemie(gameObject, type);
        }
        else if (type == PLAYER){
            createPlayer(gameObject);
        }
        else{
            createStaticObject(gameObject, type);
        }
    }
}

bool GameClient::loadImages(std::map<std::string, std::vector<std::string>> imagePaths) {
    for (std::pair<std::string, std::vector<std::string>> element : imagePaths){
        textureManager -> addPath(element.first, element.second[0], element.second[1]);
    }

    if (!textureManager -> loadImages(renderer)) return false;

    //ToDo deberian cargarse los textos tmb

    return true;
}

bool GameClient::loadTexts(StageInit stageInit) {
    bool success = textureManager->loadText(TEXT_WORLD_LEVEL_LABEL_KEY, TEXT_WORLD_LEVEL_LABEL_VALUE, WHITE_COLOR, renderer);
    success = success && textureManager->loadText(TEXT_TIMER_LABEL_KEY, TEXT_TIMER_LABEL_VALUE, WHITE_COLOR, renderer);
    /*if (stageInit.default) {
        success = success && textureManager->loadText(TEXT_DEFAULT_BACKGROUND_KEY, TEXT_DEFAULT_BACKGROUND_VALUE, WHITE_COLOR, renderer);
    }*/
    return success;
}

void GameClient::createEnemie(GameObjectInit enemy, ObjectType enemyType) {
    Enemy* tmpEnemy;
    if (enemyType == TURTLE) {
        tmpEnemy = new EnemyTurtle();
    }
    else {
        tmpEnemy = new EnemyMushroom();
    }
    if (tmpEnemy != nullptr){
        tmpEnemy -> init(enemy.xPos, enemy.yPos, enemy.imageId, camera -> getCamera(),
                         new EnemyMovement(0, enemy.frameAmount));
        gameObjectsMap[enemy.id] = tmpEnemy;
    }
}

void GameClient::createPlayer(GameObjectInit player) {
    Player* tmpPlayer = new Player(camera -> getCamera());
    tmpPlayer->init(player.xPos, player.yPos, player.imageId, camera->getCamera(), player.frameAmount);
    playersMap[player.id] = tmpPlayer;
}


void GameClient::createStaticObject(GameObjectInit gameObject, ObjectType objectType) {
    GameObject* tmp;
    if (gameObject.type == COIN){
        tmp = new Coin();
    }
    else if (gameObject.type == NORMAL_PLATFORM){
        tmp = new PlatformNormal();
    }
    else{
        tmp = new PlatformSurprise();
    }

    if (tmp != nullptr){
        tmp -> init(gameObject.xPos, gameObject.yPos, gameObject.imageId);
        gameObjectsMap[gameObject.id] = tmp;
    }
}


