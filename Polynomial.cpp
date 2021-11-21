#include "Polynomial.h"


std::ostream& operator<<(std::ostream& os, Polynomial& poly) {
	os << generatePolyFormat(poly);
	return os;
}

std::istream& operator>>(std::istream& is, Polynomial& poly) {
	float coef;
	int exp;
	is >> coef >> exp;
	Polynomial add_poly;
	add_poly.NewTerm(coef, exp);
	poly = poly.Add(add_poly);
	poly.isSorted = false; // when doing Add() function have to be check if sorted
	return is;
}

Polynomial::Polynomial() {
	capacity = 1;
	terms = 0;
	termArray = new Term[capacity];
	isSorted = true;
}

Polynomial Polynomial::Add(Polynomial b) {
	Polynomial c;

	sortTermArray();  // sort *this term array
	b.sortTermArray();// sort b term array

	int aPos = 0, bPos = 0;
	while ((aPos < terms) && (bPos < b.terms)) {
		if (termArray[aPos].exp == b.termArray[bPos].exp) {
			float t = termArray[aPos].coef + b.termArray[bPos].coef;
			if (t) c.NewTerm(t, termArray[aPos].exp);
			aPos++;
			bPos++;
		}
		else if (termArray[aPos].exp < b.termArray[bPos].exp) {
			c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
			bPos++;
		}
		else {
			c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
			aPos++;
		}
	}
	for (; aPos < terms; aPos++) {
		c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
	}
	for (; bPos < b.terms; bPos++) {
		c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
	}
	return c;
}

Polynomial Polynomial::Mult(Polynomial b) {
	Polynomial c;
	// doing multiple dont need to sort term array

	std::map<int, float> tempTermArray; //map find logn same as binary search
	// exp -> coef and which is sorted
	for (int i = 0; i < terms; i++) {
		for (int j = 0; j < b.terms; j++) {
			int exp = termArray[i].exp + b.termArray[j].exp;
			float coef = termArray[i].coef * b.termArray[j].coef;


			if (tempTermArray.find(exp) == tempTermArray.end()) {
				tempTermArray[exp] = coef;
			}
			else tempTermArray[exp] += coef;
		}
	}
	//(m+n)log(m+n)

	for (std::map<int, float>::reverse_iterator it = tempTermArray.rbegin(); it != tempTermArray.rend(); it++) {
		c.NewTerm(it->second, it->first);
	}

	return c;

}


void Polynomial::NewTerm(const float theCoeff, const int theExp) {
	if (terms == capacity) {
		capacity *= 2;
		Term* temp = new Term[capacity];
		std::copy(termArray, termArray + terms, temp);
		delete[] termArray;
		termArray = temp;
	}
	termArray[terms].coef = theCoeff;
	termArray[terms++].exp = theExp;
}

float Polynomial::Eval(float f) {
	float result = 0.0;
	for (int i = 0; i < terms; i++) {
		result += termArray[i].coef * pow(f, termArray[i].exp);
	}
	return result;
}


void Polynomial::sortTermArray() { // sort term array
	if (isSorted) return;

	MergeSort(termArray, 0, terms - 1);

	isSorted = true;
}



void MergeSort(Term* termArray, int left, int right) { // merge sort O(nlogn) 
	if (left < right) {
		int mid = (left + right) / 2;
		MergeSort(termArray, left, mid);
		MergeSort(termArray, mid + 1, right);
		Merge(termArray, left, mid, right);
	}

}
void Merge(Term* termArray, int left, int middle, int right) {
	int n1 = middle - left + 1;
	int n2 = right - middle;
	Term* leftArray = new Term[n1 + 1];
	Term* rightArray = new Term[n2 + 1];

	for (int i = 0; i < n1; i++) leftArray[i] = termArray[left + i];
	for (int j = 0; j < n2; j++) rightArray[j] = termArray[middle + j + 1];


	Term minimum;
	minimum.coef = -10000.0;
	minimum.exp = -100000;
	leftArray[n1] = minimum;
	rightArray[n2] = minimum;

	int leftIndex = 0;
	int rightIndex = 0;
	for (int i = left; i <= right; i++) {
		if (leftArray[leftIndex].exp == rightArray[rightIndex].exp) {
			if (leftArray[leftIndex].coef > rightArray[rightIndex].coef) {
				termArray[i] = leftArray[leftIndex++];
			}
			else {
				termArray[i] = rightArray[rightIndex++];
			}
		}
		else if (leftArray[leftIndex].exp > rightArray[rightIndex].exp) {
			termArray[i] = leftArray[leftIndex++];
		}
		else {
			termArray[i] = rightArray[rightIndex++];
		}
	}
}

Polynomial generateFromString(std::string s) {
	//x^n...+1
	Polynomial result;
	result.isSorted = false;
	std::string temp = "";
	float coef = 0;
	int exp = 0;
	bool nextMinus = false;
	for (int i = 0; i < s.length(); i++) {
		if (isspace(s[i])) continue;
		if (s[i] == 'x') {
			if (i < s.length() && s[i + 1] != '^') {
				exp = 1;
				continue;
			}
			else i++; // change to next char ^
			std::string temp_exp;
			bool minus = false;
			if (s[i+1] == '-') {
				minus = true;
				i++;
			}
			while (i < s.length() && s[i + 1] != '+' && s[i + 1] != '-') {
				i++;
				temp_exp += s[i];
			}
			exp = stoi(temp_exp);
			if (minus) exp *= -1;
		}
		else if (s[i] == '+' || s[i] == '-') {
			if (!temp.empty())coef = stof(temp);
			else if(exp != 0) coef = 1;
			temp.clear();
			Polynomial add_poly;
			add_poly.NewTerm(coef * (nextMinus ? -1 : 1), exp);
			result = result.Add(add_poly);
			coef = 0;
			exp = 0;
			if (s[i] == '-') nextMinus = true;
			else nextMinus = false;
		}
		else temp += s[i];
	}
	if (!temp.empty()) coef = stof(temp);
	else if(exp != 0) coef = 1;

	if (coef != 0) {
		Polynomial add_poly;
		add_poly.NewTerm(coef * (nextMinus ? -1 : 1), exp);
		result = result.Add(add_poly);
	}
	return result;
}

std::string generatePolyFormat(Polynomial& poly) {
	std::string s;
	for (int i = 0; i < poly.terms; i++) {
		std::stringstream ss;
		std::string coef, exp;
		ss << poly.termArray[i].coef; ss >> coef;
		ss.clear();
		ss << poly.termArray[i].exp; ss >> exp;
		if (i && poly.termArray[i].coef > 0) s += "+";
		if (poly.termArray[i].coef == -1) s += "-";
		s += ((poly.termArray[i].coef == 1 || poly.termArray[i].coef == -1) && poly.termArray[i].exp != 0 ? "" : coef) + (poly.termArray[i].exp != 0 ? (poly.termArray[i].exp == 1 ? "x" : "x^" + exp) : "");
	}
	return s;
}