//
// Created by lisandro on 24/10/20.
//

#ifndef TPTALLER1_VECTOR_H
#define TPTALLER1_VECTOR_H


class Vector {
public:
    Vector(int x, int y): xPosition(x), yPosition(y){}

    int getX();
    int getY();
    void setX(int x);
    void setY(int y);

    Vector operator+(const Vector& v2) const;

    friend Vector& operator+=(Vector& v1, Vector& v2);

    Vector operator*(int scalar);

    Vector& operator*=(int scalar);

    Vector operator-(const  Vector& v2) const;

    friend Vector operator-=(Vector& v1, Vector& v2);



private:
    int xPosition, yPosition;
};


#endif //TPTALLER1_VECTOR_H
