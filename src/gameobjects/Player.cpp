#include "Player.h"
static const int GRAVITY = 3;
const int MAX_TICKS_TO_BE_KILLED = 300;

void Player::init(size_t x, size_t y, std::string textureID, SDL_Rect *camera, int framesAmount) {
    GameObject::init(x, y, std::move(textureID));
    xDirection = true;
    jumping = false;
    initialJumpingPosition = yPosition;
    maxYPosition = yPosition - 100;
    cam =  camera;
    characterState = new Normal();
    type = GOT_PLAYER;
    ticks = 0;
    leftOrRightPressed = false;
    atScene = true;
    floor = yPosition;
    firstX = xPosition;
    firstY = yPosition;
    ticksAfterRespawning = MAX_TICKS_TO_BE_KILLED;
}

void Player::run(int direction) {
    if (xPosition < (cam->x + 700) || direction < 0){
        xDirection = direction ? direction > 0 : xDirection;
        xPosition += cam->x < xPosition || direction > 0 ? playerVelocity * direction : 0;
    }
}

void Player::jump(int yMovement) {
    bool isNotStartingPos = yPosition < initialJumpingPosition;
    if ((jumping = canJump() && yMovement && characterState->getStateType() != "FALLING")) {
        yPosition = yPosition + (!isNotStartingPos || yMovement ? - (yMovement + GRAVITY - 1) : + GRAVITY); //TODO change velocity to go up
    } else if (isNotStartingPos || characterState->getStateType() == "FALLING") {
        yPosition += GRAVITY;
    } else {
        yPosition--; //Fix a border case
    }
}

bool Player::canJump() const {
    return ((jumping && yPosition > maxYPosition) || (!jumping && (yPosition == initialJumpingPosition || yPosition == maxYPosition + 100)));
}

Player::Player(SDL_Rect *camera, std::string username, std::string textureID) : GameObject() {
    this->init(0, 380, textureID, camera, 6);
    this->username = username;
    this->isPlayerBig = false;
    this->levelPoints[1] = 0;
    this->levelPoints[2] = 0;
    this->levelPoints[3] = 0;
}

void Player::restartPos(int x, int y) {
    xPosition = x;
    yPosition = (y == 380) ? floor : y;;
}

void Player::changeState(CharacterState *newState) {
    delete characterState;
    Logger::getInstance()->debug("Changing Player State");
    characterState = newState;
}

void Player::move(std::vector<int> vector) {
    Uint8 keyStates[83];
    ticks = 0;
    std::vector<int> arrows = {SDL_SCANCODE_UP, SDL_SCANCODE_LEFT, SDL_SCANCODE_DOWN, SDL_SCANCODE_RIGHT};
    for (int i = 0; i < vector.size() && i < arrows.size(); i++) {
        keyStates[arrows[i]] = vector[i];
    }
    leftOrRightPressed = vector[1] || vector[3];
    firstY = yPosition;
    firstX = xPosition;
    completeMovement(keyStates);
}

void Player::draw(SDL_Renderer *renderer, int cameraX, int cameraY) {
    if (isAlive() || isAtScene(cameraX)){ //The second condition is just for finish the animation when mario dies
        SDL_RendererFlip flip = (xDirection) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        std::string textureID = (characterState->getStateType() == "PAUSED") ? "paused" : this->_textureID;
        divider = (isPlayerBig) ? 4 : 5;
        int auxY = (isPlayerBig) ? 0 : 25; //ToDo ver despues si con esta variable aca ya alcanza o si hay que hacer otros arreglos

        characterState -> draw(textureID, xPosition - cameraX, yPosition + auxY - cameraY, pWidth, pHeight, renderer, flip,divider);
    }
}

bool Player::isJumping() {
    return jumping;
}

bool Player::finishJump() {
    return initialJumpingPosition == yPosition;
}

Player::~Player() {
    delete characterState;
}

std::string Player::getUsername() {
    return username;
}

void Player::setUsername(std::string username) {
    this->username = username;
}

int Player::getFrameAmount() {
    return characterState->getFramesAmount();
}

void Player::setPosition(int x, int y) {
    xPosition = x;
    yPosition = y;
}

void Player::setDirection(bool direction) {
    this -> xDirection = direction;
}

