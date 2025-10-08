#pragma once
namespace fre
{
    class Function
    {
    public:
        virtual double Value(double y) = 0;
        virtual double Deriv(double y) = 0;
        virtual ~Function() {}
    };

    class F1 : public Function
    {
    private:
        double F = 100.0;
        double T = 3.0;
        double C = 1.2;
        double P = 98.56;

    public:
        double Value(double y);
        double Deriv(double y);
    };
    // class F2 : public Function
    // {
    // private:
    //     double a; // any real number
    // public:
    //     F2(double a_) : a(a_) {}
    //     double Value(double x);
    //     double Deriv(double x);
    // };
}