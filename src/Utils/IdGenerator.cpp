#include "IdGenerator.h"

IdGenerator* IdGenerator::instance = nullptr;

IdGenerator *IdGenerator::getInstance() {
    if (IdGenerator::instance == nullptr) {
        IdGenerator::instance = new IdGenerator();
    }

    return instance;
}

int IdGenerator::getNextId() {
    return id++;
}
