
#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

bool init();
void kill();
bool loop();

void selectInput();

SDL_Rect &
showSection(SDL_Rect &dest, const SDL_Color &foreground, const string &stringToWrite, int yPosition, int xPosition);

bool areCorrectCredentials();

// Pointers to our window, renderer, texture, music, and sound
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture *texture, *text;
TTF_Font* font;
string username;
string password;
bool isWithInput = true;
string errorLoginToShow;

int main(int argc, char** args) {

	if ( !init() ) {
		system("read -p 'Press Enter to continue...' var");
		return 1;
	}

	while ( loop() ) {
		// wait before processing the next frame
		SDL_Delay(10);
	}
    printf("%s\n", username.c_str());
	kill();
	return 0;
}

bool loop() {

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

    dest = showSection(dest, foreground, username, 225, 360);

    dest = showSection(dest, foreground, password, 295, 360);

    dest = showSection(dest, {100, 0, 0}, errorLoginToShow, 400, 400);

	// Update window
	SDL_RenderPresent( renderer );

	return true;
}

bool areCorrectCredentials() {
    if (username != "coso" || password != "cosito") { //FIXME do a method that will check the credentials
        errorLoginToShow = "Invalid username or password";
        return false;
    }
    return true;
}

SDL_Rect &
showSection(SDL_Rect &dest, const SDL_Color &foreground, const string &stringToWrite, int yPosition, int xPosition) {
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

void selectInput() {
    int x, y;
    SDL_GetMouseState( &x, &y );
    isWithInput = y < 300;
}

bool init() {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		return false;
	}

	if ( IMG_Init(IMG_INIT_PNG) < 0 ) {
		cout << "Error initializing SDL_image: " << IMG_GetError() << endl;
		return false;
	}

	// Initialize SDL_ttf
	if ( TTF_Init() < 0 ) {
		cout << "Error intializing SDL_ttf: " << TTF_GetError() << endl;
		return false;
	}

	window = SDL_CreateWindow( "Login", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN );
	if ( !window ) {
		cout << "Error creating window: " << SDL_GetError()  << endl;
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}

	SDL_Surface* buffer = IMG_Load("/home/nacho/Downloads/example/test.jpeg");
	if ( !buffer ) {
		cout << "Error loading image test.png: " << SDL_GetError() << endl;
		return false;
	}

	texture = SDL_CreateTextureFromSurface( renderer, buffer );
	SDL_FreeSurface( buffer );
	buffer = NULL;
	if ( !texture ) {
		cout << "Error creating texture: " << SDL_GetError() << endl;
		return false;
	}

	// Load font
	font = TTF_OpenFont("/home/nacho/Downloads/example/font.ttf", 72);
	if ( !font ) {
		cout << "Error loading font: " << TTF_GetError() << endl;
		return false;
	}

	// Start sending SDL_TextInput events
	SDL_StartTextInput();

	return true;
}

void kill() {
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