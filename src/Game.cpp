#include "Game.h"
#include "src/BackgroundStages/FirstStage.h"
#include "src/BackgroundStages/BackgroundStage.h"

#include <unistd.h>
#include <src/config/Config.h>

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
    //SDL initializing
    if (!SDL_Init(SDL_INIT_EVERYTHING)){
        logger -> info("SDL init success\n");
        window = SDL_CreateWindow(levelName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, 0);
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
    player->move();
}

bool Game::loadImages() {
    bool success;
    success = textureManager->load("Sprites/sprites_prueba/dino.png", "dino", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/dog.png", "dog", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/RunDog.png", "runDog", renderer);
    return success;
}

void Game::createGameObjects() {
    auto* mario = new Player();
    mario->init(0, 403, "dino", 0, camera->getCamera(), 5);
    player = mario;

    //TODO inicializar el vector GameObject
}

void Game::nextStage() {
    stage = stage->nextStage();
}

void Game::restartCharacters() {
    player->restartPos(0, 403);
    camera->restartPos();
}



