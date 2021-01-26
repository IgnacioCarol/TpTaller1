//
// Created by lisandro on 8/1/21.
//All the info in: https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer.html
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
    void playSound(std::string ID, int loop = 0);
    void playMusic(std::string ID, int loop = -1);
    bool isMusicPaused();
    bool areSoundsMuted();

    void pauseMusic();
    void unpauseMusic();
    void muteSounds();
    void unmuteSounds();

    ~MusicManager();
    void clearSoundEffectsMaps();

private:
    static MusicManager* instance;

    //To store the paths
    std::map<std::string, std::string> soundPathsMap;
    std::map<std::string, std::string> musicPathsMap;

    //To store the sounds
    std::map<std::string, Mix_Chunk*> soundsMap;
    std::map<std::string, Mix_Music*> musicMap;

    bool musicPaused = false;
    bool mutedSounds = false;

    MusicManager();
};


#endif //TPTALLER1_MUSICMANAGER_H