void Player::setState(std::string state) {
    if (state != characterState->getStateType()) {
        if (state == "JUMPING" || state == "FALLING") {
            if (state != "FALLING") {
                MusicManager::Instance()->playSound(JUMP_SMALL_SOUND);
            }
            startToJump();
            changeState(new Jumping(state == "FALLING"));
        } else if (state == "NORMAL") {
            changeState(new Normal());
        } else if (state == "RUNNING") {
            changeState(new Running());
        } else if (state == "CROUCHED") {
            changeState(new Crouched());
        } else if (state == "PAUSED" || state == "FINISH") {
            if (state == "FINISH") {
                MusicManager::Instance()->playSound(STAGE_CLEAR_SOUND);
            }
            changeState(new Paused(state == "PAUSED"));
        } else {
            changeState(new Dying(state == "DYING_FALLING"));
            if (!lives) {
                MusicManager::Instance()->pauseMusic();
                MusicManager::Instance()->playSound(GAME_OVER_SOUND);
            } else if (state == "DYING") {
                MusicManager::Instance()->playSound(MARIO_DIES_SOUND);
            }
        }
    }
}

std::string Player::getState() {
    return characterState->getStateType();
}

bool Player::getDirection() {
    return xDirection;
}

void Player::move() {
    Uint8 keyStates[83];
    if (ticks < 50) {
        keyStates[SDL_SCANCODE_UP] = isJumping() && (abs(yPosition - initialJumpingPosition) > GRAVITY || ticks < 5);
        bool isMoving = leftOrRightPressed && (characterState->getStateType() == "JUMPING" || characterState->getStateType() == "RUNNING");
        keyStates[SDL_SCANCODE_LEFT] =  isMoving && !xDirection;
        keyStates[SDL_SCANCODE_RIGHT] = isMoving && xDirection;
        keyStates[SDL_SCANCODE_DOWN] = characterState->getStateType() == "CROUCHED";
        ticks++;
    } else {
        keyStates[SDL_SCANCODE_UP] = keyStates[SDL_SCANCODE_LEFT] = keyStates[SDL_SCANCODE_RIGHT] = keyStates[SDL_SCANCODE_DOWN] = 0;
    }
    completeMovement(keyStates);
}

void Player::saveLevelPoints(int currentLevel) {
    levelPoints[currentLevel] = partialPoints;
    partialPoints = 0;
}

void Player::addPoints(int newPoints) {
    totalPoints += newPoints;
    partialPoints += newPoints;
}

void Player::setPoints(int points) {
    totalPoints = points;
}

std::map<int,int> Player::getPointsByLevel() {
    return levelPoints;
}

int Player::getTotalPoints() {
    return totalPoints;
}

void Player::completeMovement(const Uint8 *keyStates) {
    characterState->changeState(keyStates, this);
    characterState->move(keyStates, this);
    dropPlayer();
}

void Player::die() {
    if (getState() == "DYING") {
        return;
    }

    if (!testModeState && !this->isInmune()) {
        changeState(new Dying());
        loseLife();
    }
}

void Player::dieFalling() {
    if (!isAtScene(cam->x)){
        changeState(new Dying(true));
        this->setPlayerBig(false);
        if (!testModeState){
            loseLife();
        }
    }
}


void Player::collideWith(GameObject *go) {
    go->collideWith(this);
}

void Player::collideWith(Enemy *enemy) {
    if (enemy->getState() == "DYING") {
        return;
    }
    if (yPosition + getFloorPosition() + 5 < enemy->getYPosition() + enemy->getFloorPosition()) {
        addPoints(enemy->getPoints());
        enemy->die();
    } else {
        if (this->isPlayerBig) {
            this->setPlayerBig(false);
            this->activateInmunity();
            return;
        } else if (ticksAfterRespawning >= MAX_TICKS_TO_BE_KILLED) {
            this->die();
        }
    }
}

int Player::getLives() const {
    return lives;
}

void Player::loseLife() {
    if (lives > 0){
        lives--;
    }
}

bool Player::isAlive() {
    return lives != 0;
}

void Player::testMode() {
    testModeState = !testModeState;
    std::string msg = (testModeState) ? "ACTIVATED" : "DEACTIVATED";
    Logger::getInstance()->info("TEST MODE " + msg);
}

bool Player::getTestModeState() {
    return testModeState;
}

void Player::fall() {
    if (characterState->getStateType() != "FALLING" && characterState->getStateType() != "PAUSED" && ticksAfterRespawning){
        changeState(new Jumping(true));
    }
}
void Player::collideWith(Coin *coin) {
    if (!coin->isHidden() && coin->getState() != "CATCHED"){
        this->addPoints(coin->getPoints());
        coin->changeState("CATCHED");
    }
}

void Player::collideWith(PlatformNormal *nBlock) {
    standOrBlockMovement(nBlock, 60);
}

