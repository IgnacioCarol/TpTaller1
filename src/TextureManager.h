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

    ~TextureManager();

    bool load(const std::string& fileName, const std::string& id, SDL_Renderer* pRenderer);
    bool loadText(const std::string id, const std::string text, SDL_Color color, SDL_Renderer* pRenderer);

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

    TextureManager(const TextureManager&);

	TextureManager& operator=(const TextureManager&);

    std::map<std::string, SDL_Texture*> textureMap;
    std::map<std::string, TextTexture*> textTextureMap;
    Printer *printer = Printer::getInstance();

    static TextureManager* instance;
};

typedef TextureManager TheTextureManager;


#endif
