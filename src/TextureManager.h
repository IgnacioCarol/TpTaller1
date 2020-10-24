//
// Created by lisandro on 23/10/20.
//

#ifndef TPTALLER1_TEXTUREMANAGER_H
#define TPTALLER1_TEXTUREMANAGER_H


#include <string>
#include <SDL2/SDL_render.h>
#include <map>

class TextureManager {
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
    bool load(std::string fileName, std::string ID, SDL_Renderer* imageRenderer);
    void draw(std::string ID, int x, int y, int width, int height, SDL_Renderer* renderer,
              SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string ID, int x, int y, int width, int height,
                   int currentRow, int currentFrame, SDL_Renderer* renderer,
                   SDL_RendererFlip flip = SDL_FLIP_NONE);

    void drawBackground(int width, int height, SDL_Renderer* renderer);

private:
    TextureManager() {}
    ~TextureManager() {}
    std::map<std::string, SDL_Texture*> textureMap;
    static TextureManager* instance;
};


#endif //TPTALLER1_TEXTUREMANAGER_H
