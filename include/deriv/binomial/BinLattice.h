#pragma once
#include <iostream>
#include <iomanip>
#include <vector>

namespace deriv {
namespace binomial {

    // Triangular lattice holding one value per node (n, i), 0 <= i <= n <= N.
    template<typename Type>
    class BinLattice
    {
    private:
        int N;
        std::vector<std::vector<Type> > Lattice;

    public:
        BinLattice() : N(0) {}
        BinLattice(int N_) : N(N_)
        {
            Lattice.resize(N + 1);
            for (int n = 0; n <= N; n++) Lattice[n].resize(n + 1);
        }
        ~BinLattice() {}

        void SetN(int N_)
        {
            N = N_;
            Lattice.resize(N + 1);
            for (int n = 0; n <= N; n++) Lattice[n].resize(n + 1);
        }

        void SetNode(int n, int i, Type x) { Lattice[n][i] = x; }

        Type GetNode(int n, int i) const { return Lattice[n][i]; }

        int GetN() const { return N; }

        void Display(std::ostream& out = std::cout, int width = 15) const
        {
            out << std::setiosflags(std::ios::fixed) << std::setprecision(3);
            for (int n = 0; n <= N; n++)
            {
                for (int i = 0; i <= n; i++)
                    out << std::setw(width) << GetNode(n, i);
                out << std::endl;
            }
            out << std::endl;
        }
    };

}
}
