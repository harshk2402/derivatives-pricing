#include "deriv/numerics/Matrix.h"
#include <cmath>

namespace deriv {
namespace numerics {

    Vector operator*(const Matrix& C, const Vector& V)
    {
        int d = (int)C.size();
        Vector W(d);
        for (int j = 0; j < d; j++)
        {
            W[j] = 0.0;
            for (int l = 0; l < d; l++) W[j] = W[j] + C[j][l] * V[l];
        }
        return W;
    }

    Vector operator+(const Vector& V, const Vector& W)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = V[j] + W[j];
        return U;
    }

    Vector operator+(const double& a, const Vector& V)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = a + V[j];
        return U;
    }

    Vector operator*(const double& a, const Vector& V)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = a * V[j];
        return U;
    }

    Vector operator*(const Vector& V, const Vector& W)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = V[j] * W[j];
        return U;
    }

    Vector exp(const Vector& V)
    {
        int d = (int)V.size();
        Vector U(d);
        for (int j = 0; j < d; j++) U[j] = std::exp(V[j]);
        return U;
    }

    double operator^(const Vector& V, const Vector& W)
    {
        double sum = 0.0;
        int d = (int)V.size();
        for (int j = 0; j < d; j++) sum = sum + V[j] * W[j];
        return sum;
    }

    std::ostream& operator<<(std::ostream& out, const Vector& V)
    {
        for (Vector::const_iterator itr = V.begin(); itr != V.end(); itr++)
            out << *itr << "   ";
        out << std::endl;
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Matrix& W)
    {
        for (Matrix::const_iterator itr = W.begin(); itr != W.end(); itr++)
            out << *itr;    // uses operator<<(std::ostream&, const Vector&)
        out << std::endl;
        return out;
    }

}
}
