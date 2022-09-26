#include <iostream>

bool FINISHED = false;
const int table[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
bool check(int *t);
int find(const int *tab, const int &val);
void print(int *tab);
void print(bool *tab);
int find(const bool *tab, const bool &val); // p.d na templaty
bool findNine(const int *tab, const int p, const int &val);

void bruteForce(int *tab, bool *info, int p) {
	if (FINISHED)
		return;

	if (p == 8) {
		int i = find(info, true);
		//print(info);
		tab[p] = i + 1;
		FINISHED = check(tab);
		print(tab);
		if (FINISHED)
			std::cout << "ZNALEZIONO \n";
	}
	else {
		for (int i = 1; i <= 9; i++) {
			if (info[i - 1] == true) { // wartość i w miejscu p jest dostępna
				if (tab[p] != 0) { // Wykonaj, dla wszystkich iteracji poza pierwszą
					const int idx = find(table, tab[p]);
					info[idx] = true;
					//print(info);
				}
				tab[p] = i;
				info[i - 1] = false;
				//print(info);
				bruteForce(tab, info, p + 1);
			}
		} // Funkcja się zwija - udostępnij wartość "9" - jeżeli nie ma jej po lewej stronie
		if (! findNine(tab, p, 9) ) {
			const int lastIdx = 8;
			info[lastIdx] = true;
		}
	}
}

int main()
{
	int tab[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	bool infoTab[9] = { true, true, true, true, true, true, true, true, true };
	bruteForce(tab, infoTab, 0);
	print(tab);

	return 0;
}

bool check(int *t) {
	const int left = t[0] * (10 * t[4] + t[5]) * (10 * t[7] + t[8]) +
		t[3] * (10 * t[1] + t[2]) * (10 * t[7] + t[8]) +
		t[6] * (10 * t[1] + t[2]) * (10 * t[4] + t[5]);
	const int right = (10 * t[1] + t[2]) * (10 * t[4] + t[5]) * (10 * t[7] + t[8]);
	if (left == right)
		return true;
	return false;
}

int find(const int *tab, const int &val) {
	for (int i = 0; i < 9; i++) {
		if (tab[i] == val)
			return i;
	}
	std::cout << "Nie znaleziono elementu tablicy O.o \n";
}

bool findNine(const int *tab, const int p, const int &val) {
	for (int i = 0; i < p; i++) {
		if (tab[i] == val)
			return true;
	}
	return false;
}

int find(const bool *tab, const bool &val) {
	for (int i = 0; i < 9; i++) {
		if (tab[i] == val)
			return i;
	}
	std::cout << "Nie znaleziono elementu tablicy O.o \n";
}

void print(int *tab) {
	for (int i = 0; i < 9; i++) {
		std::cout << tab[i] << "  ";
	}
	std::cout << '\n';
}

void print(bool *tab) {
	std::cout << "info Tab: \n";
	for (int i = 0; i < 9; i++) {
		std::cout << tab[i] << "  ";
	}
	std::cout << '\n';
}
