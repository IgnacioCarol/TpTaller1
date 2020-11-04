#include "Game.h"

Game* Game::instance = 0;
const static char* BACKGROUND = "BG";
int IMAGE_WIDTH;
Logger* logger = Logger::getInstance();

Game::Game(){
}

Game* Game::Instance() {
    //First time we create an instance of Game
    if(instance == 0) instance = new Game();
    return instance;
}


bool Game::init(const char *levelName, int width, int height) {
    camera = new Camera(0, 0, width, height);
    config->load("./resources/config.xml");
    Window windowConfig = config->getWindow();
    Logger::getInstance()->setLogLevel(config->getLog().level);

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
    SDL_Delay(2);
    SDL_RenderClear(renderer);
    camera->render(player->getXPosition(), stage->getWidth());
    textureManager->drawBackgroundWithCamera(800, 600, renderer, camera->getCamera());
    player->draw(renderer, camera -> getXpos(), 0);

    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        _gameObjects[i]->draw(renderer, camera->getXpos(), 0);
    }
    stage->renderLevel();
    stage->renderTime();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    logger ->info("Cleaning game\n");
    //delete Logger::getInstance();
    // ToDo liberar memoria de todos los singleton.
    
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    textureManager->clearTextureMap();
    SDL_Quit();
}

void Game::handleEvents() {
    player->move();
    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        _gameObjects[i]->move();
    }
}

bool Game::loadImages() {
    bool success = textureManager -> load(renderer);
    return success;
}

bool Game::loadTexts() {
    bool success = textureManager->loadText(TEXT_WORLD_LEVEL_LABEL_KEY, TEXT_WORLD_LEVEL_LABEL_VALUE, WHITE_COLOR, renderer);
    success = success && textureManager->loadText(TEXT_TIMER_LABEL_KEY, TEXT_TIMER_LABEL_VALUE, WHITE_COLOR, renderer);
    return success;
}

void Game::createGameObjects() {
    player = new Player(camera->getCamera());
    initializeGameObjects(1);

}
void Game::nextStage() {
    BackgroundStage *currentStage = this->stage;
    stage = stage->nextStage();
    Logger::getInstance()->info("Stage changed");

    cleanGameObjects();
    initializeGameObjects(stage->getLevel());
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



