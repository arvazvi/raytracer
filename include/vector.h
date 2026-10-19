#ifndef VECTOR_H
#define VECTOR_H
#pragma once

#include <string>

class Vector3 {
    private:
        double x;
        double y;
        double z;
    public:
        Vector3();
        Vector3(double x, double y, double z);
        ~Vector3();

        double X() const;
        double Y() const;
        double Z() const;

        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator*(double t) const;
        Vector3 operator/(double t) const;

        Vector3 Cross(const Vector3& other) const;
        Vector3 Unit() const;

        double Dot(const Vector3& other) const;
        double Magnitude() const;

};

#endif
