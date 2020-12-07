#include "Login.h"


// Pointers to our window, renderer, texture, music, and sound
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture *loginTexture, *text, *waitingRoomTexture;
bool isWithInput = true;

Login::Login() {
    if (TTF_Init() < 0 || loadFont() == NULL) {
        std::string error = "Login initializer - Error initializing TTF. ";
        if (TTF_GetError() != NULL) {
            error + TTF_GetError();
        }
        Logger::getInstance()->error(error);
        return;
    }
}

Authentication* Login::getAuthentication() {
    bool authenticated = false;
    while (!authenticated) {
        static const unsigned char* keys = SDL_GetKeyboardState( NULL );

        SDL_Event e;
        SDL_Rect dest;

        // Clear the window to white
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        SDL_RenderClear( renderer );
        // Event loop
        while ( SDL_PollEvent( &e ) != 0 ) {
            switch (e.type) {
                case SDL_QUIT:
                    throw exception(); //TODO: no deberia ser una excepcion, deberia haber forma de solo terminar el juego.
                case SDL_TEXTINPUT:
                    (isWithInput ? authentication.username : authentication.password) += e.text.text;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_BACKSPACE && !(isWithInput ? authentication.username : authentication.password).empty()) {
                        (isWithInput ? authentication.username : authentication.password).pop_back();
                    }
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        Logger::getInstance()->debug("Returning authentication with username "
                                                     + authentication.username + " and password: " + authentication.password);
                        return &authentication;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    selectInput();
                    break;
            }
        }

// Render texture
        SDL_RenderCopy(renderer, loginTexture, NULL, NULL);

        SDL_Color foreground = { 0, 0, 0 };

        dest = showSection(dest, foreground, authentication.username, 255, 360);

        dest = showSection(dest, foreground, authentication.password, 325, 360);

        dest = showSection(dest, {100, 0, 0}, errorLoginToShow, 430, 50);

        // Update window
        SDL_RenderPresent( renderer );

        SDL_Delay(10);
    }
    return nullptr;
}

SDL_Rect & Login :: showSection(SDL_Rect &dest, const SDL_Color &foreground, const string &stringToWrite, int yPosition, int xPosition) {
    if ( !stringToWrite.empty() ) {
        SDL_Surface* text_surf = TTF_RenderText_Solid(font, stringToWrite.c_str(), foreground);
        text = SDL_CreateTextureFromSurface(renderer, text_surf);

        dest.x = xPosition;
        dest.y = yPosition;
        dest.w = text_surf->w;
        dest.h = text_surf->h;
        SDL_RenderCopy(renderer, text, NULL, &dest);

        SDL_DestroyTexture(text);
        SDL_FreeSurface(text_surf);
    }
    return dest;
}

void Login :: selectInput() {
    int x, y;
    SDL_GetMouseState( &x, &y );
    isWithInput = y < 300;
}

bool Login::init() {
    Logger::getInstance()->debug("Initializing login view");
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
        Logger::getInstance() -> error("Error initializing SDL");
		return false;
	}

	if ( IMG_Init(IMG_INIT_PNG) < 0 ) {
        Logger::getInstance() -> error("Error initializing SDL_image");
		return false;
	}

	// Initialize SDL_ttf
	if ( TTF_Init() < 0 ) {
        Logger::getInstance() -> error("Error intializing SDL_ttf");
		return false;
	}

	window = SDL_CreateWindow( "Login", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN );
	if ( !window ) {
        Logger::getInstance() -> error("Error creating window");
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
        Logger::getInstance() -> error("Error creating renderer");
		return false;
	}

	SDL_Surface* loginBuffer = IMG_Load(LOGIN_BACKGROUND_PATH);
	if ( !loginBuffer ) {
        Logger::getInstance() -> error("Error loading login background image ");
		return false;
	}

    loginTexture = SDL_CreateTextureFromSurface(renderer, loginBuffer );
    SDL_FreeSurface( loginBuffer );
    loginBuffer = NULL;
    if ( !loginTexture ) {
        Logger::getInstance() -> error("Error creating login texture");
        return false;
    }

    SDL_Surface* waitingRoomBuffer = IMG_Load(WAITING_ROOM_BACKGROUND_PATH);
    if (!waitingRoomBuffer){
        Logger::getInstance() -> error("Error loading waiting room image");
        return false;
    }

    waitingRoomTexture = SDL_CreateTextureFromSurface(renderer, waitingRoomBuffer );
    SDL_FreeSurface( waitingRoomBuffer );
    waitingRoomBuffer = NULL;
    if ( !loginTexture ) {
        Logger::getInstance() -> error("Error creating waiting room texture");
        return false;
    }

	// Start sending SDL_TextInput events
	SDL_StartTextInput();

	/*
    while (getAuthentication() ) {
        // wait before processing the next frame
        SDL_Delay(10);
    }*/
    return true;
}

TTF_Font* Login::loadFont() {
    this->font = TTF_OpenFont(FONT_FILE_PATH, fontSize);
    return this->font;
}

Login::~Login() {
	SDL_StopTextInput();

	TTF_CloseFont( font );
	SDL_DestroyTexture(loginTexture);
    loginTexture = NULL;
    SDL_DestroyTexture(waitingRoomTexture);
    waitingRoomTexture = NULL;

    SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	TTF_Quit();
	IMG_Quit();
//	SDL_Quit();
}

void Login::showError(std::string error) {
    this->errorLoginToShow = error;
}


void Login::showWaitingRoom(SDL_Event e) {
    while(SDL_PollEvent(&e) != 0) {
        if (e.type  == SDL_QUIT ) {
            return;
        }
    }

    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderClear( renderer );

    SDL_RenderCopy(renderer, waitingRoomTexture, NULL, NULL);

    SDL_Rect dest;

    SDL_Color foreground = { 255, 255, 255 };

    dest = showSection(dest, foreground, WAITING_ROOM_MESSAGE, 280, 100);

    // Update window
    SDL_RenderPresent( renderer );

    SDL_Delay(10);
}