void Player::standOrBlockMovement(GameObject *go, int heigth) {
    int yBlock = go->getYPosition() + go->getFloorPosition();
    if(yPosition + getFloorPosition() + 20 < yBlock || (isAtScene(cam->x) && (xPosition == cam->x) && !ticksAfterRespawning)) {
        yPosition = yBlock - heigth;
        initialJumpingPosition = yPosition;
    } else {
        restartPos(firstX, firstY);
        if (yPosition < floor) {
            yPosition = yPosition + GRAVITY > floor ? floor : yPosition + GRAVITY;
        }
        if (yPosition >= 595) {
            xPosition = go->getXPosition();
            yPosition = yBlock - heigth - getFloorPosition();
        } else if (yPosition > yBlock) {
            jumping = false;
            if(go->getType() == GOT_PLATFORM_SURPRISE  && abs(xPosition - go->getXPosition()) < go->getWidth() - 5) {
                go->popItem();
            }
        }
    }
}

void Player::collideWith(Hole* hole) {
    if (!ticksAfterRespawning){
        restartPos(hole->getXPosition() + 2 * hole->centerXPos() + 60, yPosition);
        xDirection = true;
    } else if (yPosition > floor){
        if (xPosition < hole->getLeftEdgePosition() || xPosition > hole->getRightEdgePosition()){
            restartPos(firstX, yPosition);
        }
    }
}

void Player::collideWith(Pipe* pipe) {
    standOrBlockMovement(pipe, pipe->getHeight() / 4 - 95);
}

void Player::collideWith(Mushroom* mushroom) {
    int divid = (isPlayerBig) ? 4 : 5;
    auto* bottomLeftPlayer = new Vector(xPosition + 50, yPosition + (pHeight / divid));
    auto* bottomRightPlayer = new Vector(xPosition + (pWidth / divid) - 50, yPosition + (pHeight / divid));

    if (!mushroom->isHidden() && (bottomLeftPlayer->isIn(mushroom->getTopLeftBorder(), mushroom->getBottomRightBorder()) ||
    bottomRightPlayer->isIn(mushroom->getTopLeftBorder(), mushroom->getBottomRightBorder()))) {
        setPlayerBig(true);
        mushroom->changeState("CATCHED");
    }
}

void Player::collideWith(PlatformSurprise *sBlock) {
    int height = (sBlock->getState() == "EMPTY") ? 57 : 60;
    standOrBlockMovement(sBlock, height);
}

void Player::startToJump() {
    initialJumpingPosition = floor;
    maxYPosition = yPosition - 100;
}

void Player::setJumpConfig() {
    initialJumpingPosition = floor;
    maxYPosition = initialJumpingPosition - 100;
    jumping = false;
}

void Player::restartPos() {
    ticksAfterRespawning = 0;
    restartPos(cam->x, floor);
}

bool Player::getPlayerBig() {
    return this->isPlayerBig;
}

void Player::setPlayerBig(bool playerBig) {
    this->isPlayerBig = playerBig;
}

bool Player::isInmune() {
    return this->inmune > 0;
}

void Player::tryUndoInmunity() {
    if (this->inmune > 0) {
        this->inmune--;
    }
}

void Player::activateInmunity() {
    this->inmune = INMUNITY_TIME;
}

bool Player::operator<(const Player &p) const {
    return this->totalPoints > p.totalPoints;
}

void Player::setPointsByLevel(std::map<int,int> points) {
    levelPoints = points;
}

void Player::dropPlayer() {
    if (initialJumpingPosition < floor) {
        yPosition += GRAVITY;
        initialJumpingPosition = yPosition > floor ? floor : yPosition;
    }
}

void Player::finishMovement() {
    firstX = xPosition;
    firstY = yPosition;
    ticksAfterRespawning = ticksAfterRespawning < MAX_TICKS_TO_BE_KILLED ? ticksAfterRespawning + 1 : MAX_TICKS_TO_BE_KILLED;
}

int Player::getWidth() {
    return pWidth / 4;
}

bool Player::isActive() {
    std::string stateType = characterState->getStateType();
    return stateType != "PAUSED" && stateType != "DYING" && stateType != "DYING_FALLING";
}

void Player::setLives(int totalLives) {
    lives = totalLives;
}

void Player::setTestMode(bool testModeState) {
    this->testModeState = testModeState;
}

int Player::getMinHeightToIntersect() {
    return isPlayerBig ? 35 : 60;
}

int Player::centerXPos() {
    return isPlayerBig ? 10 : 0;
}
