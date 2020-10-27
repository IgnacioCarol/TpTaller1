cmake_minimum_required(VERSION 3.14)
project(TpTaller1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

add_executable(TpTaller1
        src/main.cpp
        test/logger_test.cpp src/logger/logger.cpp src/logger/logger.h src/logger/loggerException.cpp src/logger/loggerException.h
        test/config_test.cpp src/config/Config.cpp src/config/Config.h src/Game.h src/Game.cpp src/TextureManager.h src/TextureManager.cpp
        src/gameobjects/Player.h src/gameobjects/Player.cpp src/gameobjects/GameObject.cpp src/gameobjects/GameObject.h
        src/Factory/Factory.cpp src/Factory/Factory.h src/gameobjects/Coin.cpp src/gameobjects/Coin.h src/GameMap.cpp src/GameMap.h test/gamemap_test.cpp src/gameobjects/PlatformNormal.cpp src/gameobjects/PlatformNormal.h src/gameobjects/PlatformSurprise.cpp src/gameobjects/PlatformSurprise.h
        src/Camera.cpp src/Camera.h src/BackgroundStages/FirstStage.cpp src/BackgroundStages/FirstStage.h src/BackgroundStages/BackgroundStage.h
        src/BackgroundStages/BackgroundStage.cpp src/BackgroundStages/SecondStage.cpp src/BackgroundStages/SecondStage.h src/CharacterStates/CharacterState.cpp
        src/CharacterStates/CharacterState.h src/CharacterStates/Normal.cpp src/CharacterStates/Normal.h src/CharacterStates/Jumping.cpp src/CharacterStates/Jumping.h
        src/CharacterStates/Running.cpp src/CharacterStates/Running.h src/Utils/Vector.cpp src/Utils/Vector.h)

if (APPLE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${TpTaller1_SOURCE_DIR}/cmake")
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED SDL2_image>=2.0.0)
    find_package(SDL2_ttf REQUIRED SDL2_ttf>=2.0.0)
    find_package(SDL2_mixer REQUIRED SDL2_mixer>=2.0.0)
    find_package(Boost REQUIRED)

    include_directories(${SDL2_INCLUDE_DIR}
            ${SDL2_IMAGE_INCLUDE_DIR}
            ${SDL2_TTF_INCLUDE_DIR}
            ${SDL2_MIXER_INCLUDE_DIR}
            ${Boost_INCLUDE_DIRS})
    target_link_libraries(TpTaller1 ${SDL2_LIBRARY}
            ${SDL2_IMAGE_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
            gtest
            gtest_main
            ${Boost_LIBRARIES})
else(APPLE)
    INCLUDE(FindPkgConfig)
    include_directories(.)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
    PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
    target_link_libraries(TpTaller1 ${SDL2_LIBRARY}
            ${SDL2IMAGE_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
            gtest
            gtest_main
            ${Boost_LIBRARIES})
endif (APPLE)