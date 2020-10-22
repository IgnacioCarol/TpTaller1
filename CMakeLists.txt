cmake_minimum_required(VERSION 3.14)
project(TpTaller1)

if (APPLE)

    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${TpTaller1_SOURCE_DIR}/cmake")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(Boost REQUIRED)

    add_executable(TpTaller1
            src/main.cpp
            test/logger_test.cpp src/logger/logger.cpp src/logger/logger.h src/logger/loggerException.cpp src/logger/loggerException.h
            test/config_test.cpp src/config/Config.cpp src/config/Config.h
            )

    add_subdirectory(lib/googletest-master)
    include_directories(lib/googletest-master/googletest/include)
    include_directories(lib/googletest-master/googlemock/include)

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

else (APPLE)
    find_package(SDL2 REQUIRED)
    set(CMAKE_CXX_STANDARD 14)
    include_directories(.)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_subdirectory(lib/googletest-master)
    include_directories(lib/googletest-master/googletest/include)
    include_directories(lib/googletest-master/googlemock/include)

    add_executable(TpTaller1
            test/logger_test.cpp
            src/main.cpp
            src/logger.cpp
            src/logger.h
            src/loggerException.cpp
            src/loggerException.h)
    target_link_libraries(TpTaller1 ${SDL2_LIBRARIES}
            gtest
            gtest_main)
endif (APPLE)