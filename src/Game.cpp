#include "Game.h"
#include "BackgroundStages/FirstStage.h"
#include "BackgroundStages/BackgroundStage.h"

#include "Factory/Factory.h"
#include "logger/logger.h"
#include "config/Config.h"
#include "gameobjects/Coin.h"

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
    Config * config = new Config();
    config->load("test/resources/config_test.xml");
//    config->getStage(); //ToDo handlear init de stage
    Window windowConfig = config->getWindow();
    Logger::getInstance()->setLogLevel(config->getLog().level);
    _gameObjects = Factory::getInstance()->createGameObjectsFromLevelConfig(config->getStage().levels.at(0)); //ToDo Asumo que el 0 contiene el level inicial, chequear!!

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    //SDL initializing
    if (!SDL_Init(SDL_INIT_EVERYTHING)){
        logger -> info("SDL init success\n");
        window = SDL_CreateWindow(levelName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  windowConfig.width, windowConfig.height, 0);
        if (window){
            logger -> info("Window init success\n");
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer){
                camera = new Camera(0, 0, width, height);
                stage = new FirstStage(textureManager, renderer);
                logger -> info("Renderer init success\n");
            }

            else {
                logger -> error("Render init fail\n");
                return false;
            }
        }
        else{
            logger -> error("Window init fail\n");
            return false;
        }
    }
    else{
        logger -> error("SDL init fail\n");
        return false;
    }

    logger -> info("Init success\n");
    playing = true;
    return true;
}

void Game::render() {
    SDL_RenderClear(renderer);
    camera->render(player->getXPosition(), stage->getWidth());
    textureManager->drawBackgroundWithCamera(800, 600, renderer, camera->getCamera());
    player->draw(renderer, camera -> getXpos(), 0);
    //TODO renderizar todos los game objects iterando
    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        _gameObjects[i]->draw(renderer, 0, 0);
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    logger ->info("Cleaning game\n");
    printf("Cleaning game");
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    _gameObjects.clear();
    textureManager->clearTextureMap();

    delete Logger::getInstance();
    // ToDo liberar memoria de todos los singleton.
    delete Factory::getInstance();
    delete TextureManager::Instance();

    SDL_Quit();
}

void Game::handleEvents() {
    player->move();
}

bool Game::loadImages() {
    bool success;
    success = textureManager->load("Sprites/sprites_prueba/dino.png", "dino", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/dog.png", "dog", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/RunDog.png", "runDog", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/coinsSprites.png", coinsID, renderer);
    return success;
}

void Game::createGameObjects() {
    auto* mario = new Player();
    mario->init(0, 403, "dino", 0, camera->getCamera(), 5);
    player = mario;
}

void Game::nextStage() {
    stage = stage->nextStage();
}

void Game::restartCharacters() {
    player->restartPos(0, 403);
    camera->restartPos();
}

void Game::update() {
}



