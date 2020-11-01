#ifndef __TextureManager__
#define __TextureManager__

#include <iostream>
#include <string>
#include <map>
#include "SDL2/SDL.h"

#define imgCount 5

//Images ID
#define emID "goomba" //Enemy Mushroom
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


    ~TextureManager() {}

private:
    TextureManager() {}

    TextureManager(const TextureManager&);

	TextureManager& operator=(const TextureManager&);

    std::map<std::string, SDL_Texture*> textureMap;

    static TextureManager* instance;

    const char* fileNames[imgCount] = {"Sprites/sprites_prueba/dino.png", "Sprites/coinsSprites.png",
                                        "Sprites/sprites_prueba/Goomba.png", "Sprites/normalBlock.png",
                                        "Sprites/surpriseBlockSprite.png"};

    const char* id[imgCount] = {"dino", coinsID, emID, nBlockID, sBlockID};
};

typedef TextureManager TheTextureManager;


#endif /* defined(__SDL_Game_Programming_Book__TextureManager__) */
