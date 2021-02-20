#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
using std::cout;
using std::endl;

const int INITIAL_POS = 1;
const int NLAYERS = 95;

class layer {
public:
	int move_cnt = 0;
	layer(int _depth, int _range) : depth(_depth), range(_range), pos(INITIAL_POS) { }
	layer(const layer& obj) :
		move_cnt(obj.move_cnt), depth(obj.depth), range(obj.range),
		pos(obj.pos), slope_down(obj.slope_down) { }
	void move() {
		if (slope_down) {
			if (pos < range)
				pos++;
			else {
				slope_down = false;
				pos--;
			}
		}
		else {
			if (pos > INITIAL_POS)
				pos--;
			else {
				slope_down = true;
				pos++;
			}
		}
		move_cnt++;
	}

	int check() const {
		return (pos == INITIAL_POS) ? depth*range : 0; }

	bool caught() const {
			return (pos == INITIAL_POS) ? true : false; }

	int get_depth() { return depth; }

	void reset() { pos = INITIAL_POS; slope_down = true; move_cnt = 0; }


	void print(std::ostream& os) const {
		os << "layer " << depth << " pos: " << pos << " / " << range << endl;
	}

private:
	const int depth;
	const int range;
	int pos;
	bool slope_down = true;
};

std::ostream& operator<<(std::ostream& os, const layer& obj)  {
	obj.print(os);
	return os;
}

int calc_severity(std::vector<layer>& firewall);
bool test(std::vector<layer> firewall);


int main() {
	std::string line;
	std::ifstream myfile("input.txt");
	std::smatch wynik;
	std::regex wzorzec("([0-9]+): ([0-9]+)");
	std::vector<layer> firewall;

	while ( getline(myfile, line) ) {
		std::regex_search(line, wynik, wzorzec);
		int depth = stoi(wynik[1]);
		int range = stoi(wynik[2]);
		firewall.emplace_back(depth, range);
	}

	// part A
	int severity = calc_severity(firewall);
	cout << "severity = " << severity << endl;

	for (auto& x : firewall) x.reset();

	// part B
	int delay = 0;
	while (true) {
		for (auto& x : firewall) {
			x.move();
			x.move(); // tylko parzyste maja sens ze wzgledu na layer[1]
		}

		bool caught = test(firewall); // 3903378
		delay += 2;

		if (caught==0)
			break;
	}

	cout << "delay = " << firewall[0].move_cnt;
	return 0;
}


bool test(std::vector<layer> firewall) {
	int mypos = 0;

	while (mypos < NLAYERS) {
		for (auto& x : firewall)
			if ( mypos == x.get_depth() )   // czy warstwa ma skaner?
				if ( x.caught() )           // czy skaner nas zalapal?
					return true;		    // caught!

		for (auto& x : firewall)
			x.move();
		mypos++;
	}
	return false; // escaped!
}

int calc_severity(std::vector<layer>& firewall) {
	int mypos = 0;
	int severity = 0;

	while (mypos < NLAYERS) {
		for (auto& x : firewall)
			if (mypos == x.get_depth())
				severity += x.check();

		for (auto& x : firewall)
			x.move();
		mypos++;
	}
	return severity;
}

