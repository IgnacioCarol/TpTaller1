//
// Created by lisandro on 8/1/21.
//

#ifndef TPTALLER1_MUSICMANAGER_H
#define TPTALLER1_MUSICMANAGER_H


#include <string>
#include <map>
#include "SDL2/SDL_mixer.h"
#include "../logger/logger.h"

class MusicManager {
public:
    static MusicManager* Instance();
    void addPath(std::string ID, std::string soundPath, bool isMusic);
    void loadSounds();
    void playSound(std::string ID, int loop);
    void playMusic(std::string ID, int loop);

    void pauseMusic();
    void unpauseMusic();

    ~MusicManager();

private:
    static MusicManager* instance;

    //To store the paths
    std::map<std::string, std::string> soundPathsMap;
    std::map<std::string, std::string> musicPathsMap;

    //To store the sounds
    std::map<std::string, Mix_Chunk*> soundsMap;
    std::map<std::string, Mix_Music*> musicMap;

    MusicManager();
};


#endif //TPTALLER1_MUSICMANAGER_H
