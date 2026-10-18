// Regression tests.
//
// Deterministic results are checked to ~1e-12 against reference values. Monte
// Carlo results are checked two ways:
//   - statistically, against known prices, within a few standard errors;
//   - exactly, with a fixed seed. These reference values depend on the
//     platform's std::rand() sequence, so they only run on Apple platforms,
//     where they were recorded.

#include "deriv/deriv.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace deriv;

namespace {
    int checks = 0;
    int failures = 0;

    void CheckClose(const string& name, double actual, double expected, double tol)
    {
        ++checks;
        if (!(fabs(actual - expected) <= tol))
        {
            ++failures;
            cout << "  FAIL  " << name << ": got " << setprecision(15) << actual
                 << ", expected " << expected << " (tolerance " << tol << ")" << endl;
        }
    }

    // relative tolerance for results that must reproduce the reference exactly
    void CheckExact(const string& name, double actual, double expected)
    {
        CheckClose(name, actual, expected, 1e-11 * max(1.0, fabs(expected)));
    }

    void Check(const string& name, bool condition)
    {
        ++checks;
        if (!condition)
        {
            ++failures;
            cout << "  FAIL  " << name << endl;
        }
    }

    // runs f with std::cout redirected, returning what it printed
    template<typename F>
    string CaptureOutput(F f)
    {
        ostringstream captured;
        streambuf* old = cout.rdbuf(captured.rdbuf());
        f();
        cout.rdbuf(old);
        return captured.str();
    }

    double Linear(double x) { return x + 1; }
    double Cubic(double x) { return x * x * x - x * x + 1; }

    binomial::BinomialTreeModel ReferenceTree()
    {
        return binomial::BinomialTreeModel(106.0, 1.15125, 0.86862, 1.00545);
    }

    binomial::BinomialTreeModel CalibratedTree(int N)
    {
        double S0 = 106.0, r = 0.058, sigma = 0.46, T = 0.75;
        double h = T / N, U = exp(sigma * sqrt(h));
        return binomial::BinomialTreeModel(S0, U, 1.0 / U, exp(r * h));
    }

    // ------------------------------------------------------------ numerics

    void TestNumerics()
    {
        numerics::DefiniteIntegral linear(1.0, 2.0, Linear);
        CheckExact("trapezoid, x+1", linear.ByTrapezoid(1000), 2.5);
        CheckExact("Simpson, x+1", linear.BySimpson(1000), 2.5);

        numerics::DefiniteIntegral cubic(1.0, 2.0, Cubic);
        CheckExact("trapezoid, cubic", cubic.ByTrapezoid(1000), 2.41666725);
        CheckExact("Simpson, cubic", cubic.BySimpson(1000), 29.0 / 12.0);

        using namespace numerics;
        Vector V(2), W(2);
        V[0] = 1.0; V[1] = 2.0;
        W[0] = 3.0; W[1] = 4.0;
        Matrix C(2, Vector(2));
        C[0][0] = 1.0; C[0][1] = 2.0;
        C[1][0] = 3.0; C[1][1] = 4.0;
        CheckExact("dot product", V ^ W, 11.0);
        CheckExact("matrix * vector", (C * V)[1], 11.0);
        CheckExact("element-wise product", (V * W)[1], 8.0);
        CheckExact("scalar * vector", (2.0 * V)[1], 4.0);
        CheckExact("vector + vector", (V + W)[0], 4.0);
        CheckExact("scalar + vector", (1.0 + V)[0], 2.0);
        CheckExact("element-wise exp", numerics::exp(V)[0], std::exp(1.0));
    }

    // ------------------------------------------------------------ binomial

