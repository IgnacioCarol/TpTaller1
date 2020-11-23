//
// Created by Daniel Bizari on 23/11/2020.
//

#ifndef TPTALLER1_SERVER_H
#define TPTALLER1_SERVER_H


class Server {

public:
    static Server * getInstance();
    virtual ~Server();
    bool run();

private:
    static Server * instance;
    Server();
};


#endif //TPTALLER1_SERVER_H
