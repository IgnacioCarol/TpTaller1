#include "Game.h"

Game* Game::instance = 0;
Logger* logger = Logger::getInstance();

Game::Game(){
}

Game* Game::Instance() {
    //First time we create an instance of Game
    if(instance == 0) instance = new Game();
    return instance;
}


bool Game::init(const char *levelName, std::string xmlPath) {
    config->load(xmlPath);

    this->textureManager = TextureManager::Instance();
    camera = new Camera(0, 0, config->getWindow().width, config->getWindow().height);
    Window windowConfig = config->getWindow();

    //SDL initializing
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    if (!SDL_Init(SDL_INIT_EVERYTHING)){
        logger -> info("SDL init success");
        window = SDL_CreateWindow(levelName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  windowConfig.width, windowConfig.height, 0);
        if (window){
            logger -> info("Window init success");
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer){
                stage = new FirstStage(textureManager, renderer);
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
    playing = true;
    return true;
}

Game::~Game() {
    delete this->player;
    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        delete _gameObjects[i];
    }
    Logger::getInstance()->info("All Game Objects were deleted");
    delete this->camera;
    Logger::getInstance()->info("The camera was deleted");
    delete this->config;
    Logger::getInstance()->info("The parser(config) was deleted");
    delete this->factory;
    Logger::getInstance()->info("The Factory was deleted");
    delete this->textureManager;
    Logger::getInstance()->info("Texture Manager was deleted");
}

void Game::render() {
    SDL_RenderClear(renderer);
    camera->update(players, stage->getWidth());
    textureManager->drawBackgroundWithCamera(800, 600, "BG", renderer, camera->getCamera());
    player->draw(renderer, camera -> getXpos(), 0);

    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        _gameObjects[i]->draw(renderer, camera->getXpos(), 0);
    }
    stage->renderLevel();
    stage->renderTime();
    if (config->isDefault()) {
        stage->renderDefaultBackground();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    logger ->info("Cleaning game\n");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    textureManager->clearTextureMap();
    SDL_Quit();
}

void Game::handleEvents() {
    for (Player* element : players){
        element->move(std::vector<int>());
    }
    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        _gameObjects[i]->move();
    }
}

bool Game::loadImages() {
    bool success = TextureManager::Instance() -> loadImages(renderer);
    return success;
}

bool Game::loadTexts() {
    bool success = textureManager->loadText(TEXT_WORLD_LEVEL_LABEL_KEY, TEXT_WORLD_LEVEL_LABEL_VALUE, WHITE_COLOR, renderer);
    success = success && textureManager->loadText(TEXT_TIMER_LABEL_KEY, TEXT_TIMER_LABEL_VALUE, WHITE_COLOR, renderer);
    if (config->isDefault()) {
        success = success && textureManager->loadText(TEXT_DEFAULT_BACKGROUND_KEY, TEXT_DEFAULT_BACKGROUND_VALUE, WHITE_COLOR, renderer);
    }
    return success;
}

void Game::createGameObjects() {
    player = new Player(camera->getCamera(), "mario");
    players.push_back(player);
    TextureManager::Instance() -> addPath("mario", imgPlayer, defaultPlayer); //ToDo ver como hacer para conseguir los paths de mario sin usar los define que tiene
    initializeGameObjects(1);
}
void Game::nextStage() {
    BackgroundStage *currentStage = this->stage;
    stage = stage->nextStage();
    if (currentStage != stage) {
        Logger::getInstance()->info("Stage changed");

        cleanGameObjects();
        initializeGameObjects(stage->getLevel());
        loadImages();
    }
    delete currentStage;
}

void Game::restartCharacters() {
    Logger::getInstance()->info("Restarting Player and Camera position");
    player->restartPos(0, 380);
    camera->restartPos();
}

void Game::update() {
}
bool Game::isPlaying() const {
    return this->playing && !this->stage->isTimeOver();
}

SDL_Rect *Game::getCamera() {
    return camera -> getCamera();
}

void Game::cleanGameObjects() {
    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        delete _gameObjects[i];
    }
    _gameObjects.clear();
}

void Game::initializeGameObjects(int level) {
    _gameObjects = factory->createGameObjectsFromLevelConfig(config->getLevel(level));
}

void Game::play(std::string xmlPath) {
    if (!init("Level 1", xmlPath)) { //Aca inicializo el background
        Logger::getInstance() -> error("Error: the game could not be initialized");
        throw exception();
    }

    createGameObjects(); //ToDo refactorizar y mover al Factory, factory tiene que ser el unico responsable de instanciar gameObjects

    if (!loadImages()){
        Logger::getInstance() -> error("Error: Loading the sprites went wrong");
        throw exception();
    }
    if (!loadTexts()) {
        Logger::getInstance()->error("Error: Loading texts went wrong");
        throw exception();
    }

    //Event handler
    SDL_Event e;

    while(isPlaying()){

        while(SDL_PollEvent(&e) != 0) {
            if (e.type  == SDL_QUIT ) {
                Game::Instance()->gameOver();
            }
        }

        handleEvents();
        render();
        SDL_Delay(4);
    }
    Logger::getInstance() -> info("Game over");
    clean();
}



