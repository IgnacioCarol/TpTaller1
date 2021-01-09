//
// Created by lisandro on 8/1/21.
//

#include "MusicManager.h"

MusicManager* MusicManager::instance = nullptr;

MusicManager * MusicManager::Instance() {
    if (!instance){
        instance = new MusicManager();
    }
    return instance;
}

MusicManager::MusicManager() {
    Mix_OpenAudio(22050, AUDIO_S16, 2, 4096); //See page 194 of the book
}

void MusicManager::addPath(std::string ID, std::string soundPath, bool isMusic) {
    if (isMusic && !musicPathsMap.count(ID)){
        musicPathsMap[ID] = soundPath;
    }
    else if (!soundPathsMap.count(ID)){
        soundPathsMap[ID] = soundPath;
    }
}

void MusicManager::loadSounds() {
    for(std::pair<std::string, std::string> element : musicPathsMap){
        Mix_Music* music = Mix_LoadMUS(element.second.c_str());
        if (!music){
            Logger::getInstance() -> error("Error: couldn't load the song with path: " + element.second);
            continue;
        }
        musicMap[element.first] = music;
    }

    for(std::pair<std::string, std::string> element : soundPathsMap){
        Mix_Chunk* sound = Mix_LoadWAV(element.second.c_str());
        if (!sound){
            Logger::getInstance() -> error("Error: couldn't load the sound with path: " + element.second);
            continue;
        }
        soundsMap[element.first] = sound;
    }
}

void MusicManager::playSound(std::string ID, int loop) {
    if(soundsMap.count(ID)){
        Mix_PlayChannel(-1, soundsMap[ID], loop);
    }
}

void MusicManager::playMusic(std::string ID, int loop) {
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    if (musicMap.count(ID)){
       Mix_PlayMusic(musicMap[ID], loop);
    }
}

MusicManager::~MusicManager() {
    Mix_CloseAudio();
}


