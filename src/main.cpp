#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include <iostream>
#include <arpa/inet.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "logger/logger.h"
#include "Game.h"

#define FPS 40;
const int DELAY_TIME = 1000.0f / FPS;
Uint32 frameStart, frameTime;

typedef enum {SERVER, CLIENT} game_mode_t;
#define MAX_ARGS 5 // app_name, xml_path, server/client, ip_address, port

bool parseCLI(int argc, char * argv[], std::string * xmlPath, game_mode_t * mode, std::string * ipAddr, int * port) {
    if (argc > MAX_ARGS) {
        return false;
    }

    if (argc == 1) {
        // set default values
        *xmlPath = "./resources/config.xml";
        *ipAddr = "127.0.0.1";
        *port = 8080;
        return true;
    }
    std::string tmp;

    // Parse XML file
    *xmlPath = argv[1];

    // Parse server/client mode
    tmp = argv[2];
    if (tmp == "client") {
        *mode = CLIENT;
    } else if (tmp == "server") {
        *mode = SERVER;
    } else {
        Logger::getInstance()->error("Invalid launch format, only server and client mode are available");
        return false;
    }

    // Parse IP address
    *ipAddr = argv[3];
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddr->c_str(), &(sa.sin_addr));
    if(result < 0) {
        Logger::getInstance()->error("Invalid IP address format");
        return false;
    }

    // Parse Port
    tmp = argv[4];
    std::stringstream intPort(tmp);
    intPort >> *port;
    if(*port < 1) {
        Logger::getInstance()->error("The PORT must be a postive int");
        return false;
    }

    return true;
}


int main(int argc, char * argv[]) {

//#ifdef TEST
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//#endif
    Logger::getInstance()->info("System initializing...");

    std::string xmlPath;
    game_mode_t mode;
    std::string ipAddr;
    int         port;

    if (!parseCLI(argc, argv, &xmlPath, &mode, &ipAddr, &port)) {
        Logger::getInstance() -> error("Error: incorrect the program does not support the amount of CLI params");
        return EXIT_FAILURE;
    }

    Logger::getInstance()->info("XML Path: " + xmlPath);

    Game* game = Game::Instance();

    if (!game->init("Level 1", xmlPath)) { //Aca inicializo el background
        Logger::getInstance() -> error("Error: the game could not be initialized");
        return 1;
    }

    game->createGameObjects(); //ToDo refactorizar y mover al Factory, factory tiene que ser el unico responsable de instanciar gameObjects

    if (!game->loadImages()){
        Logger::getInstance() -> error("Error: Loading the sprites went wrong");
        return EXIT_FAILURE;
    }
    if (!game-> loadTexts()) {
        Logger::getInstance()->error("Error: Loading texts went wrong");
        return 1;
    }

    //Event handler
    SDL_Event e;

    while(game->isPlaying()){

        while(SDL_PollEvent(&e) != 0) {
            if (e.type  == SDL_QUIT ) {
                Game::Instance()->gameOver();
            }
        }

        game->handleEvents();
       // game->update();
        game->render();
        SDL_Delay(4);
    }
    Logger::getInstance() -> info("Game over");
    game->clean();
    delete game;
    delete Logger::getInstance();

    SDL_Quit();
    return EXIT_SUCCESS;
}
