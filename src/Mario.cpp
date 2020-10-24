//
// Created by lisandro on 23/10/20.
//

#include <cstdio>
#include "Mario.h"


Mario::Mario() {
    xPosition = 0;
    yPosition = 403; //Por defecto lo creo ahi
    marioState = "dino"; //dino jumpDino, runDino, asi accedo a la imagen en el map del TextureManager TODO cambiar por clase state
    xDirection = true;
    jumping = false;
    initialJumpingPosition = 403;
    frames[0]= 0;
    frames[1]= 1;
    frames[2]= 2;
    frames[3]= 3;
    frames[4]= 4;
}

void Mario::run(int direction) {
    currentFrame = (frames[currentFrame] + 1) % ((sizeof(frames) / sizeof(frames[0])) - 1);
    xDirection = direction == 1;
    //direction puede ser +x o -x  hacemos que valga +- 1 por ahora
    //el y se mantiene igual
    xPosition += direction;
    if (!jumping){
        marioState = (marioState == "dino") ? "runDino" : "dino";
    }
    if (jumping || (!jumping && yPosition != initialJumpingPosition)){
        this->jump(); //para que avance mientras salto
    }
}

void Mario::jump() {
    if (!jumping ){
        initialJumpingPosition = yPosition; //asi tengo un registro de donde estaba
        jumping = true;
        marioState = "jumpDino";
        currentFrame = frames[(sizeof(frames) / sizeof(frames[0]))]; //TODO CHEQUEAR SI SIEMPRE EL SALTO EN LA ULTIMA POSICION DEL SHEET
    }
    //defino que puede saltar de donde esta 50 para arriba
    if (jumping && (yPosition > initialJumpingPosition - 50)){
        yPosition -= 1;
        printf("el valor es: %d", yPosition);
    }   //del 403 vas al 353

    else if (jumping && (yPosition <= initialJumpingPosition - 50)){ //llegue al limite
        jumping = false;
    }

    else if (!jumping && (yPosition < initialJumpingPosition)){
        yPosition += 1; //voy aumentando hasta llegar a donde estaba OJO QUE ESTO EXPLOTA SI ESTOY ARRIBA DE UN LADRILLO
    }

    else if (!jumping && yPosition == initialJumpingPosition){
        marioState = "dino";
        currentFrame = 0;
    }

}
