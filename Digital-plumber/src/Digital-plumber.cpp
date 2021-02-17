/* Digital plumber 12 A B */
#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_set>
#include <list>
#include <vector>
using std::cout;
using std::endl;
using setlist = std::list<std::unordered_set<int>>;

std::unordered_set<int> split(const std::string& s);
bool common_elements(const std::unordered_set<int>& A, const std::unordered_set<int>& B);


int main() {
	std::string line;
	std::ifstream myfile("input.txt");
	std::smatch wynik;
	std::regex wzorzec("([0-9]+) <-> (.*)");

	setlist bucket;

	getline(myfile, line);
	std::regex_search(line, wynik, wzorzec);
	std::unordered_set<int> current_set = split(wynik[2]);
	current_set.insert(stoi(wynik[1]));
	bucket.push_back(current_set);

	while ( getline(myfile, line) ) {
		// pobierz liczby z nowej linii
		std::regex_search(line, wynik, wzorzec);
		std::unordered_set<int> current_set = split(wynik[2]);
		current_set.insert(stoi(wynik[1]));

		// Wyznacz zbiory, ktore maja wspolne elementy z current_set
		std::vector<setlist::iterator> iters;
		for (setlist::iterator it = bucket.begin(); it != bucket.end(); it++)
			if ( common_elements(current_set, *it) )
				iters.push_back(it);


		// elementy iters wskazuja na zbiory pasujace do current_set:
		if (iters.size() == 0)
			bucket.insert(bucket.end(), current_set);	// dodaj current_set do wiadra

		else if (iters.size() == 1)						// dodaj current_set do zbioru w wiadrze
			iters[0]->insert(current_set.begin(), current_set.end());

		else {											// Polacz zbiory, usun duplikaty i dodaj current_set do main_set
			setlist::iterator main_set = iters[0];
			for (std::vector<setlist::iterator>::reverse_iterator it = iters.rbegin();
					std::distance(it, iters.rend()) > 1; it++) {
				std::unordered_set<int> set_to_cat = **it; // <-- verbose, bo zawila skladnia

				main_set->insert(set_to_cat.begin(), set_to_cat.end());
				bucket.erase(*it);
			}
			main_set->insert(current_set.begin(), current_set.end());
		}
	}

	int checksum = 0;
	int i = 0;
	/* wiadro[0]: 115 elementow. Wiader w sumie: 221 */
	for (auto x : bucket) {
		checksum += x.size();
		cout <<" Liczba elementow w wiadrze: " << i++ << ": " << x.size() << endl;
	}

	cout << "Checksum (wszystkich elementow) = " << checksum << endl;
	return 0;
}


std::unordered_set<int> split(const std::string& s) {
	std::unordered_set<int> tokens;

	std::string::const_iterator from = s.begin();
	for (std::string::const_iterator it = s.begin(); it != s.end(); it++) {
		if ( *it == ',' ) {
			std::string tmp_str(from, it);
			tokens.emplace(std::stoi(tmp_str));
			from = it + 2;	// delimiter to ', ', wiec skocz 2 do przodu
		}
		if (it == s.end()-1) {
			std::string tmp_str(from, it+1);
			tokens.emplace(std::stoi(tmp_str));
		}
	}
	return tokens;
}

bool common_elements(const std::unordered_set<int>& A, const std::unordered_set<int>& B) {
	for (int a : A) {
		std::unordered_set<int>::const_iterator it = B.find(a);
		if (it != B.end())
			return true;
	}
	return false;
}
