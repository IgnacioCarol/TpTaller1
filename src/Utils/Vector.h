//
// Created by lisandro on 24/10/20.
//

#ifndef TPTALLER1_VECTOR_H
#define TPTALLER1_VECTOR_H


class Vector {
public:
    explicit Vector(int x, int y);

    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    bool isIn(Vector* topLeft, Vector* bottomRight);

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
