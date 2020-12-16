#include "GameClient.h"


GameClient* GameClient::instance = 0;

GameClient::GameClient() {
}

GameClient *GameClient::Instance() {
    if(instance == 0) instance = new GameClient();
    return instance;
}

bool GameClient::init(GameMsgParams initialize, const char* username) {
    this -> textureManager = TextureManager::Instance();
    clientUsername = username;
    int cameraWidth = initialize.camera.width;
    int cameraHeight = initialize.camera.height;
    camera = new Camera(initialize.camera.xPos, initialize.camera.yPos, cameraWidth, cameraHeight);
    std::vector<GameObjectInit> players;

    for (auto& gameObject: initialize.gameObjectsInit.gameObjects) {
        if (gameObject.type == GOT_PLAYER) {
            players.push_back(gameObject);
        }
    }

    //SDL initializing
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    if (!SDL_Init(SDL_INIT_EVERYTHING)){
        logger -> info("SDL init success");
        window = SDL_CreateWindow(clientUsername.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

    if (!this -> loadTexts(initialize.stage.isDefault, players)){
        logger -> error("Error: Loading the sprites went wrong");
        return false;
    }

    if(!this -> createGameObjects(initialize.gameObjectsInit)){ //ToDo volar despues estos ifs
        logger -> error("Cannot create the objects in the client");
        return false;
    }

    initBackground(renderer, initialize.stage);
    logger -> info("Init success");
    return true;
}

void GameClient::render() {
    SDL_RenderClear(renderer);
    background -> renderBackground(camera -> getCamera());
    for (int objectID: idsToRender){
        gameObjectsMap[objectID] -> draw(renderer, camera->getXpos(), 0);
    }

    renderPlayers();
    if (serverIsDown) {
        textureManager->printText(TEXT_SERVER_DISCONNECTED_KEY, 200, 520, renderer);
    }

    if (levelCompleted){
        textureManager->printText(TEXT_LEVEL_COMPLETED, 300, 300, renderer);
    }

    SDL_RenderPresent(renderer);
}

void GameClient::renderPlayers() {
    int playerUsernameYPos = 40;
    Player* clientPlayer;
    for (std::pair<int, Player*> element: playersMap){
        if (clientUsername == element.second->getUsername()){
            clientPlayer = element.second;
            continue;
        }
        element.second -> draw(renderer, camera->getXpos(), 0);
        TextureManager::Instance()->printText(element.second->getTextureId() + "_text", 20, playerUsernameYPos, renderer);
        playerUsernameYPos += 20;
    }
    clientPlayer -> draw(renderer, camera -> getXpos(), 0);
    TextureManager::Instance()->printText(clientPlayer->getTextureId() + "_text", 20, 20, renderer);
}

void GameClient::update(GameMsgPlaying updateObjects) {
    updatePlayers(updateObjects.players);
    updateGameObjects(updateObjects.gameObjects);

    //Update camera position and timer
    camera -> setXPos(updateObjects.camera.xPos);
    background->setCurrentTime(updateObjects.timer);
}

void GameClient::updatePlayers(std::vector<GamePlayerPlaying> players) {
    for (GamePlayerPlaying playerUpdate: players){
        Player* player = playersMap[playerUpdate.id];
        if (clientUsername == player->getUsername() && playerUpdate.xPos >= levelLimit) levelCompleted = true;
        player -> setPosition(playerUpdate.xPos, playerUpdate.yPos);
        player -> setDirection(playerUpdate.direction);
        player -> setState(playerUpdate.state);
    }
}

void GameClient::updateGameObjects(std::vector<GameObjectPlaying> gameObjects) {
    idsToRender.clear();
    for (GameObjectPlaying gameObjectUpdate: gameObjects){
        idsToRender.push_back(gameObjectUpdate.id);
        GameObject* gameObject = gameObjectsMap[gameObjectUpdate.id];
        gameObject->setPosition(gameObjectUpdate.xPos, gameObjectUpdate.yPos);
        gameObject->setState(gameObjectUpdate.state);
        gameObject->setDirection(gameObjectUpdate.direction);
    }
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

bool GameClient::loadTexts(bool isDefault, std::vector<GameObjectInit> players) {
    bool success = textureManager->loadText(TEXT_WORLD_LEVEL_LABEL_KEY, TEXT_WORLD_LEVEL_LABEL_VALUE, WHITE_COLOR, renderer);
    success = success && textureManager->loadText(TEXT_TIMER_LABEL_KEY, TEXT_TIMER_LABEL_VALUE, WHITE_COLOR, renderer);
    if (isDefault) {
        success = success && textureManager->loadText(TEXT_DEFAULT_BACKGROUND_KEY, TEXT_DEFAULT_BACKGROUND_VALUE, WHITE_COLOR, renderer);
    }
    for (auto& player : players) {
        if (player.imageId == "player0") {
            success = success && textureManager->loadText("player0_text", player.username, RED_COLOR, renderer);
        } else if (player.imageId == "player1") {
            success = success && textureManager->loadText("player1_text", player.username, GREEN_COLOR, renderer);
        } else if (player.imageId == "player2") {
            success = success && textureManager->loadText("player2_text", player.username, YELLOW_COLOR, renderer);
        } else if (player.imageId == "player3") {
            success = success && textureManager->loadText("player3_text", player.username, PURPLE_COLOR, renderer);
        }
    }
    success = success && textureManager->loadText(TEXT_SERVER_DISCONNECTED_KEY, TEXT_SERVER_DISCONNECTED_VALUE, BLACK_COLOR, renderer);
    success = success && textureManager->loadText(TEXT_LEVEL_COMPLETED, TEXT_LEVEL_COMPLETED_VALUE, WHITE_COLOR, renderer);

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
    Player* tmpPlayer = new Player(camera -> getCamera(), player.username, player.imageId);
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
    background -> setBackgroundID("BG" + std::to_string(stage.level));
    background -> setLevel(stage.level);
    background -> setCurrentTime(stage.timer);
    levelLimit = stage.levelLimit;
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

void GameClient::gameOver() {
    playing = false;
}

void GameClient::changeLevelBackground(StageInit nextLevelConfig) {
    background->setLevel(nextLevelConfig.level);
    background->setCurrentTime(nextLevelConfig.timer);
    background->setBackgroundID("BG" + std::to_string(nextLevelConfig.level));
    background -> isDefaultBackground(nextLevelConfig.isDefault);
    levelLimit = nextLevelConfig.levelLimit;
}

void GameClient::changeLevel(GameMsgLevelChange nextLevelConfig) {
    for (std::pair<int, Player*> player: playersMap){
        player.second->restartPos(0, 380);
        player.second->setDirection(true);
        player.second->changeState(new Normal(0, player.second->getFrameAmount()));
    }
    levelCompleted = false;
    changeLevelBackground(nextLevelConfig.stage);
    camera->restartPos();
    //Deleting the gameObjects of the current level
    for (std::pair<int, GameObject*> gameObject: gameObjectsMap){
        delete gameObject.second;
    }
    gameObjectsMap.clear();
    idsToRender.clear();
    createGameObjects(nextLevelConfig.gameObjectsInit);
}

void GameClient::setServerDown() {
    serverIsDown = true;
}
