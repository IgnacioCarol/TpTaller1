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
    static GameClient* Instance(); //no se si hace falta que sea singleton
    bool init(nlohmann::json message); //se le pasa por primera vez todos los ids y esas cosas asi crea los objetos
    ~GameClient();
    void render(); //aca haremos el draw
    void update(nlohmann::json message); //que se encargue de recibir el json y updatear a todos los objetos

private:
    //functions
    GameClient(); //Private constructor to prevent instancing.
    static GameClient* instance; //Here will be the instance stored.
    bool createGameObjects(nlohmann::json message);
    bool loadImages(nlohmann::json message);


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
