#include <math.h>
#include "Vec2f.h"

//Possibly unnecessary code, 2D Vector can be defined as a 3D vector with no k-plane component. Does not really matter whether we delete or not, could just make things less cluttered.

Vec2f::Vec2f() : x(0.0f), y(0.0f) { }

Vec2f::Vec2f(float x0, float y0) : x(x0), y(y0){ }

Vec2f::Vec2f(const Vec2f &v) : x(v.x), y(v.y){ }

float Vec2f::mag() const {
    return sqrtf(x*x + y*y);
}

float Vec2f::lengthSquared() const {
    return x*x + y*y;
}

Vec2f Vec2f::norm() const {
    float m = mag();
    return Vec2f(x/m, y/m);
}

void Vec2f::normalize() {
    float m = mag();
    x /= m;
    y /= m;
}

Vec2f Vec2f::operator+(const Vec2f &rhs) const {
    return Vec2f(x + rhs.x, y + rhs.y);
}

Vec2f Vec2f::operator-(const Vec2f &rhs) const {
    return Vec2f(x - rhs.x, y - rhs.y);
}

Vec2f Vec2f::operator*(const Vec2f &rhs) const {
    return Vec2f(x * rhs.x, y * rhs.y);
}

void Vec2f::operator+=(const Vec2f &rhs) {
    x += rhs.x;
    y += rhs.y;
}

void Vec2f::operator-=(const Vec2f &rhs) {
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2f::operator*=(const Vec2f &rhs) {
    x *= rhs.x;
    y *= rhs.y;
}

void Vec2f::operator*=(float rhs) {
    x *= rhs;
    y *= rhs;
}

void Vec2f::operator/=(float rhs) {
    x /= rhs;
    y /= rhs;
}

bool Vec2f::operator==(const Vec2f &rhs) {
    return x == rhs.x && y == rhs.y;
}

bool Vec2f::operator!=(const Vec2f &rhs) {
    return x != rhs.x || y != rhs.y;
}

Vec2f operator*(const Vec2f &vec, float scl) {
    return Vec2f(vec.x * scl, vec.y * scl);
}

Vec2f operator/(const Vec2f &vec, float scl) {
    return Vec2f(vec.x / scl, vec.y / scl);
}

Vec2f operator*(float scl, const Vec2f &vec) {
    return Vec2f(vec.x * scl, vec.y * scl);
}

Vec2f operator/(float scl, const Vec2f &vec) {
    return Vec2f(vec.x / scl, vec.y / scl);
}

std::ostream &operator<<(std::ostream &os, const Vec2f &vec) {
    os << "<" << vec.x << ", " << vec.y << ">";
    return os;
}

float Vec2f::dot(const Vec2f &a, const Vec2f &b) {
    return a.x*b.x + a.y*b.y;
}

float Vec2f::angle(const Vec2f &a, const Vec2f &b) {
    return acosf(dot(a, b) / a.mag() / b.mag());
}