    void TestBinomial()
    {
        using namespace binomial;
        double S0 = 106.0, U = 1.15125, D = 0.86862, R = 1.00545, K = 100.0;
        int N = 8;

        // closed-form CRR formula
        CheckExact("CRR formula call", PriceByCRRFormula(S0, U, D, R, N, K), 21.6811023816929);
        CheckExact("CRR formula put", PriceByCRRFormula(S0, U, D, R, N, K, PutPayoff), 11.4261175757024);
        CheckExact("CRR formula digital call", PriceByCRRFormula(S0, U, D, R, N, K, DigitCallPayoff), 0.575897327724042);
        CheckExact("CRR formula digital put", PriceByCRRFormula(S0, U, D, R, N, K, DigitPutPayoff), 0.381552824216053);

        // backward induction
        BinomialTreeModel Model = ReferenceTree();
        Call call(N, K);
        Put put(N, K);
        BullSpread bull(N, 100.0, 110.0);
        BearSpread bear(N, 100.0, 110.0);
        Strangle strangle(N, 100.0, 110.0);
        Butterfly butterfly(N, 100.0, 110.0);
        DoubleDigital doubleDigital(N, 100.0, 110.0);
        CheckExact("backward induction call", BinomialPricer(&call).PriceByCRR(Model), 21.6811023816929);
        CheckExact("backward induction put", BinomialPricer(&put).PriceByCRR(Model), 11.4261175757024);
        CheckExact("bull spread", BinomialPricer(&bull).PriceByCRR(Model), 4.71583969105227);
        CheckExact("bear spread", BinomialPricer(&bear).PriceByCRR(Model), 4.85866182834868);
        CheckExact("strangle", BinomialPricer(&strangle).PriceByCRR(Model), 28.3913802663431);
        CheckExact("butterfly", BinomialPricer(&butterfly).PriceByCRR(Model), 1.04313358618815);
        CheckExact("double digital", BinomialPricer(&doubleDigital).PriceByCRR(Model), 0.260749538281705);

        // replicating strategy (reference lattices printed to 3 decimals)
        BinLattice<double> X, Y;
        CheckExact("replication call price", BinomialPricer(&call).PriceByCRR(Model, X, Y), 21.6811023816929);
        CheckClose("call X(0,0)", X.GetNode(0, 0), 0.672, 5e-4);
        CheckClose("call Y(0,0)", Y.GetNode(0, 0), -49.518, 5e-4);
        CheckClose("call X(7,3)", X.GetNode(7, 3), 0.231, 5e-4);
        CheckClose("call Y(7,3)", Y.GetNode(7, 3), -18.339, 5e-4);
        CheckExact("call replication cost", X.GetNode(0, 0) * S0 + Y.GetNode(0, 0), 21.6811023816929);
        CheckExact("replication put price", BinomialPricer(&put).PriceByCRR(Model, X, Y), 11.4261175757024);
        CheckClose("put X(0,0)", X.GetNode(0, 0), -0.328, 5e-4);
        CheckClose("put Y(0,0)", Y.GetNode(0, 0), 46.227, 5e-4);
        CheckClose("put X(7,3)", X.GetNode(7, 3), -0.769, 5e-4);
        CheckClose("put Y(7,3)", Y.GetNode(7, 3), 81.119, 5e-4);

        // calibrated tree: European replication and American (Snell) prices
        BinomialTreeModel tree = CalibratedTree(8);
        CheckExact("calibrated call", BinomialPricer(&call).PriceByCRR(tree, X, Y), 21.6817078655976);
        CheckExact("calibrated call X(0,0)", X.GetNode(0, 0), 0.671702744658536);
        CheckExact("calibrated call Y(0,0)", Y.GetNode(0, 0), -49.5187830682072);
        CheckExact("calibrated put", BinomialPricer(&put).PriceByCRR(tree, X, Y), 11.4249632746943);
        CheckExact("calibrated put X(0,0)", X.GetNode(0, 0), -0.328297255341464);
        CheckExact("calibrated put Y(0,0)", Y.GetNode(0, 0), 46.2244723408895);

        BinLattice<double> PriceTree;
        BinLattice<bool> StoppingTree;
        CheckExact("American call", BinomialPricer(&call).PriceBySnell(tree, PriceTree, StoppingTree), 21.6817078655976);
        CheckExact("American put", BinomialPricer(&put).PriceBySnell(tree, PriceTree, StoppingTree), 11.7229402897614);
        Check("American put exercised deep in the money", StoppingTree.GetNode(7, 0));
        Check("American put held at the root", !StoppingTree.GetNode(0, 0));

        Call call1024(1024, K);
        CheckExact("calibrated call, N = 1024", BinomialPricer(&call1024).PriceByCRR(CalibratedTree(1024)), 21.5666229167877);

        // input validation
        int ok = 0, arbitrageHigh = 0, arbitrageLow = 0, illegal = 0;
        string out = CaptureOutput([&]() {
            ok = Model.ValidateInputData();
            arbitrageHigh = BinomialTreeModel(100.0, 1.1, 0.9, 1.2).ValidateInputData();
            arbitrageLow = BinomialTreeModel(100.0, 1.1, 0.9, 0.8).ValidateInputData();
            illegal = BinomialTreeModel(100.0, 0.9, 1.1, 1.0).ValidateInputData();
        });
        Check("valid model accepted", ok == 0);
        Check("R >= U rejected", arbitrageHigh == -1);
        Check("R <= D rejected", arbitrageLow == -1);
        Check("U <= D rejected", illegal == -1);
        Check("validation reports arbitrage", out.find("Arbitrage exists") != string::npos);
    }

