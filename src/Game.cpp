#include "Game.h"
#include "BackgroundStages/FirstStage.h"
#include "BackgroundStages/BackgroundStage.h"

#include "Factory/Factory.h"
#include "logger/logger.h"
#include "config/Config.h"

Game* Game::instance = 0;
const static char* BACKGROUND = "BG";
int IMAGE_WIDTH;
Game::Game(){
}

Game* Game::Instance() {
    //First time we create an instance of Game
    if(instance == 0) instance = new Game();
    return instance;
}


bool Game::init(const char *levelName, int width, int height) {
    Config * config = new Config();
    config->load("asdf"); //ToDo poner path de xml de test
//    config->getStage(); //ToDo handlear init de stage
    Window windowConfig = config->getWindow();
    Logger::getInstance()->setLogLevel(config->getLog().level);
    Factory::getInstance()->createGameObjectsFromLevelConfig(config->getStage().levels.at(0)); //ToDo Asumo que el 0 contiene el level inicial, chequear!!

    //SDL initializing
    if (!SDL_Init(SDL_INIT_EVERYTHING)){
        printf("SDL init success\n");
        window = SDL_CreateWindow(levelName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  windowConfig.width, windowConfig.height, 0);
        if (window){
            printf("Window init success\n");
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer){
                camera = new Camera(0, 0, width, height);
                stage = new FirstStage(textureManager, renderer);
                printf("Renderer init success\n");
            }

            else {
                printf("Render init fail\n");
                return false;
            }
        }
        else{
            printf("Window init fail\n");
            return false;
        }
    }
    else{
        printf("SDL init fail\n");
        return false;
    }

    printf("Init success\n");
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
    delete Logger::getInstance();
    // ToDo liberar memoria de todos los singleton.
    printf("Cleaning game");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    textureManager->clearTextureMap();
    SDL_Quit();
}

void Game::handleEvents() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    player->jump(currentKeyStates [ SDL_SCANCODE_UP]);
    player->run(currentKeyStates[ SDL_SCANCODE_RIGHT ] - currentKeyStates[ SDL_SCANCODE_LEFT ]);
}

bool Game::loadImages() {
    bool success;
    success = textureManager->load("Sprites/sprites_prueba/dino.png", "dino", renderer);
    if (!success){
        printf("No encontre la ruta\n");
        return false;
    }
    success = textureManager -> load("Sprites/sprites_prueba/dog.png", "dog", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/RunDog.png", "runDog", renderer);
    return success;
}

void Game::createGameObjects() {
    auto* mario = new Player();
    mario->init(0, 403, "dino", 0, camera->getCamera());
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



