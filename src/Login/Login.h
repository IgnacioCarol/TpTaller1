#ifndef TPTALLER1_LOGIN_H
#define TPTALLER1_LOGIN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../logger/logger.h"
#ifdef __APPLE__
#define FONT_FILE_PATH "../resources/fonts/SuperMario256.ttf"
#else
#define FONT_FILE_PATH "resources/fonts/SuperMario256.ttf"
#endif

#define LOGIN_BACKGROUND_PATH "Sprites/loginBackground.png"
#define WAITING_ROOM_BACKGROUND_PATH "Sprites/waitingRoomBackground.png"
#define WAITING_ROOM_MESSAGE "Waiting for all players..."

struct Authentication {
    std::string username;
    std::string password;
};

using namespace std;
class Login {
public:
    Login();
    ~Login();
    bool init();
    Authentication* getAuthentication();
    void showError(std::string error);
    void showWaitingRoom(SDL_Event e);
    bool isWaitingRoom = false;
private:
    static void selectInput();
    SDL_Rect &showSection(SDL_Rect &dest, const SDL_Color &foreground, const string &stringToWrite, int yPosition, int xPosition);
    Authentication authentication = {"", ""};
    string errorLoginToShow;
    TTF_Font *loadFont();
    int fontSize = 35;
    TTF_Font *font = NULL;
};
#endif //TPTALLER1_LOGIN_H
