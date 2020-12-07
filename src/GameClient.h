//
// Created by lisandro on 6/12/20.
//

#ifndef TPTALLER1_GAMECLIENT_H
#define TPTALLER1_GAMECLIENT_H


#include <string>
#include <json.hpp>
#include <SDL2/SDL.h>
#include <src/BackgroundStages/BackgroundStage.h>

#include "TextureManager.h"
#include "Camera.h"

class GameClient {
public:
    static GameClient* Instance();
    bool init(); //
    ~GameClient();
    void render(); //para hacer el draw, no recibe nada
    void update(); //Recibe las actualizaciones de los objetos
    bool createGameObjects(); //recibe la lista del init de game objects
    bool loadImages(std::map<std::string, std::vector<std::string>> imagePaths);

private:
    //functions
    GameClient(); //Private constructor to prevent instancing.
    static GameClient* instance; //Here will be the instance stored.
    /*void createEnemies(nlohmann::json message, std::string enemyType);
    void createPlayers(nlohmann::json message);
    void createStaticObjects(nlohmann::json message, std::string objectType);*/


    SDL_Window* window;
    SDL_Renderer* renderer;
    bool playing = false;
    Camera* camera;
    Logger* logger = Logger::getInstance();
    TextureManager* textureManager;

    BackgroundStage* stage;

    //diccionario con los players, [idPlayer, objetoPlayer]
    std::map<std::string, Player*> playersMap;
    std::map<std::string, GameObject*> gameObjectsMap;

    //diccionario con los enemigos

    //diccionarios con los bloquees y esas cosas

    //el textureManager deberia cargar aca todas las imagenes

};


#endif //TPTALLER1_GAMECLIENT_H
