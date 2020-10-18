#include "Game.h"
#include "Stage.h"

Game* Game::_instance = 0;

static Game* Game::Instance() {
    //First time we create an instance of Game
    if(s_pInstance == 0) {
        _instance = new Game();
        return _instance;
    }
    //If an instance has been created, returns that instance.
    return _instance;
}


void Game::init() {
    //Ver cómo se settea la cantidad de enemigos-> por XML?
    m_enemy = new Enemy();
    m_enemy->init(0, 0, 128, 82, "animate");
    _gameObjects.push_back(m_enemy);
    _running = 1;
}

int Game::isRunning() {
    return _running;
}

void Game::handleUserEvents() {

}

void Game::updateState() {

}

void Game::render() {

}

void Game::destroy() {

}
