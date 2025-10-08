#include <iostream>

class DefInt {
    private:
        double a, b;
        double (*f)(double x);
        const double initial_sum;

    public:
        // Here we set the initial sum to avoid recalculation in member functions
        DefInt(double a_, double b_, double (*f_)(double x)) 
            : a(a_), b(b_), f(f_), initial_sum(f(a)+f(b)) {}

        double ByTrapezoid(int N);

        double BySimpson(int N);
};

auto f = [](double x) -> double { return (x+1); };

double DefInt :: ByTrapezoid(int N) {
    double h = (b - a) / N;
    double sum  = initial_sum;
    
    for(int i = 1; i < N; i++)
        sum += 2*f(a+(i*h));

    return (h/2)*sum;
}

double DefInt :: BySimpson(int N){
    double sum  = initial_sum;

    if(N % 2 != 0) {
        std::cerr << "N needs to be even for the Simpson method" << std::endl; 
        return 0.0;
    }

    double h = (b - a) / N;
    for(int i = 1; i < N; i++)
        sum += (i % 2 == 0 ? 2 : 4) * f(a+i*h);
    
    return (h/3)*sum;
}

int main(){
    DefInt myInt(1.0, 2.0, f);

    std::cout << "By Trapezoid: " << myInt.ByTrapezoid(1000) << std::endl;
    std::cout << "By Simpson: " << myInt.BySimpson(1000) << std::endl;
}

/*
    By Trapezoid: 2.5
    By Simpson: 2.5
*/