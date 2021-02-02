#include <iostream>
#include <fstream>
#include <regex>
#include <map>
using std::cout;	using std::map;
using std::endl;	using std::string;

// switch - case dla const char* (funkcja condition)
/*https://hbfs.wordpress.com/2017/01/10/strings-in-c-switchcase-statements/*/
uint64_t constexpr mix(char m, uint64_t s) {return ((s<<7) + ~(s>>3)) + ~m;      }
uint64_t constexpr hash(const char * m)    {return (*m) ? mix(*m,hash(m+1)) : 0; }
bool condition(const map<string, int>&, const string&, const char*, const string&);


int main() {
	string line;
	std::ifstream myfile("src\\input.txt");
	std::smatch wynik;
	std::regex wzorzec("([a-z]+)\\s");
	int max = 0;

	// Inicjalizacja mapy
	map<string, int> registers;
	map<string, int>::const_iterator it;
	while( getline(myfile, line) ) {
		std::regex_search(line, wynik, wzorzec);
		const string key = wynik[1].str();
		it = registers.find(key);
		if (it == registers.end())
			registers[key] = 0;
	}

	// re-read input file
	myfile.clear();
	myfile.seekg(0, myfile.beg);

	//			 (1)	  (2)		 (3)		 (4)	 (5)		(6)
	wzorzec = "([a-z]+) ([a-z]+) (-?[0-9]+) if ([a-z]+) (.?.?) (-?[0-9]+)";
	/*int i = 1;*/
	while( getline(myfile, line) ) {
		std::regex_search(line, wynik, wzorzec);
		const char* operand = wynik[5].str().c_str();
		bool cond = condition(registers, wynik[4].str(), operand, wynik[6].str());
/*		cout << "krok: " << i++ << " reg: " << registers.find(wynik[1].str())->first <<
				"\tcur value: " << registers.find(wynik[1].str())->second << " ";*/
		if (cond) {
			if (! wynik[2].str().compare("inc"))		// note: false, jesli jest match (!!!)
				registers[wynik[1].str()] += stoi(wynik[3].str());
			else if (! wynik[2].str().compare("dec"))
				registers[wynik[1].str()] -= stoi(wynik[3].str());
			else
				throw "no match for inc/dec value";
			if (registers.find(wynik[1].str())->second > max)
				 max = registers.find(wynik[1].str())->second;
			/*cout << "new value: " << registers.find(wynik[1].str())->second;*/
		}
		/*cout << endl;*/
	}

	it = registers.begin();
	int last_max = it->second;
	for (auto x : registers)
		last_max = (x.second > last_max) ? x.second : last_max;

	cout << "max register value ever: " << max << endl;
	cout << "max register value at final: " << last_max << endl;
	return 0;
}

bool condition(const map<string, int>& map, const string& var,
			   const char* operand, const string& s_val) {
	const int val = stoi(s_val);
	const int ref = map.find(var)->second;

	switch (hash(operand)) {
		case hash("==") :
					return (ref == val) ? true : false;
		case hash("!=") :
					return (ref != val) ? true : false;
		case hash(">=") :
					return (ref >= val) ? true : false;
		case hash("<=") :
					return (ref <= val) ? true : false;
		case hash(">") :
					return (ref > val) ? true : false;
		case hash("<") :
					return (ref < val) ? true : false;
	}
	throw "no match for operand found!";
}
