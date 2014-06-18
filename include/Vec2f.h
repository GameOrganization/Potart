#ifndef VEC2F_H
#define VEC2F_H

#include <iostream>

struct Vec2f {
    //The components of the vector, public for accessibility
    float x, y;

    //Constructs a vector with zero magnitude
    Vec2f();
    //Constructs a vector with the specified components
    Vec2f(float x0, float y0);
    //Copy constructor
    Vec2f(const Vec2f &toCopy);

    //Returns the magnitude of the vector
    float mag() const;
    //Returns the squared length of the vector
    float lengthSquared() const;
    //Returns a normalized copy of the vector
    Vec2f norm() const;
    //Normalizes the vector
    void normalize();

    //Returns the vector sum of this vector and the RHS
    Vec2f operator+(const Vec2f &rhs) const;
    //Returns the vector difference of this vector and the RHS
    Vec2f operator-(const Vec2f &rhs) const;
    //Returns a vector that contains the products of each respective component, useful for RGB color arithmetic
    Vec2f operator*(const Vec2f &rhs) const;


    //Adds the RHS to this vector
    void operator+=(const Vec2f &rhs);
    //Subtracts the RHS from this vector
    void operator-=(const Vec2f &rhs);
    //Multiplies each component of this vector by the corresponding component of the RHS
    void operator*=(const Vec2f &rhs);
    //Scales this vector by the RHS
    void operator*=(float rhs);
    //Scales this vector by the inverse of the RHS
    void operator/=(float rhs);

    //Returns whether the RHS is equal to this vector
    bool operator==(const Vec2f &rhs);
    //Returns whether the RHS is unequal to this vector
    bool operator!=(const Vec2f &rhs);

    //Returns the scaled vector of this and the RHS
    friend Vec2f operator*(const Vec2f &vec, float scl);
    //Returns the scaled vector of this and the inverse of the RHS
    friend Vec2f operator/(const Vec2f &vec, float scl);
    //Returns the scaled vector of this and the LHS
    friend Vec2f operator*(float scl, const Vec2f &vec);
    //Returns the scaled vector of this and the inverse of the LHS
    friend Vec2f operator/(float scl, const Vec2f &vec);

    //Output stream function, prints in the format <x, y>
    friend std::ostream &operator<<(std::ostream &os, const Vec2f &vec);

    //Returns the dot product of two vectors
    static float dot(const Vec2f &a, const Vec2f &b);
    //Returns the angle between two vectors
    static float angle(const Vec2f &a, const Vec2f &b);

};

#endif // Vec2f_H
