#ifndef __TextureManager__
#define __TextureManager__

#include <iostream>
#include <string>
#include <map>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Utils/Printer.h"
#include "../src/logger/logger.h"

#define imgCount 6

//Images ID
#define defaultPlayer "Sprites/Default/defaultPlayer.png"
#define defaultBlock "Sprites/Default/defaultBlock.png"
#define defaultCoin "Sprites/Default/defaultCoins.png"
#define defaultGoomba "Sprites/Default/defaultGoomba.png"
#define defaultKoopa "Sprites/Default/defaultKoopa.png"

#define playerID "dino" //TODO cambiar esto por mario
#define emID "goomba" //Enemy Mushroom
#define etID "koopaGreen" //Enemy turtle
#define coinsID "coinsSprites" //Coins
#define sBlockID "surpriseBlockSprites" //Platform Surprise
#define nBlockID "normalBlock" //Platform Normal

class TextureManager
{
public:

    static TextureManager* Instance()
    {
        if(instance == 0)
        {
            instance = new TextureManager();
            return instance;
        }

        return instance;
    }

    bool load(const std::string& fileName, const std::string& id, SDL_Renderer* pRenderer);
    bool loadText(const std::string id, const std::string text, SDL_Color color, SDL_Renderer* pRenderer);

    bool load(SDL_Renderer* pRenderer);

    void clearTextureMap();
    void clearFromTextureMap(std::string id);

    void draw(std::string ID, int x, int y, int width, int height, SDL_Renderer* renderer,
              SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string ID, int x, int y, int width, int height, int currentFrame, SDL_Renderer *renderer,
                   SDL_RendererFlip flip);

    void drawBackground(int width, int height, SDL_Renderer* renderer);
    std::map<std::string, SDL_Texture*> getTextureMap() { return textureMap; }

    void drawBackgroundWithCamera(int width, int height, SDL_Renderer *renderer, SDL_Rect *clip);

    void printText(std::string id, int x, int y, SDL_Renderer* pRenderer);

private:
    TextureManager() {}

    ~TextureManager() {}
    TextureManager(const TextureManager&);

	TextureManager& operator=(const TextureManager&);

    std::map<std::string, SDL_Texture*> textureMap;
    std::map<std::string, TextTexture*> textTextureMap;

    static TextureManager* instance;

    std::string fileNames[imgCount][2]= {{"Sprites/sprites_prueba/dino.png", defaultPlayer},
                                                {"Sprites/coinsSprites.png", defaultCoin},
                                                {"Sprites/sprites_prueba/Goomba.png", defaultGoomba},
                                                {"Sprites/normalBlock.png", defaultBlock},
                                                {"Sprites/surpriseBlockSprite.png", defaultBlock },
                                                {"Sprites/sprites_prueba/koopaGreen.png", defaultKoopa}};

    std::string id[imgCount] = {playerID, coinsID, emID, nBlockID, sBlockID, etID};
};

typedef TextureManager TheTextureManager;


#endif /* defined(__SDL_Game_Programming_Book__TextureManager__) */
