#ifdef __APPLE__
#include "SDL.h"
#else

#include "SDL2/SDL.h"

#endif

#include <iostream>
#include <arpa/inet.h>
#include "logger/logger.h"
#include "Server/Server.h"
#include "Client/Client.h"
#include <json.hpp>

using json = nlohmann::json;

#define FPS 40;
const int DELAY_TIME = 1000.0f / FPS;
Uint32 frameStart, frameTime;

#define MAX_ARGS 5 // app_name, xml_path, server/client, ip_address, port

bool parseCLI(int argc, char *argv[], std::string *xmlPath, ConnectionType *mode, std::string *ipAddr, int *port) {
    if (argc > MAX_ARGS) {
        return false;
    }

    if (argc < MAX_ARGS) {
        // set default values
        *xmlPath = "./resources/config.xml";
        *ipAddr = "127.0.0.1";
        *port = 8080;
        *mode = CLIENT;
        return true;
    }

    std::string tmp;

    // Parse XML file
    *xmlPath = argv[1];

    // Parse server/client mode
    tmp = argv[2];
    if (tmp == "client") {
        *mode = CLIENT;
    } else if (tmp == "server") {
        *mode = SERVER;
    } else {
        Logger::getInstance()->error("Invalid launch format, only server and client mode are available");
        return false;
    }

    // Parse IP address
    *ipAddr = argv[3];
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddr->c_str(), &(sa.sin_addr));
    if (result < 0) {
        Logger::getInstance()->error("Invalid IP address format");
        return false;
    }

    // Parse Port
    tmp = argv[4];
    std::stringstream intPort(tmp);
    intPort >> *port;
    if (*port < 1) {
        Logger::getInstance()->error("The PORT must be a postive int");
        return false;
    }

    return true;
}


int main(int argc, char *argv[]) {

//#ifdef TEST
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//#endif
    Logger::getInstance()->info("System initializing...");

    std::string xmlPath;
    ConnectionType mode;
    std::string ipAddr;
    int port;

    if (!parseCLI(argc, argv, &xmlPath, &mode, &ipAddr, &port)) {
        Logger::getInstance()->error("Error: incorrect the program does not support the amount of CLI params");
        return EXIT_FAILURE;
    }

    std::stringstream ss;
    ss << "xmlPath: " << xmlPath << " mode: " << mode << " ipAddr: " << ipAddr << " port: " << port;
    Logger::getInstance()->debug(ss.str());


    if (mode == SERVER) {
        Logger::getInstance()->info("Initializing in server mode");
        Config::getInstance()->load(xmlPath);
        Server *server = Server::getInstance();
        try {
            server->init(ipAddr.c_str(), std::to_string(port).c_str());
            server->run();
            delete server;
        } catch (std::exception &ex) {
            delete server;
            return EXIT_FAILURE;
        }
    } else {
        Logger::getInstance()->info("[Client] Initializing in client mode");
        auto *client = new Client(ipAddr, to_string(port));
        try {
            client->init();
            if (client->login()) {
                client->run();
            }
            delete client;
        } catch (std::exception &ex) {
            delete client;
            Logger::getInstance()->error("Algo fallo en el client");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
