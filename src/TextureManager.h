#ifndef __TextureManager__
#define __TextureManager__

#ifdef __APPLE__
#include "SDL_image.h"
#else
#include "SDL2/SDL_image.h"
#endif

#include <iostream>
#include <string>
#include <map>
#include "SDL2/SDL.h"
#include "Utils/Printer.h"
#include "../src/logger/logger.h"
#include <vector>

class TextureManager
{
public:

    static TextureManager* Instance();

    ~TextureManager();

    bool load(const std::string& fileName, const std::string& id, SDL_Renderer* pRenderer);
    bool loadText(const std::string id, const std::string text, SDL_Color color, SDL_Renderer* pRenderer);
    void addPath(std::string ID, std::string imagePath, std::string defaultImagePath);
    bool loadImages(SDL_Renderer* renderer);

    void clearTextureMap();
    void clearFromTextureMap(std::string id);

    void draw(std::string ID, int x, int y, int width, int height, SDL_Renderer* renderer,
              SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string ID, int x, int y, int width, int height, int currentFrame, SDL_Renderer *renderer,
                   SDL_RendererFlip flip, int divider = 4);

    std::map<std::string, SDL_Texture*> getTextureMap() { return textureMap; }

    void drawBackgroundWithCamera(int width, int height, std::string bgID, SDL_Renderer *renderer, SDL_Rect *clip);

    void printText(std::string id, int x, int y, SDL_Renderer* pRenderer);

private:
    TextureManager() {}

    TextureManager(const TextureManager&);

	TextureManager& operator=(const TextureManager&);

    std::map<std::string, SDL_Texture*> textureMap;
    std::map<std::string, TextTexture*> textTextureMap;
    std::map<std::string, std::vector<std::string>> imagePathsMap;
    Printer *printer;
    int cont = 0;

    static TextureManager* instance;
};

#endif
