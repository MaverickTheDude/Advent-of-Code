#include <iostream>
using std::string;
using std::cout;
using std::endl;
typedef long long unsigned int ulongint;

#define partA false

const int Astart  = 618;
const int Bstart  = 814;
const int AFACTOR = 16807;
const int BFACTOR = 48271;
const int PAD     = 65535; 			 	 // 2^16-1
const unsigned int DIVIDOR = 2147483647; // 2^31-1

ulongint nextA();
ulongint nextB();
ulongint nextA_picky();
ulongint nextB_picky();
bool check(ulongint A, ulongint B);


int main() {
	// part B by default
	ulongint (*funA)() = &nextA_picky;
	ulongint (*funB)() = &nextB_picky;
	int attempts = 5e6;
	if (partA) {
		funA = &nextA;
		funB = &nextB;
		attempts = 4e7;
	}

	int matches = 0;
	for (int i = 0; i < attempts; i++)
			if (  check(funA(), funB() )  )
				matches++;

	cout << "matching numbers: " << matches << endl;
	return 0;
}

ulongint nextA() {
	static ulongint A = Astart;
	A = (A * AFACTOR) % DIVIDOR;
	return A;
}

ulongint nextB() {
	static ulongint B = Bstart;
	B = (B * BFACTOR) % DIVIDOR;
	return B;
}

ulongint nextA_picky() {
	static ulongint A = Astart;
	do
		A = nextA();  // podaj kolejne A dopoki...
	while
		(A % 4 != 0); // ...A niepodzielne przez 4
	return A;
}

ulongint nextB_picky() {
	static ulongint B = Bstart;
	do
		B = nextB();
	while
		(B % 8 != 0);
	return B;
}

bool check(ulongint A, ulongint B) {
	// Ostatnie 16 bitow liczb A i B takie same ?
	A = A & PAD;
	B = B & PAD;
	return (A == B) ? true : false;
}
