#include "Polynomial.h"
#include <iostream>
#include <vector>
#include <windows.h>
#include <stdlib.h>
using namespace std;

class timer{
public:
	static LARGE_INTEGER startTime, endTime, fre;
	static double time;
	static void start_timer() {
		QueryPerformanceCounter(&startTime);
	}
	static void stop_timer() {
		QueryPerformanceCounter(&endTime);

		time = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;
	}
	static string getFormat() {
		char tmp[100];
		sprintf(tmp, "this action spends about %f second(s).",((float)time)/ CLOCKS_PER_SEC);// string format
		string timeformat(tmp);
		return timeformat;
	}
};

LARGE_INTEGER timer::startTime;
LARGE_INTEGER timer::endTime;
LARGE_INTEGER timer::fre;
double timer::time = 0;

void listAllPolynomial(vector<Polynomial> &polys,bool labeled = false) {
	cout << "total have (" << polys.size() << ") polys" << endl;
	int c = 0;
	cout << "-----------------------------" << endl;
	for (Polynomial p : polys) {
		if (labeled) cout << "(" << c++ << ")";
		cout << p << endl;
	}
	cout << "-----------------------------" << endl;
}

int showMenu(int currentChoose1, int currentChoose2, vector<Polynomial> &polys,string resultFormat = "") {
	cout << "=========================================" << endl;
	cout << "current chosen polynomial 1 : " << (currentChoose1 >= 0 ? generatePolyFormat(polys[currentChoose1]) : "Nan") << endl;
	cout << "current chosen polynomial 2 : " << (currentChoose2 >= 0 ? generatePolyFormat(polys[currentChoose2]) : "Nan") << endl;
	cout << "choose an option" << endl;
	cout << "(1) show all poly list" << endl;
	cout << "(2) choose polynomial 1" << endl;
	cout << "(3) choose polynomial 2" << endl;
	cout << "(4) add new terms to chosen polynomial 1" << endl;
	cout << "(5) add new terms to chosen polynomial 2" << endl;
	cout << "(6) remove current polynomial 1 from list" << endl;
	cout << "(7) remove current polynomial 2 from list" << endl;
	cout << "(8) generate new polynomial to poly list" << endl;
	cout << "(9) sum two chosen polynomial" << endl;
	cout << "(10) product two chosen polynomial" << endl;
	cout << "(11) eval chosen polynomial 1" << endl;
	cout << "(12) eval chosen polynomial 2" << endl;
	cout << "=========================================" << endl;

	if (!resultFormat.empty()) {
		cout << endl << endl << endl << endl;
		cout << "=========[action]=========" << endl;
		cout << resultFormat << endl;
		cout << "==========================" << endl;
	}

	int choose;
	cin >> choose;
	return choose;
}

string choosePolyFromList(vector<Polynomial>& polys, int &choose) {
	if (!polys.size()) return "[error] polynomial list is empty";
	listAllPolynomial(polys,true);
	cout << "choose one" << endl;
	int choosed;
	while (cin >> choosed && choosed != -1) {
		if (choosed >= 0 && choosed < polys.size()) {
			choose = choosed;
			break;
		}
		cout << "[invalid input] please choose number between 0 to " << polys.size() - 1 << endl;
		cout << "or enter -1 to exit" << endl;
	}
	system("cls");
	if (choosed == -1) return "[choose poly] didnt do any chnages";
	return "[choose poly] change poly(" + generatePolyFormat(polys[choose]) + ") to chosen polynomial";
	
}

string addTermToPolynomial(vector<Polynomial>& polys,int choose) {
	if (!polys.size()) return "[error] polynomial list is empty";
	if (choose == -1) return "[error] you have to choose a polynomial";
	cout << "current Poly : (" << polys[choose] << ")" << endl;
	cout << "Enter two value , coef and exp:";
	timer::start_timer();
	cin >> polys[choose]; // input 2 value , coef and exp
	timer::stop_timer();
	system("cls");
	return "[add term] chosen poly updated: " + generatePolyFormat(polys[choose])+ " \n"+timer::getFormat();
}

