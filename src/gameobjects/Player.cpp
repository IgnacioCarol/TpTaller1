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
    characterState = new Normal(this->isPlayerBig);
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
    if ((jumping = canJump() && yMovement)) {
        yPosition = yPosition + (!isNotStartingPos || yMovement ? - (yMovement + GRAVITY - 1) : + GRAVITY); //TODO change velocity to go up
    } else if (isNotStartingPos) {
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
}

void Player::restartPos(int x, int y) {
    xPosition = x;
    yPosition = y;
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
    if (itsAlive() || isAtScene(cameraX)){ //The second condition is just for finish the animation when mario dies
        SDL_RendererFlip flip = (xDirection) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        std::string textureID = this->_textureID;

        if (this->isPlayerBig) {
            textureID += "-big";
        }

        characterState -> draw(textureID, xPosition - cameraX, yPosition - cameraY, pWidth, pHeight, renderer, flip);
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
        if (state == "JUMPING") {
            MusicManager::Instance()->playSound(JUMP_SMALL_SOUND);
            startToJump();
            changeState(new Jumping(this->isPlayerBig));
        } else if (state == "NORMAL") {
            changeState(new Normal(this->isPlayerBig));
        } else if (state == "RUNNING") {
            changeState(new Running(this->isPlayerBig));
        } else if (state == "CROUCHED") {
            changeState(new Crouched(this->isPlayerBig));
        }
        else if (state == "PAUSED" || state == "FINISH"){
            if (state == "FINISH"){
                MusicManager::Instance()->playSound(STAGE_CLEAR_SOUND);
            }
            changeState(new Paused(state == "PAUSED"));
        }
        else{
            changeState(new Dying(state == "DYING_FALLING"));
            (!loseLife()) ? MusicManager::Instance()->playSound(GAME_OVER_SOUND) : MusicManager::Instance()->playSound(MARIO_DIES_SOUND);
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

void Player::addPoints(int newPoints) {
    points += newPoints;
    levelPoints[level] += newPoints;
}

void Player::changeLevel() {
    level += 1;
    levelPoints[level] = 0; //TODO que Dani C revise esto que era la que lo estaba haciendo
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
        lives = loseLife();
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

int Player::loseLife() {
    lives = (0 > lives - 1) ? 0 : lives - 1;
    return lives;
}

bool Player::itsAlive() {
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

void Player::collideWith(Coin *coin) {
    this->addPoints(coin->getPoints());
    coin->die();
}

void Player::collideWith(PlatformNormal *nBlock) {
    int yBlock = nBlock->getYPosition() + nBlock->getFloorPosition();
    if(yPosition + getFloorPosition() + 20 < yBlock) {
        yPosition = yBlock - nBlock->getHeight() + 10;
        initialJumpingPosition = yPosition;
    } else {
        restartPos(firstX, firstY);
        if (yPosition < floor) {
            yPosition = yPosition + GRAVITY > floor ? floor: yPosition + GRAVITY;
        }
    }
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
    this->characterState->setPlayerBig(playerBig);
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
