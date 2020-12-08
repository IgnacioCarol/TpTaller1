#include "gameobjects/EnemyTurtle.h"
#include "gameobjects/PlatformNormal.h"
#include "gameobjects/PlatformSurprise.h"
#include "gameobjects/Coin.h"
#include "gameobjects/EnemyMushroom.h"
#include "BackgroundStages/FirstStage.h"
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

bool GameClient::init(GameMsgParams initialize) {
    this -> textureManager = TextureManager::Instance();
    int cameraWidth = initialize.camera.width;
    int cameraHeight = initialize.camera.height;
    camera = new Camera(initialize.camera.xPos, initialize.camera.yPos, cameraWidth, cameraHeight);

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

    if(!this -> loadImages(initialize.paths)){ //cargo las imagenes
        logger -> error("Cannot load the images in the client"); //TODO mejorar estos logs o volarlos
        return false;
    }

    if (!this -> loadTexts(initialize.stage.isDefault)){
        logger -> error("Error: Loading the sprites went wrong");
        return false;
    }

    if(!this -> createGameObjects(initialize.gameObjectsInit)){ //ToDo volar despues estos ifs
        logger -> error("Cannot create the objects in the client");
        return false;
    }

    initBackground(renderer, initialize.stage);
    logger -> info("Init success");
    playing = true; //Ver que onda esto despues, como carajo te avisa el back que termino
    return true;
}

void GameClient::render() {
    SDL_RenderClear(renderer);
    background -> renderBackground(camera -> getCamera()); //ToDo que imprima los usernames
    //Drawing the players
    for (std::pair<int, Player*> element: playersMap){
        element.second -> draw(renderer, camera->getXpos(), 0);
    }
    //Drawing the other gameObjects
    for (std::pair<int, GameObject*> element: gameObjectsMap){
        element.second -> draw(renderer, camera->getXpos(), 0);
    }

    SDL_RenderPresent(renderer);
}

void GameClient::update(GameMsgPlaying initialize) { //ToDo por ahora solo actualizamos las cosas de los jugadores ya que no hay colisiones y esas cosas
    //Aca solo recibo las cosas que cambian de posicion
    /*if (initialize.stage.changeLevel){
        //codigo para hacer el cambio de level, no ejecuto lo que sigue
    }*/
    updatePlayers(initialize.players);


    for (std::pair<int, GameObject*> gameObject: gameObjectsMap){ //Muevo todos los objetos distintos a player
        gameObject.second -> move();
    }

    //Update camera position
    camera -> setXPos(initialize.camera.xPos);
}

bool GameClient::createGameObjects(GameObjectsInit gameObjectsInit) {
    for (GameObjectInit gameObject: gameObjectsInit.gameObjects){
        GameObjectType type = gameObject.type;
        if (type == GOT_ENEMY_TURTLE || type == GOT_ENEMY_MUSHROOM){
            createEnemy(gameObject, type);
        }
        else if (type == GOT_PLAYER){ //ToDo agregar algo aca para guardar los usernames
            createPlayer(gameObject);
        }
        else{
            createStaticObject(gameObject, type);
        }
    }
    return true;
}

bool GameClient::loadImages(std::map<std::string, std::vector<std::string>> imagePaths) {
    for (std::pair<std::string, std::vector<std::string>> element : imagePaths){
        textureManager -> addPath(element.first, element.second[0], element.second[1]);
    }
    if (!textureManager -> loadImages(renderer)) return false;
    return true;
}

bool GameClient::loadTexts(bool isDefault) {
    bool success = textureManager->loadText(TEXT_WORLD_LEVEL_LABEL_KEY, TEXT_WORLD_LEVEL_LABEL_VALUE, WHITE_COLOR, renderer);
    success = success && textureManager->loadText(TEXT_TIMER_LABEL_KEY, TEXT_TIMER_LABEL_VALUE, WHITE_COLOR, renderer);
    if (isDefault) {
        success = success && textureManager->loadText(TEXT_DEFAULT_BACKGROUND_KEY, TEXT_DEFAULT_BACKGROUND_VALUE, WHITE_COLOR, renderer);
    }
    return success;
}

void GameClient::createEnemy(GameObjectInit enemy, GameObjectType enemyType) {
    Enemy* tmpEnemy;
    if (enemyType == GOT_ENEMY_TURTLE) {
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
    Player* tmpPlayer = new Player(camera -> getCamera(), player.username);
    tmpPlayer->init(player.xPos, player.yPos, player.imageId, camera->getCamera(), player.frameAmount);
    playersMap[player.id] = tmpPlayer;
}

void GameClient::createStaticObject(GameObjectInit gameObject, GameObjectType objectType) {
    GameObject* tmp;
    if (gameObject.type == GOT_COIN){
        tmp = new Coin();
    }
    else if (gameObject.type == GOT_PLATFORM_NORMAL){
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

void GameClient::initBackground(SDL_Renderer* renderer, StageInit stage) {
    background = new BackgroundStage(textureManager, renderer);
    background -> isDefaultBackground(stage.isDefault);
    background -> setBackgroundID("BG1");
    background -> setLevel(stage.level);
    background -> setCurrentTime(stage.timer);
}

void GameClient::updatePlayers(GameObjectsInit initialize) {
    for (GameObjectInit gameObject: initialize.gameObjects){
        Player* player = playersMap[gameObject.id];
        player -> setPosition(gameObject.xPos, gameObject.yPos);
        //player -> setDirection(gameObject.direction);
        //player -> setState(gameObject.state); //ToDo una vez que se agreguen estas cosas sacarlo
    }
}

GameClient::~GameClient() {
    for (std::pair<int, Player*> players: playersMap){
        delete players.second;
    }
    Logger::getInstance()->info("All Players were deleted");

    for(std::pair<int, GameObject*> gameObjects: gameObjectsMap) {
        delete gameObjects.second;
    }
    Logger::getInstance()->info("All Game Objects were deleted");

    delete background;
    Logger::getInstance()->info("The background was deleted");

    delete this->camera;
    Logger::getInstance()->info("The camera was deleted");

    delete this->textureManager;
    Logger::getInstance()->info("Texture Manager was deleted");
}

void GameClient::clean() {
    logger ->info("Cleaning game\n");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    textureManager->clearTextureMap();
    SDL_Quit();
}

bool GameClient::isPlaying() {
    return playing;
}
