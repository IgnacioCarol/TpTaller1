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

void MusicManager::pauseMusic() {
    Mix_PauseMusic();
}

void MusicManager::unpauseMusic() {
    Mix_ResumeMusic();
}

void MusicManager::clearSoundEffectsMaps() {
    soundPathsMap.clear();
    musicPathsMap.clear();

    for (std::pair<std::string, Mix_Chunk*> chunk: soundsMap){
        Mix_FreeChunk(chunk.second);
    }
    Logger::getInstance()->info("All sounds were deleted");
    soundsMap.clear();

    for (std::pair<std::string, Mix_Music*> songs: musicMap){
        Mix_FreeMusic(songs.second);
    }
    Logger::getInstance()->info("All songs were deleted");
    musicMap.clear();
}

MusicManager::~MusicManager() {

    /*Mix_VolumeChunk
     * int Mix_VolumeChunk(Mix_Chunk *chunk, int volume)

    chunk
    Pointer to the Mix_Chunk to set the volume in.
    volume
The volume to use from 0 to MIX_MAX_VOLUME(128).
If greater than MIX_MAX_VOLUME,
then it will be set to MIX_MAX_VOLUME.
If less than 0 then chunk->volume will not be set.
Set chunk->volume to volume.

     */
}