    // ------------------------------------------------------------ analytic

    void TestAnalytic()
    {
        using namespace analytic;

        GeneralizedBlackScholes call(0.08, 0.30, 65.0, 0.25, 60.0, 0.08);
        CheckExact("generalized BS call", call.Price(), 2.13292841871403);
        CheckExact("generalized BS call delta", call.Delta(), 0.372492167139648);

        GeneralizedBlackScholes put(0.10, 0.30, 50.0, 0.25, 50.0, 0.10, "P");
        CheckExact("generalized BS put", put.Price(), 2.37561138342553);
        CheckExact("generalized BS put delta", put.Delta(), -0.404522297636484);

        GeneralizedBlackScholes dividendPut(0.10, 0.30, 50.0, 0.25, 50.0 - 1.5 * exp(-0.10 * 2.0 / 12.0), 0.10, "P");
        CheckExact("generalized BS put with dividend", dividendPut.Price(), 3.03038771093756);

        // rule of three: copies are deep, assignment and Toggle manage the string
        GeneralizedBlackScholes copy(put);
        copy.Toggle();
        Check("toggle switches to call", strcmp(copy.GetOptType(), "C") == 0);
        Check("copy is independent", strcmp(put.GetOptType(), "P") == 0);
        CheckExact("put-call parity", copy.Price() - put.Price(), 50.0 - 50.0 * exp(-0.10 * 0.25));
        copy = put;
        Check("assignment copies the type", strcmp(copy.GetOptType(), "P") == 0);
        CheckExact("assignment copies the data", copy.Price(), put.Price());
        GeneralizedBlackScholes& self = copy;
        copy = self;
        CheckExact("self-assignment is harmless", copy.Price(), put.Price());

        BlackScholesCall bs(0.75, 100.0);
        CheckExact("BS call price", bs.PriceByBSFormula(106.0, 0.46, 0.058), 21.5637534895449);
        CheckExact("BS call vega", bs.VegaByBSFormula(106.0, 0.46, 0.058), 33.0263953965208);
        CheckExact("BS call delta", bs.DeltaByBSFormula(106.0, 0.46, 0.058), 0.675318749931757);

        CheckClose("NormalCDF(0)", NormalCDF(0.0), 0.5, 1e-7);
        CheckClose("NormalCDF symmetry", NormalCDF(1.3) + NormalCDF(-1.3), 1.0, 1e-12);
        CheckClose("NormalCDF(1.96)", NormalCDF(1.96), 0.5 * erfc(-1.96 / sqrt(2.0)), 1e-7);

        ImpliedVolatility iv(0.75, 100.0, 106.0, 0.058);
        numerics::NonlinearSolver solver(bs.PriceByBSFormula(106.0, 0.46, 0.058), 0.01, 2.0, 1e-8, 0.3);
        CheckClose("implied vol, Newton-Raphson", solver.SolveByNR(&iv), 0.46, 1e-7);
        CheckClose("implied vol, bisection", solver.SolveByBisect(&iv), 0.46, 1e-7);
    }

    // ------------------------------------------------------------ fixed income

    void TestFixedIncome()
    {
        fixedincome::BondYield bond(100.0, 3.0, 1.2, 98.56);
        numerics::NonlinearSolver tight(0.0, 0.0, 1.0, 1e-14, 0.2);
        double root = tight.SolveByNR(&bond);
        CheckClose("bond yield is a root", bond.Value(root), 0.0, 1e-10);
        CheckClose("bond yield value", root, 0.016821, 5e-7);

        numerics::NonlinearSolver solver(0.0, 0.0, 1.0, 0.0001, 0.2);
        double yBisect = solver.SolveByBisect(&bond);
        CheckExact("bond yield, bisection", yBisect, 0.01678466796875);
        CheckClose("bisection within its accuracy", yBisect, root, 0.0001);
        CheckClose("bond yield, Newton-Raphson", solver.SolveByNR(&bond), root, 1e-8);

        // analytic derivative against a central difference
        double h = 1e-6;
        CheckClose("bond derivative", bond.Deriv(0.05), (bond.Value(0.05 + h) - bond.Value(0.05 - h)) / (2 * h), 1e-5);
    }

