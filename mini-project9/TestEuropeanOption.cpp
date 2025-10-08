// TestEuropeanOption.cpp
// Test program for the exact solutions of European options.
// (C) Datasim Component Technology BV 2003-2006
//

#include "EuropeanOption.hpp"
#include <iostream>
#include <cmath>

int main()
{
	// Call option on a stock
	double r = 0.08;
	double sig= 0.30;
	double K = 65.0;
	double T = 0.25;
	double U = 60.0;		// U == stock in this case
	double b = r;	

	EuropeanOption callOption(r, sig, K ,T, U, b);
	cout << "Q1: Call option on a stock: " << callOption.Price() << endl;

	// Put option on a stock index
	r = 0.1;
	sig= 0.30;
	K = 50.0;
	T = 0.25;
	U = 50.0;		// U == stock in this case
	b = r;
	char* optType = (char*)"P";
	EuropeanOption indexOption(r, sig, K, T, U, b, optType);

	cout << "Q2: Put option on a stock: " << indexOption.Price() << endl;

	double D = 1.5;
	double t = 2.0 / 12.0;
	U = 50 - D * exp(-r * t);

	EuropeanOption dividendOption(r, sig, K, T, U, b, optType);

	cout << "Q3: Put option with dividend: " << dividendOption.Price() << endl;



	return 0;
}

/*
Q1: Call option on a stock: 2.13293
Q2: Put option on a stock: 2.37561
Q3: Put option with dividend: 3.03039
*/

