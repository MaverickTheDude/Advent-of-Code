#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>

using std::cout;
using std::endl;

std::vector<std::string> split(const std::string& s);
class program {
public:
	std::vector<std::string> names() const;
	program(const std::string& line);
private:
	std::string name;
	int weight;
	std::vector<std::string> children;
};

int main() {
	using set_iter = std::unordered_set<std::string>::const_iterator;
	std::string line;
	std::ifstream myfile("input.txt");
	std::unordered_set<std::string> set;

	while ( getline(myfile, line) ) {
		program p(line);
		std::vector<std::string> v = p.names();
		for (auto x : v) {
			set_iter hit = set.find(x);
			if (hit == set.end())
				set.insert(x);
			else
				set.erase(hit);
		}
	}

	for (auto x : set)
		cout << x << endl;

	return 0;
}

std::vector<std::string> program::names() const {
	std::vector<std::string> vec;
	vec.push_back(name);
	for (auto x : children)
		vec.push_back(x);
	return vec;
}

program::program(const std::string& line) {
	std::smatch wynik;
	std::regex wzorzec("([a-z]+) \\(([0-9]+)\\) -> ([a-z,\\s]+)");
	/*   np.             kozpul       (59)      -> shavjjt, anujsv, tnzvo*/
	std::regex_search(line, wynik, wzorzec);
	if (wynik.size()==0) { // note: jezeli wszystkie capture segmenty nie zlapia, to mamy zero matchy
		wzorzec = "([a-z]+) \\(([0-9]+)\\)";
		std::regex_search(line, wynik, wzorzec);
	}
	else {
		const std::string& allChildren = wynik[3].str();
		children = split(allChildren);
	}

	name = wynik[1].str();
	weight = std::stoi( wynik[2].str() );
}


std::vector<std::string> split(const std::string& s) {
	std::vector<std::string> tokens;

	std::string::const_iterator from = s.begin();
	for (std::string::const_iterator it = s.begin(); it < s.end(); it++) {
		if ( *it == ',' ) {
			tokens.emplace_back(from, it);
//std::copy(from, it, back_inserter(tokens)); // error: from, it wskazuja na char, back_inserter spodziewa sie string
// dodatkowy blad: to podejscie da vector<string> = {s, h, a v, ...} zamiast {shavjjt, ...}
			from = it + 2;
		}
		if (it == s.end()-1)
			tokens.emplace_back(from, it+1);
	}
	return tokens;
}

/* (c) https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/ */
// fail: delimiter moze byc tylko jednym znakiem...
/*std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}*/
