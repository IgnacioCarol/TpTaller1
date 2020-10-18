/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "TextureManager.h"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path ); //FIXME Currently unused, maybe we should remove it

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;
std::string mario = "coso";
std::string marioJump = "jump";
std::string BACKGROUND = "background";
TextureManager* textureManager = TextureManager::Instance();

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success;

    //Load texture
    success = textureManager->load("../assets/NES - Super Mario Bros - World 1-1.png", BACKGROUND, gRenderer);
    success = success && textureManager->load("../png/Dead (1).png", mario, gRenderer);
    success = success && textureManager->load("../png/Jump (5).png", marioJump, gRenderer);

    //Nothing to load
    return success;
}

void close()
{
    //Free loaded image
    SDL_DestroyTexture( gTexture );
    textureManager->clearTextureMap();
    textureManager = nullptr;
    gTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

void handleUserMovement(int &xValue, int &xValueBackground, int &yPosition, int maxYPosition, bool *jumping,
                        SDL_RendererFlip &state, std::string &userState) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    bool isJumping = * jumping; //Fixme Could be a global variable so we will avoid the global variable
    if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        if (xValue > 550) {
            xValueBackground += 1;
        } else {
            xValue +=  1;
            state = SDL_FLIP_NONE;
        }
    }
    if( currentKeyStates[ SDL_SCANCODE_LEFT ] && xValue > -5)
    {
        xValue -=  1;
        state = SDL_FLIP_HORIZONTAL;
    }
    if (!isJumping && currentKeyStates [ SDL_SCANCODE_UP] && yPosition == 450) {
        *jumping = true;
        userState = marioJump;
    }
    if (isJumping) {
        yPosition -= 1;
        *jumping = yPosition > maxYPosition;
    }
    if (yPosition < 450 && !isJumping) {
        yPosition += 1;
        if (yPosition == 450) { //Fixme really hardcoded values, we should check which values are correct and how to obtain them dynamically
            userState = mario; //Fixme use state with this, do not handle from this
        }
    }
}

int main(int argc, char* args[] )
{
    SDL_RendererFlip state = SDL_FLIP_NONE;
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;
            int xValue = 0;
            int xValueBackground = 0;
            int yPosition = 450;
            int maxYPosition = 350;
            bool jumping = false; //FIXME implement a better state
            std::string stateOfMario = mario;
            //While application is running
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                handleUserMovement(xValue, xValueBackground, yPosition, maxYPosition, &jumping, state,
                                   stateOfMario);
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                textureManager->draw(BACKGROUND, -xValueBackground, 0, 3392 , SCREEN_HEIGHT * 2, gRenderer);
                textureManager->draw(stateOfMario, xValue, yPosition, SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer, state, false);

                //Update screen
                SDL_RenderPresent( gRenderer );
                SDL_Delay(2);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}