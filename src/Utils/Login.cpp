#include "Login.h"


// Pointers to our window, renderer, texture, music, and sound
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture *texture, *text;
bool isWithInput = true;

Login::Login() {
    if (TTF_Init() < 0 || loadFont() == NULL) {
        std::string error = "Printer initializer - Error initializing TTF. ";
        if (TTF_GetError() != NULL) {
            error + TTF_GetError();
        }
        Logger::getInstance()->error(error);
        return;
    }
}

bool Login :: loop() {

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
				return false;
			case SDL_TEXTINPUT:
                (isWithInput ? username : password) += e.text.text;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_BACKSPACE && !(isWithInput ? username : password).empty()) {
                    (isWithInput ? username : password).pop_back();
				}
				if (e.key.keysym.sym == SDLK_RETURN) {
				    return !areCorrectCredentials();
				}
				break;
		    case SDL_MOUSEBUTTONDOWN:
                selectInput();
		}
	}

// Render texture
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_Color foreground = { 0, 0, 0 };

    dest = showSection(dest, foreground, username, 255, 360);

    dest = showSection(dest, foreground, password, 325, 360);

    dest = showSection(dest, {100, 0, 0}, errorLoginToShow, 430, 50);

	// Update window
	SDL_RenderPresent( renderer );

	return true;
}

bool Login::areCorrectCredentials() {
    if (username != "coso" || password != "cosito") { //FIXME do a method that will check the credentials
        errorLoginToShow = "Invalid username or password";
        return false;
    }
    return true;
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

	SDL_Surface* buffer = IMG_Load("Sprites/loginBackground.png");
	if ( !buffer ) {
        Logger::getInstance() -> error("Error loading image backgoundImage");
		return false;
	}

	texture = SDL_CreateTextureFromSurface( renderer, buffer );
	SDL_FreeSurface( buffer );
	buffer = NULL;
	if ( !texture ) {
        Logger::getInstance() -> error("Error creating texture");
		return false;
	}

	// Start sending SDL_TextInput events
	SDL_StartTextInput();

    while ( loop() ) {
        // wait before processing the next frame
        SDL_Delay(10);
    }
    return true;
}

TTF_Font* Login::loadFont() {
    this->font = TTF_OpenFont(FONT_FILE_PATH, fontSize);
    return this->font;
}

Login::~Login() {
	SDL_StopTextInput();

	TTF_CloseFont( font );
	SDL_DestroyTexture( texture );
	texture = NULL;

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}