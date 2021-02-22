//
// Created by lisandro on 24/10/20.
//

#include "Vector.h"

Vector::Vector(int x, int y) {
    xPosition = x;
    yPosition = y;
}

int Vector::getX() { return xPosition;}

int Vector::getY() { return yPosition;}

void Vector::setX(int x) { xPosition = x;}

void Vector::setY(int y) { yPosition = y;}

Vector Vector::operator+(const Vector &v2) const {
    return Vector(xPosition + v2.xPosition, yPosition + v2.yPosition);
}

Vector &operator+=(Vector &v1, Vector &v2) {
    v1.xPosition += v2.xPosition;
    v1.yPosition += v2.yPosition;
    return v1;
}

Vector Vector::operator*(int scalar) {
    return Vector(xPosition * scalar, yPosition * scalar);
}

Vector &Vector::operator*=(int scalar) {
    xPosition *= scalar;
    yPosition *= scalar;
    return *this;
}

Vector Vector::operator-(const Vector &v2) const {
    return Vector(xPosition - v2.xPosition, yPosition - v2.yPosition);
}

Vector operator-=(Vector &v1, Vector &v2) {
    v1.xPosition -= v2.xPosition;
    v1.yPosition -= v2.yPosition;
    return v1;
}

bool Vector::isIn(Vector* topLeft, Vector* bottomRight) {
    return (xPosition >= topLeft->getX() && xPosition <= bottomRight->getX()) &&
    (yPosition >= topLeft->getY() && yPosition <= bottomRight->getY());
}

