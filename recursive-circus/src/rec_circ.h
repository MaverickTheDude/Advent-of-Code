#ifndef REC_CIRC_H_
#define REC_CIRC_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>
#include <map>
#include <cassert>
using std::cout;
using std::endl;

std::unordered_set<std::string> split(const std::string& s);
class program {
	friend struct tier;
public:
	std::unordered_set<std::pair<std::string, const program*>> names() const  ;
	program(const std::string& line);
	program(const program&);
	program(const program& obj, const program* parent);
	std::string name;
	bool operator==(const program& obj) const;
private:
	int weight;
	std::unordered_set<std::string> children;
	const program* parent;
};

template<>
struct std::hash<program> {
	size_t operator() (const program& obj) const {
		return std::hash<std::string>() (obj.name); // nie rozumiem tej skladni
	}
};

template<>
struct std::hash<std::pair<std::string, const program*>> {
	size_t operator() (const std::pair<std::string, const program*>& obj) const {
		return std::hash<std::string>() (obj.first);
	}
};

using set_pair = std::unordered_set<std::pair<std::string, const program*>>;
struct tier {
	std::unordered_set<program> programs;
	set_pair child_names() const;

	//					   parent                weight N_wystapien
	using map = std::map<const program*, std::pair<int, int>>;
	map finalize() const;
	map finalize(const map& upper_map) const;

	tier(const std::string& line) { programs.emplace(line);	}
	tier() {}
};

#endif /* REC_CIRC_H_ */
