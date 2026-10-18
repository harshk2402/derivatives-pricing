#pragma once
#include <vector>
#include <iostream>

namespace deriv {
namespace numerics {

    typedef std::vector<double> Vector;
    typedef std::vector<Vector> Matrix;

    // Linear algebra on Vector / Matrix as free operators.
    // Note: numerics::exp(Vector) hides ::exp(double) for code inside this
    // namespace, so scalar maths here is always written std::exp, std::sqrt, ...
    Vector operator*(const Matrix& C, const Vector& V);   // matrix-vector product
    Vector operator*(const double& a, const Vector& V);   // scalar multiple
    Vector operator*(const Vector& V, const Vector& W);   // element-wise product
    Vector operator+(const double& a, const Vector& V);   // add a scalar to every element
    Vector operator+(const Vector& V, const Vector& W);   // element-wise sum
    Vector exp(const Vector& V);                          // element-wise exponential
    double operator^(const Vector& V, const Vector& W);   // scalar (dot) product
    std::ostream& operator<<(std::ostream& out, const Vector& V);
    std::ostream& operator<<(std::ostream& out, const Matrix& W);

}
}
