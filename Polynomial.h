#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <sstream>
class Polynomial;

class Term {
	friend Polynomial;
	friend std::ostream& operator<<(std::ostream&, Polynomial&); // out stream
	friend std::istream& operator>>(std::istream&, Polynomial&); // input stream
	friend void MergeSort(Term*, int, int);//used for merge sort
	friend void Merge(Term*, int, int, int);//used for merge sort
	friend std::string generatePolyFormat(Polynomial&); // out string format
private:
	float coef;
	int exp;
};

class Polynomial {
	friend std::ostream& operator<<(std::ostream&, Polynomial&); // out stream
	friend std::istream& operator>>(std::istream&, Polynomial&); // input stream
	friend Polynomial generateFromString(std::string); // generate Polynomial
	friend std::string generatePolyFormat(Polynomial&); // 
private:
	Term* termArray;
	int capacity;
	int terms = 0;
	bool isSorted = false; // check if is sorted

public:
	Polynomial();
	Polynomial Add(Polynomial);
	Polynomial Mult(Polynomial);
	float Eval(float f);
	void NewTerm(const float, const int);

	void sortTermArray(); // sort the termArray ensure that is sorted maximum at left
};

Polynomial generateFromString(std::string); // define
#endif
