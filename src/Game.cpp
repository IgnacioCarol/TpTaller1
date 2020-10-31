#include "Game.h"
#include "BackgroundStages/FirstStage.h"
#include "BackgroundStages/BackgroundStage.h"

#include "Factory/Factory.h"
#include "logger/logger.h"
#include "config/Config.h"
#include "gameobjects/Coin.h"
#include "src/CharacterStates/EnemyMovement.h"

#include "src/gameobjects/PlatformNormal.h"
#include "src/gameobjects/PlatformSurprise.h"

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
    config->load("test/resources/config_test_sarasa.xml");
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
    enemy->draw(renderer, camera ->getXpos(), 0);

    //TODO renderizar todos los game objects iterando (faltan los enemigos)
    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        _gameObjects[i]->draw(renderer, camera->getXpos(), 0);
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    logger ->info("Cleaning game\n");
    printf("Cleaning game");
    delete Logger::getInstance();
    // ToDo liberar memoria de todos los singleton.
    
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    textureManager->clearTextureMap();
    SDL_Quit();
}

void Game::handleEvents() {
    player->move();
    enemy->move();
}

bool Game::loadImages() {
    bool success;
    success = textureManager->load("Sprites/sprites_prueba/dino.png", "dino", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/dog.png", "dog", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/RunDog.png", "runDog", renderer);
    success = success && textureManager -> load("Sprites/coinsSprites.png", coinsID, renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/goomba.png", "goomba", renderer);
    success = success && textureManager -> load("Sprites/normalBlock.png", nBlockID, renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/surpriseBlockSprite.png", sBlockID, renderer);
    return success;
}

void Game::createGameObjects() {
    auto* mario = new Player();
    mario->init(0, 403, "dino", 0, camera->getCamera(), 5);
    player = mario;

    //TODO poner esto en Factory
    auto* hongo = new Enemy();
    hongo -> init(900, 403, "goomba", 0, camera->getCamera(), 3, new EnemyMovement(0, 3));
    enemy = hongo;}

void Game::nextStage() {
    stage = stage->nextStage();
}

void Game::restartCharacters() {
    player->restartPos(0, 403);
    camera->restartPos();
}

void Game::update() {
}