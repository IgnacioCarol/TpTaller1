#include "TextureManager.h"

static const char *const BACKGROUND = "BG";
static const int CURRENT_ROW = 0; //The sprite sheet always has one row

TextureManager* TextureManager::instance = nullptr;

TextureManager *TextureManager::Instance() {
    if(instance == nullptr){
        instance = new TextureManager();
        instance->printer = Printer::getInstance();
    }

    return instance;
}

bool TextureManager::loadImages(SDL_Renderer* renderer) {
    bool success = true;
    for (std::pair<std::string, std::vector<std::string>> element : imagePathsMap) {
        std::vector<std::string> pathsVector = element.second;
        std::string ID = element.first;

        if(!load(pathsVector[0], ID, renderer)){
            Logger::getInstance() -> debug("Loading the default image for the ID: " + ID);
            success &= load(pathsVector[1], ID, renderer);
            if (!success) {
                Logger::getInstance() -> error("Error: couldn't load the default image");
                return false;
            }
            Logger::getInstance() -> debug("Default image loaded correctly");
        }
        else Logger::getInstance() -> debug("Image loaded correctly for the ID: "+ ID);
    }
    return success;
}

bool TextureManager::load(const std::string& fileName, const std::string& ID, SDL_Renderer *imageRenderer) {
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if (!tempSurface){
        Logger::getInstance() -> error("Error: couldn't load the image with path: " + fileName);
        return false;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(imageRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (imageTexture != nullptr){
        textureMap[ID] = imageTexture;
        return true;
    }
    Logger::getInstance() -> error("Error: couldn't create the imageTexture");
    return false;
}

bool TextureManager::loadText(const std::string key, const std::string text, SDL_Color color, SDL_Renderer* pRenderer) {
    TextTexture* textTexture = printer->getTextTexture(text, color, pRenderer);
    if (textTexture == nullptr) {
        Logger::getInstance()->error("Couldnt load text: " + text);
        return false;
    }
    if (textTextureMap[key]) {
        printer->freeTexture(textTextureMap[key]);
    }
    textTextureMap[key] = textTexture;
    return true;
}

void TextureManager::draw(std::string ID, int x, int y, int width, int height, SDL_Renderer *renderer,
                          SDL_RendererFlip flip) {
    SDL_Rect  srcRect; //Aca defino size de la imagen
    SDL_Rect destRect; //Aca donde va a ir, se mapea para ajustarse el tamanio

    if (ID == "hole") {
        SDL_SetRenderDrawColor(renderer, 107, 140, 255, 255); // Sky blue
        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // the rect color (solid red)
        SDL_Rect rect; // the rectangle
        rect.h = height;
        rect.w = width;
        rect.x = x;
        rect.y = y;
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    SDL_Texture* texture = textureMap[ID];
    SDL_QueryTexture(texture, nullptr, nullptr, &srcRect.w, &srcRect.h);

    srcRect.x = 0;
    srcRect.y = 0; //Tomo parte superior de la imagen
    if (width == 1) {
        destRect.w = srcRect.w / 4;
        destRect.h = srcRect.h / 4;
    }
    else{
        srcRect.w = width;
        srcRect.h = height;
        destRect.w = srcRect.w / 4;
        destRect.h = srcRect.h / 4;
    }
    destRect.x = x;
    destRect.y = y;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, 0, flip);

}


void TextureManager::drawBackgroundWithCamera(int width, int height, std::string bgID, SDL_Renderer *renderer,
                                              SDL_Rect *clip) {
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { 0, 0, width, height };
        SDL_Texture* texture = textureMap[bgID];
        //Set clip rendering dimensions
        if( clip != nullptr )
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        //Render to screen
        SDL_RenderCopyEx( renderer, texture, clip, &renderQuad, 0, 0, SDL_FLIP_NONE );
    }
}

void
TextureManager::drawFrame(std::string ID, int x, int y, int width, int height, int currentFrame, SDL_Renderer *renderer,
                          SDL_RendererFlip flip, int divider) {

    if (ID.find("hole") != std::string::npos) {
        if(ID == "hole-black") {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        } else {
            SDL_SetRenderDrawColor(renderer, 107, 140, 255, 255); // Sky blue
        }

        SDL_Rect rect; // the rectangle
        rect.h = height;
        rect.w = width;
        rect.x = x;
        rect.y = y;
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = currentFrame;
    srcRect.y = CURRENT_ROW;
    srcRect.w = width;
    srcRect.h = height;

    destRect.x = x;
    destRect.y = y;
    destRect.w = width / divider;
    destRect.h = height / divider;
    SDL_RenderCopyEx(renderer, textureMap[ID], &srcRect, &destRect, 0, 0, flip);
}

void TextureManager::printText(std::string id, int x, int y, SDL_Renderer* pRenderer) {
    if (textTextureMap[id] == nullptr) {
        Logger::getInstance()->error("Couldnt find text with id: " + id);
        return;
    }

    printer->render(textTextureMap[id], x, y, pRenderer);
}

void TextureManager::clearTextureMap()
{
    textureMap.clear();
    for(auto element: textTextureMap) {
        this->printer->freeTexture(element.second);
    }
    textTextureMap.clear();
}

void TextureManager::clearFromTextureMap(std::string id)
{
    textureMap.erase(id);
    Logger::getInstance()->debug("Texture deleted correctly");
}

TextureManager::~TextureManager() {
    delete this->printer;
}

void TextureManager::addPath(std::string ID, std::string imagePath, std::string defaultImagePath) {
    if (!imagePathsMap.count(ID)){
        imagePathsMap[ID].push_back(imagePath);
        imagePathsMap[ID].push_back(defaultImagePath);
    }
}



