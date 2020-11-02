#include "Game.h"
#include "BackgroundStages/FirstStage.h"
#include "BackgroundStages/BackgroundStage.h"

#include "Factory/Factory.h"
#include "logger/logger.h"
#include "config/Config.h"
#include "CharacterStates/EnemyMovement.h"

#include "gameobjects/PlatformNormal.h"
#include "gameobjects/PlatformSurprise.h"

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
    Config * config = Config::getInstance();
    config->load("../resources/config.xml"); //ToDo poner path de xml de test
//    config->getStage(); //ToDo handlear init de stage
    Window windowConfig = config->getWindow();
    Logger::getInstance()->setLogLevel(config->getLog().level);
    _gameObjects = Factory::getInstance()->createGameObjectsFromLevelConfig(config->getStage().levels.at(0)); //ToDo Asumo que el 0 contiene el level inicial, chequear!!

    //SDL initializing
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
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

Game::~Game() {
    delete this->camera;
    delete this->stage;
    delete this->player;
}

void Game::render() {
    SDL_Delay(2);
    SDL_RenderClear(renderer);
    camera->render(player->getXPosition(), stage->getWidth());
    textureManager->drawBackgroundWithCamera(800, 600, renderer, camera->getCamera());
    player->draw(renderer, camera -> getXpos(), 0);
    enemy->draw(renderer, camera ->getXpos(), 0);

    //TODO renderizar todos los game objects iterando (faltan los enemigos)

    for(std::vector<GameObject*>::size_type i = 0; i != _gameObjects.size(); i++) {
        _gameObjects[i]->draw(renderer, camera->getXpos(), 0);
    }
    stage->renderLevel();
    stage->renderTime();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    logger ->info("Cleaning game\n");
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
    bool success = textureManager -> load(renderer);
    return success;
}

bool Game::loadTexts() {
    bool success = textureManager->loadText(TEXT_WORLD_LEVEL_LABEL_KEY, TEXT_WORLD_LEVEL_LABEL_VALUE, WHITE_COLOR, renderer);
    success = success && textureManager->loadText(TEXT_TIMER_LABEL_KEY, TEXT_TIMER_LABEL_VALUE, WHITE_COLOR, renderer);
    return success;
}

void Game::createGameObjects() {
    auto* mario = new Player();
    mario->init(0, 403, "dino", 0, camera->getCamera(), 5);
    player = mario;

    //TODO poner esto en Factory
    auto* hongo = new EnemyMushroom();
    hongo -> init(900, 425, emID, 0, camera->getCamera(), 3, new EnemyMovement(0, 5));
    enemy = hongo;

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
bool Game::isPlaying() const {
    return this->playing && !this->stage->isTimeOver();
}



