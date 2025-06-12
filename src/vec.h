#pragma once
#include "flops.h"
// for sqrtf, sinf, cosf, atan2f
extern "C" {
    #include <math.h>
}

#ifndef TOL
#define TOL 1e-12
#endif

class Vector3 {
public:

    FLOATTYPE x, y, z;

    Vector3() : x(0), y(0), z(0) {};
    Vector3(FLOATTYPE _x, FLOATTYPE _y, FLOATTYPE _z) : x(_x), y(_y), z(_z) {};
    Vector3(const Vector3 &other) { *this = other; }

    bool operator==(const Vector3 &other) {
        return (fabs(x - other.x) < TOL) && (fabs(y - other.y) < TOL) && (fabs(z - other.z) < TOL);
    }

    Vector3& operator=(const Vector3 &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3 operator+(const Vector3 &other) const {
        increment_n_flops(3);
        return Vector3(x+other.x, y+other.y, z+other.z);
    }

    Vector3& operator+=(const Vector3 &other) {
        *this = *this + other;
        return *this;
    }

    Vector3 operator-(const Vector3 &other) const {
        increment_n_flops(3);
        return Vector3(x-other.x, y-other.y, z-other.z);
    }

    Vector3& operator-=(const Vector3 &other) {
        *this = *this - other;
        return *this;
    }

    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    Vector3 operator*(const FLOATTYPE scalar) const {
        increment_n_flops(3);
        return Vector3(x*scalar, y*scalar, z*scalar);
    }

    Vector3& operator*=(const FLOATTYPE scalar) {
        *this = *this * scalar;
        return *this;
    }

    Vector3 operator/(const FLOATTYPE scalar) const {
        increment_n_flops(3);
        return Vector3(x/scalar, y/scalar, z/scalar);
    }

    Vector3& operator/=(const FLOATTYPE scalar) {
        *this = *this / scalar;
        return *this;
    }

    Vector3 norm() const {
        return Vector3(*this) / len();
    }

    /**
	 * @brief Returns the cross product of the left and right hand vectors
	 * @return cross product of the two vectors
	 */
	Vector3 cross(const Vector3& v) const {
        increment_n_flops(9);
		Vector3 ret(   this->y * v.z - this->z * v.y, 
                    this->z * v.x - this->x * v.z, 
                    this->x * v.y - this->y * v.x);
		return ret;
    }

    /**
	 * @brief Returns the dot product of the left and right hand vectors
	 * @return dot product of the two vectors
	 */
	FLOATTYPE dot(const Vector3& other) const {
        increment_n_flops(5);
		return (x*other.x + y*other.y + z*other.z);
	}

    FLOATTYPE len() const {
        increment_n_flops(6);
        return sqrtf(x*x + y*y + z*z);
    }

    FLOATTYPE angle_between_vectors(const Vector3 &other) const {
        increment_n_flops(3);
		return acosf(dot(other) / (len() * (other.len())));
	}

};
