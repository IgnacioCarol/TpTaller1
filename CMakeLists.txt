cmake_minimum_required(VERSION 3.14)
project(TpTaller1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)
include_directories(lib/nlohmann)

add_executable(TpTaller1
        src/main.cpp
        src/logger/logger.cpp src/logger/logger.h
        src/logger/loggerException.cpp src/logger/loggerException.h
        src/config/Config.cpp src/config/Config.h
        src/Utils/Printer.cpp src/Utils/Printer.h
        src/Utils/Timer.cpp src/Utils/Timer.h
        src/Utils/Vector.cpp src/Utils/Vector.h
        src/Game.cpp src/Game.h
        src/GameMap.cpp src/GameMap.h test/gamemap_test.cpp
        src/TextureManager.cpp src/TextureManager.h
        src/Camera.cpp src/Camera.h
        src/BackgroundStages/BackgroundStage.cpp src/BackgroundStages/BackgroundStage.h
        src/BackgroundStages/FirstStage.cpp src/BackgroundStages/FirstStage.h
        src/BackgroundStages/SecondStage.cpp src/BackgroundStages/SecondStage.h
        src/CharacterStates/CharacterState.cpp src/CharacterStates/CharacterState.h
        src/CharacterStates/Jumping.cpp src/CharacterStates/Jumping.h
        src/CharacterStates/Normal.cpp src/CharacterStates/Normal.h
        src/CharacterStates/Running.cpp src/CharacterStates/Running.h src/CharacterStates/Crouched.cpp src/CharacterStates/Crouched.h
        src/Factory/Factory.cpp src/Factory/Factory.h test/factory_test.cpp
        src/gameobjects/Coin.cpp src/gameobjects/Coin.h
        src/gameobjects/EnemyMushroom.cpp src/gameobjects/EnemyMushroom.h
        src/gameobjects/EnemyTurtle.cpp src/gameobjects/EnemyTurtle.h
        src/gameobjects/GameObject.cpp src/gameobjects/GameObject.h
        src/gameobjects/PlatformNormal.cpp src/gameobjects/PlatformNormal.h
        src/gameobjects/PlatformSurprise.cpp src/gameobjects/PlatformSurprise.h
        src/gameobjects/Player.cpp src/gameobjects/Player.h
        test/config_test.cpp
        test/logger_test.cpp src/config/Constants.h src/config/ConfigException.cpp src/config/ConfigException.h
        src/CharacterStates/EnemyMovement.h src/CharacterStates/EnemyMovement.cpp src/gameobjects/Enemy.h src/gameobjects/Enemy.cpp
        src/BackgroundStages/ThirdStage.cpp src/BackgroundStages/ThirdStage.h
        src/Login/Login.cpp src/Login/Login.h
        src/Client/Client.cpp src/Client/Client.h
        src/Server/Server.cpp src/Server/Server.h
        src/Socket/Socket.cpp src/Socket/Socket.h
        src/Socket/SocketException.cpp src/Socket/SocketException.h
        src/Server/ServerException.cpp src/Server/ServerException.h
        src/Server/PlayerClient.cpp src/Server/PlayerClient.h
        src/Client/ClientMsg.h
        src/Server/ServerMsg.h
        src/Socket/SocketMsg.h
        src/Protocol/Protocol.cpp src/Protocol/Protocol.h
        src/Protocol/MessageValidator.cpp src/Protocol/MessageValidator.h
        src/Client/ClientException.cpp src/Client/ClientException.h
        src/Server/GameServer.cpp src/Server/GameServer.h src/Protocol/InitializeGameMsg.h)

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
            ${SDL2TTF_LIBRARIES}
            ${SDL2MIXER_LIBRARIES}
            gtest
            gtest_main
            ${Boost_LIBRARIES})
endif (APPLE)