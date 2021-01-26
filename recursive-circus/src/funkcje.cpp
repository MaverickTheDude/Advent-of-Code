#include "rec_circ.h"


set_pair program::names() const {
	set_pair set;
	for (auto x : children) {
		std::pair<std::string, const program*> tmp;// = {x, this}; (inna skladnia)
		const program* that = this;
		tmp = make_pair(x, that);
		set.insert(tmp);
	}
	return set;
}

program::program(const std::string& line) : parent(nullptr) {
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

program::program(const program& obj) :
		name(obj.name), weight(obj.weight), children(obj.children), parent(obj.parent)	{ }

program::program(const program& obj, const program* parent) :
		name(obj.name), weight(obj.weight), children(obj.children), parent(parent)	{ }

bool program::operator==(const program& obj) const {
	if (this->name.compare(obj.name) == 0)
		return true;
	else return false;
}

std::unordered_set<std::string> split(const std::string& s) {
	std::unordered_set<std::string> tokens;

	std::string::const_iterator from = s.begin();
	for (std::string::const_iterator it = s.begin(); it != s.end(); it++) {
		if ( *it == ',' ) {
			tokens.emplace(from, it);
//std::copy(from, it, back_inserter(tokens)); // error: from, it wskazuja na char, back_inserter spodziewa sie string
// dodatkowy blad: to podejscie da vector<string> = {s, h, a v, ...} zamiast {shavjjt, ...}
			from = it + 2;
		}
		if (it == s.end()-1)
			tokens.emplace(from, it+1);
	}
	return tokens;
}


