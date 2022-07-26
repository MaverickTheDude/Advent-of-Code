#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <unordered_set>
#include <math.h>
using std::cout;
using std::endl;

int maxBridgeStrength = 0;
unsigned int maxBridgeLength = 0;
std::vector<int> strongestBridge;
const int Ncomponents = 57;

using componentType = std::pair<int,int>;
using setOfComponentsType = std::unordered_set<componentType>;
setOfComponentsType findMatchingComponents(int element, const setOfComponentsType& components);
int computeBridgeStrength(std::vector<int> bridge);

template<>
struct std::hash<componentType> {
	size_t operator() (const componentType& obj) const {
		const double mag = sqrt(obj.first*obj.first + obj.second*obj.second);
		const double arg = obj.second / (obj.first + 1);
		return std::hash<double>() (obj.first*mag + obj.second*arg); // Still, I don't get this syntax
	}
};


/* bridge stores only one element of the component pair, e.g.: [0, 45, 13, 1] means [0-45, 45-13, 13-1] */
void connect(std::vector<int> bridge, setOfComponentsType componentsLeft) {
	int lastComp = bridge.back();
	setOfComponentsType possibleMatches = findMatchingComponents(lastComp, componentsLeft);
	for (const componentType& x : possibleMatches) {
		/* Make copy of component list and brdged elements and move element x from set to vector */
		setOfComponentsType possibleMatchesCopy;
		possibleMatchesCopy = componentsLeft;
		std::vector<int> bridgeCopy;
		bridgeCopy = bridge;
		
		possibleMatchesCopy.erase(x);
		const int nextComp = (x.second == lastComp ? x.first : x.second);
		bridgeCopy.push_back(nextComp);

		connect(bridgeCopy, possibleMatchesCopy);
	}
	int strenght = computeBridgeStrength(bridge);
	bool longerBridge = (bridge.size() > maxBridgeLength);
	bool strongerBridge = (bridge.size() == maxBridgeLength && strenght > maxBridgeStrength);
	if (longerBridge || strongerBridge) {
		maxBridgeLength = bridge.size();
		maxBridgeStrength = strenght;
		strongestBridge = bridge;
		std::cout << "assigning new winner with strenght " << strenght << " and #elements: " << bridge.size() << endl;
	}
}

int main() {
	std::ifstream myfile("/home/maverick/code/Advent-of-Code/electromagnetic-moat-24/input.txt");
	if (!myfile) {
        throw std::runtime_error("unexpected status with i/o\n"); }

	setOfComponentsType componentsAv; // available components
	componentsAv.reserve(Ncomponents);
	std::regex wzorzec("(\\d+)\\/(\\d+)");
	std::string line;
	std::smatch wynik;
	while ( getline(myfile, line) ) {
		std::regex_search(line, wynik, wzorzec);
		int a = std::stoi(wynik[1]);
		int b = std::stoi(wynik[2]);
		componentsAv.emplace(a,b);
		/* print hash of each entry: */
		/* if (componentsAv.find(std::make_pair(a,b)) != componentsAv.end()) {
			setOfComponentsType::iterator it = componentsAv.find(std::make_pair(a,b));
			std::cout << it->first << "/" << it->second << "\t";
			std::cout << componentsAv.hash_function() (*it) << '\n';
		}
		else
			cout << "not found" << endl; */
	}
	myfile.close();

	std::vector<int> bridge;
	bridge.reserve(Ncomponents);
	bridge.push_back(0);
	connect(bridge, componentsAv);

	cout << "max strenght = " << maxBridgeStrength << std::endl;
	return 0;
}



setOfComponentsType findMatchingComponents(int element, const setOfComponentsType& components) {
	setOfComponentsType componentsMatched;
	for (const componentType& x : components) {
		if (x.first == element || x.second == element)
			componentsMatched.insert(x);
	}
	return componentsMatched;
}

int computeBridgeStrength(std::vector<int> bridge) {
	int strenght = 0;
	for (std::vector<int>::iterator it = bridge.begin(); it < bridge.end(); ++it)
		strenght += 2 * (*it);
	strenght -= bridge.back();
	return strenght;
}