cmake_minimum_required(VERSION 3.14)
project(TpTaller1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

find_package(SDL2 REQUIRED)

add_executable(TpTaller1
        src/main.cpp
        test/logger_test.cpp src/logger/logger.cpp src/logger/logger.h src/logger/loggerException.cpp src/logger/loggerException.h
        test/config_test.cpp src/config/Config.cpp src/config/Config.h src/Game.h src/Game.cpp src/TextureManager.h src/TextureManager.cpp
        src/Player.h src/Player.cpp src/GameObject.cpp src/GameObject.h
        src/Camera.cpp src/Camera.h src/BackgroundStages/FirstStage.cpp src/BackgroundStages/FirstStage.h src/BackgroundStages/BackgroundStage.h src/BackgroundStages/BackgroundStage.cpp src/BackgroundStages/SecondStage.cpp src/BackgroundStages/SecondStage.h)

if (APPLE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${TpTaller1_SOURCE_DIR}/cmake")

    INCLUDE(FindPkgConfig)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
    PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)


    include_directories(.)
    target_link_libraries(TpTaller1 ${SDL2_LIBRARIES}
            ${SDL2IMAGE_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
            gtest
            gtest_main
            ${Boost_LIBRARIES})

else (APPLE)
    INCLUDE(FindPkgConfig)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
    PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

    include_directories(.)
    target_link_libraries(TpTaller1 ${SDL2_LIBRARIES}
            ${SDL2IMAGE_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
            gtest
            gtest_main)

endif (APPLE)