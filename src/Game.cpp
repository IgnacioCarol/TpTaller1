#include "Game.h"
#include <unistd.h>
Game* Game::instance = 0;
int IMAGE_WIDTH;
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
        printf("SDL init success\n");
        window = SDL_CreateWindow(levelName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, 0);
        if (window){
            printf("Window init success\n");
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer){
                camera = { 0, 0, width, height };
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
    camera.x = ( player->getXPosition()  + 200) - camera.w;
    lastValue = camera.x > lastValue ? camera.x : lastValue;
    if( camera.x < lastValue )
    {
        camera.x = lastValue;
    }
    if( camera.x > IMAGE_WIDTH - camera.w )
    {
        camera.x = IMAGE_WIDTH - camera.w;
    }
    textureManager->drawBackgroundWithCamera(800, 600, renderer, &camera);
    player->draw(renderer, camera.x, 0);
    //Aca deberia ir la parte de cargar las imagenes y esas cosas
    //El textureManager deberia tener un puntero al renderer asi le carga las cosas
    SDL_RenderPresent(renderer);
}

void Game::clean() {
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
    success = setBackground("Sprites/sprites_prueba/backgroundCompleto.png");
    success = success && textureManager -> load("Sprites/sprites_prueba/dog.png", "dog", renderer);
    success = success && textureManager -> load("Sprites/sprites_prueba/RunDog.png", "runDog", renderer);
    return success;
}

bool Game::setBackground(const char *path) {
    bool success =  textureManager-> load(path, "BG", renderer);
    if (!success) {
        printf("Image not found at %s\n", path);
        return false;
    }
    SDL_QueryTexture(textureManager->getTextureMap()["BG"], NULL, NULL, &IMAGE_WIDTH, NULL);
}

void Game::createGameObjects() {
    auto* mario = new Player();
    mario->init(0, 403, "dino", 0, &camera);
    player = mario;

    //TODO inicializar el vector GameObject
}