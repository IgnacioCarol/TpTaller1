#ifndef TPTALLER1_LOGIN_H
#define TPTALLER1_LOGIN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <src/logger/logger.h>
#ifdef __APPLE__
#define FONT_FILE_PATH "../resources/fonts/SuperMario256.ttf"
#else
#define FONT_FILE_PATH "resources/fonts/SuperMario256.ttf"
#endif
using namespace std;
class Login {
public:
    Login();
    bool init();
    ~Login();
    bool loop();

private:
    bool areCorrectCredentials();
    static void selectInput();
    SDL_Rect &showSection(SDL_Rect &dest, const SDL_Color &foreground, const string &stringToWrite, int yPosition, int xPosition);
    string username;
    string password;
    string errorLoginToShow;
    TTF_Font *loadFont();
    int fontSize = 35;
    TTF_Font *font = NULL;
};
#endif //TPTALLER1_LOGIN_H
