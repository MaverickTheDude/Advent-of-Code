#include <iostream>
#include <vector>
#include <fstream>
using std::cout;
using std::endl;
using std::string;
using std::pair;

string solution = "";
enum directions {N, S, E, W};
void move(int dir, pair<int, int>& obj);
int counter(bool add);


class tubes {
public:
	tubes(std::vector<string> _map_, pair<int, int> _coords_) :
		map(_map_), coords(_coords_) {	}

	bool step() {
		char current = map[coords.first][coords.second];
		while (current != '+') {
			if (current == ' ')						// warunek stopu
				return false;
			if (current >= 'A' && current <= 'Z') 	// dodaj litere
				solution += current;
			move(direction, coords);
			current = map[coords.first][coords.second];
		}
		return true;
	}

	void turn();

	void traverse() {
		while ( step() ) {
			turn();
		}
	}

	void print() {
		cout << "i = " << coords.first << " j = " << coords.second << endl;
	}

private:
	int direction = S; // hardcoded
	std::vector<string> map;
	pair<int, int> coords;
};

int main() {
	// initialize data
	std::vector<string> field;
	field.reserve(200);
	std::string line;
	std::ifstream file("input.txt");
	while ( getline(file, line) ) {
		field.emplace_back(line);
	}

	pair<int, int> coords = {0, 0};
	for (char t : field[0]) {
		if (t == '|')
			break;
		++coords.second;
	}

	// calculate stuff on object
	tubes tubeA(field, coords);
	tubeA.traverse();

	cout << solution << endl;
	cout << counter(false) << endl;
	return 0;
}



void tubes::turn() {
	bool motionWasVertical = (direction == S || direction == N);
	if (motionWasVertical) { 	// testujemy prawo-lewo
		char left  = map[coords.first][coords.second-1];
		char right = map[coords.first][coords.second+1];
		if (left == right)
			throw "mapa niezgodna z zalozeniami!";
		if (left == '-')
			direction = W;
		else
			direction = E;
	}
	else {						// testujemy gora-dol
		char up   = map[coords.first-1][coords.second];
		char down = map[coords.first+1][coords.second];
		if (up == down)
			throw "mapa niezgodna z zalozeniami!";
		if (up == '|')
			direction = N;
		else
			direction = S;
	}
	move(direction, coords);
}

void move(int dir, pair<int, int>& obj) {
	switch (dir) {
		case N :
			--obj.first;
			break;
		case S :
			++obj.first;
			break;
		case E :
			++obj.second;
			break;
		case W :
			--obj.second;
	}
	counter(true);
}

int counter(bool add) {
	static int counter = 0;
	if (add) ++counter; // else: read only
	return counter;
}
