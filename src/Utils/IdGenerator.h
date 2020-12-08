#ifndef TPTALLER1_IDGENERATOR_H
#define TPTALLER1_IDGENERATOR_H


class IdGenerator {
public:
    static IdGenerator * getInstance();
    int getNextId();

private:
    static IdGenerator *instance;
    IdGenerator() = default;

    int id = 0;
};


#endif //TPTALLER1_IDGENERATOR_H