string removeElement(vector<Polynomial>& polys, int& choose, int& choose2) {
	if (choose == -1) return "[error] chosen polynomial is Nan";
	if (!polys.size()) return "[error] polynomial list is empty";
	polys.erase(polys.begin() + choose);
	if (choose == choose2) {
		choose2 = -1;
	}
	if (choose < choose2) {
		choose2--;
	}
	choose = -1;
	return "[remove poly] remove polynomial successfully";
}


string createElement(vector<Polynomial>& polys) {
	cout << "Please Input polynomial format with variable x (c.g. 4x+3 , empty is accaptable)" << endl;
	string s;
	cin.get();
	getline(cin,s);
	timer::start_timer();
	polys.push_back(generateFromString(s));
	timer::stop_timer();
	system("cls");
	return "[add poly] add polynomial successfully\n"+timer::getFormat();
}

string addPolynomial(vector<Polynomial>& polys, int choose1, int choose2) {
	if (choose1 == -1 || choose2 == -1) return "make sure two chosen polynomial are not Nan";
	timer::start_timer();
	Polynomial np = polys[choose1].Add(polys[choose2]);
	timer::stop_timer();
	cout << generatePolyFormat(polys[choose1]) << " + " << generatePolyFormat(polys[choose2]) << " get result:" << endl;
	cout << generatePolyFormat(np) << endl;
	cout << "add this poly to polynomial list? (Y/N)";
	char c;
	cin >> c;
	if (c == 'Y' || c == 'y') polys.push_back(np);
	system("cls");
	return "[sum action] sum polynomial successfully\n"+timer::getFormat();
}

string productPolynomial(vector<Polynomial>& polys, int choose1, int choose2) {
	if (choose1 == -1 || choose2 == -1) return "make sure two chosen polynomial are not Nan";
	timer::start_timer();
	Polynomial np = polys[choose1].Mult(polys[choose2]);
	timer::stop_timer();
	cout << generatePolyFormat(polys[choose1]) << " * " << generatePolyFormat(polys[choose2]) << " get result:" << endl;
	cout << generatePolyFormat(np) << endl;
	cout << "add this poly to polynomial list? (Y/N)";
	char c;
	cin >> c;
	if (c == 'Y' || c == 'y') polys.push_back(np);
	system("cls");
	return "[product action] product polynomial successfully\n" + timer::getFormat();
}

string evalPolynomial(vector<Polynomial>& polys, int choose) {
	if (!polys.size()) return "[error] polynomial list is empty";
	if (choose == -1) return "[error] you have to choose a polynomial";
	float f;
	cout << "f(x)=" << generatePolyFormat(polys[choose]) << endl;
	cout << "enter an number(float): "; cin >> f;
	timer::start_timer();
	cout << "f(" << f << ")= " << polys[choose].Eval(f) << endl;
	timer::stop_timer();
	cout << "press any key to continue..." << endl;
	cin.get();
	getchar();
	system("cls");
	return "[eval action] eval action successfully\n" + timer::getFormat();
}


int main() {
	QueryPerformanceFrequency(&timer::fre);//ªì©l¤Æ frequency
	int currentChoose1 = -1, currentChoose2 = -1;
	vector<Polynomial> polys(0);
	string resultFormat;
	while (true) {
		int choose = showMenu(currentChoose1, currentChoose2, polys,resultFormat);
		system("cls");
		resultFormat.clear();
		
		switch (choose) {
		case 1: listAllPolynomial(polys); break;
		case 2: resultFormat = choosePolyFromList(polys, currentChoose1); break;
		case 3: resultFormat = choosePolyFromList(polys, currentChoose2); break;
		case 4: resultFormat = addTermToPolynomial(polys, currentChoose1); break;
		case 5: resultFormat = addTermToPolynomial(polys, currentChoose2); break;
		case 6: resultFormat = removeElement(polys, currentChoose1, currentChoose2); break;
		case 7: resultFormat = removeElement(polys, currentChoose2, currentChoose1); break;
		case 8: resultFormat = createElement(polys); break;
		case 9: resultFormat = addPolynomial(polys, currentChoose2, currentChoose1); break;
		case 10: resultFormat = productPolynomial(polys, currentChoose2, currentChoose1); break;
		case 11: resultFormat = evalPolynomial(polys, currentChoose1); break;
		case 12: resultFormat = evalPolynomial(polys, currentChoose2); break;
		}
		
	}
}