    // ------------------------------------------------------------ Monte Carlo

    void TestMonteCarlo()
    {
        using namespace montecarlo;
        double T = 1.0 / 12.0, K = 100.0;
        int m = 30;
        long N = 30000;
        double epsilon = 0.001;

        GBMModel Model(100.0, 0.03, 0.2);
        ArithmeticAsianCall option(T, K, m);
        GeometricAsianCall cv(T, K, m);

        CheckExact("geometric Asian closed form", cv.PriceByBSFormula(Model), 1.41144009164914);
        CheckExact("geometric Asian closed-form delta", cv.DeltaByBSFormula(Model), 0.51788072552899);

        // statistical: both estimators agree with the reference price ~1.4261
        option.PriceByVarRedMC(Model, N, cv, epsilon);
        double cvPrice = option.GetPrice();
        CheckClose("control variate price", cvPrice, 1.4261, 0.001);
        Check("control variate error is small", option.GetPricingError() < 0.0005);
        option.PriceByMC(Model, N, epsilon);
        CheckClose("direct MC price", option.GetPrice(), cvPrice, 5 * option.GetPricingError());
        CheckClose("direct MC delta", option.GetDelta(), 0.52, 0.03);
        CheckClose("direct MC gamma", option.GetGamma(), 0.115, 0.06);

        // the geometric Asian estimated by simulation matches its closed form
        cv.PriceByMC(Model, N, epsilon);
        CheckClose("geometric Asian MC vs closed form", cv.GetPrice(), 1.41144009164914, 5 * cv.GetPricingError());

#ifdef __APPLE__
        srand(12345);
        option.PriceByMC(Model, N, epsilon);
        CheckExact("seeded direct MC price", option.GetPrice(), 1.41367823591921);
        CheckExact("seeded direct MC error", option.GetPricingError(), 0.0119904361318033);
        CheckExact("seeded direct MC delta", option.GetDelta(), 0.52238186570987);
        CheckExact("seeded direct MC gamma", option.GetGamma(), 0.116108652458952);

        srand(12345);
        option.PriceByVarRedMC(Model, N, cv, epsilon);
        CheckExact("seeded control variate price", option.GetPrice(), 1.42604868961853);
        CheckExact("seeded control variate error", option.GetPricingError(), 0.000137596487276853);
        CheckExact("seeded control variate delta", option.GetDelta(), 0.519978027253475);
#endif
    }

    void TestBasket()
    {
        using namespace montecarlo::basket;
        int d = 3;
        Vector S0(d);
        S0[0] = 40.0; S0[1] = 60.0; S0[2] = 100.0;
        Matrix C(d, Vector(d));
        C[0][0] = 0.1;  C[0][1] = -0.1; C[0][2] = 0.0;
        C[1][0] = -0.1; C[1][1] = 0.2;  C[1][2] = 0.0;
        C[2][0] = 0.0;  C[2][1] = 0.0;  C[2][2] = 0.3;
        GBMModel Model(S0, 0.03, C);
        ArithmeticAsianCall option(1.0 / 12.0, 200.0, 30);

        CheckExact("basket sigma[0]", Model.GetSigma()[0], 0.14142135623731);
        CheckExact("basket sigma[1]", Model.GetSigma()[1], 0.223606797749979);
        CheckExact("basket sigma[2]", Model.GetSigma()[2], 0.3);

        // reference from 2,000,000 paths: 2.2474 +/- 0.0023; one standard error at 30,000 paths is ~0.019
        CheckClose("basket price", option.PriceByMC(Model, 30000), 2.2474, 0.08);

#ifdef __APPLE__
        srand(12345);
        CheckExact("seeded basket price", option.PriceByMC(Model, 3000), 2.34447114128432);
        Vector delta = option.DeltaByMC(Model, 0.001, 100000);
        CheckExact("basket delta[0]", delta[0], 0.506403591594007);
        CheckExact("basket delta[1]", delta[1], 0.513270675413831);
        CheckExact("basket delta[2]", delta[2], 0.530540854870205);
#endif
    }
}

int main()
{
    cout << "Running deriv regression tests" << endl;
    TestNumerics();
    TestBinomial();
    TestAnalytic();
    TestFixedIncome();
    TestMonteCarlo();
    TestBasket();

    cout << checks - failures << " / " << checks << " checks passed" << endl;
    return failures == 0 ? 0 : 1;
}
