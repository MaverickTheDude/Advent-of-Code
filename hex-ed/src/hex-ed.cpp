#include <iostream>
#include <fstream>

using std::cout;	using std::string;
using std::endl;

enum direction { n = 1, ne, se, s, sw, nw };

int parse(std::ifstream& input);

/* https://www.redblobgames.com/grids/hexagons/ */
class point_cube {
public:

	void move(int dir);
	int distance() const;
	int max_distance() const { return max_dist; }

private:
	int max_dist = 0;
	int x = 0;
	int y = 0;
	int z = 0;
};

int main() {
	std::ifstream myfile("input.txt");
	point_cube point;

	while (true) {
		const int dir = parse(myfile);
		if (!dir)
			break;
		point.move(dir);
	}

	cout << "Manhattan distance = " << point.distance() << endl;
	cout << "Maximum distance = "   << point.max_distance() << endl;

	return 0;
}

void point_cube::move(int dir) {
	switch (dir) {
		case n :
			++y; --z;	break;
		case s :
			--y; ++z;	break;
		case ne :
			++x; --z;	break;
		case sw :
			--x; ++z;	break;
		case nw :
			--x; ++y;	break;
		case se :
			++x; --y;	break;
		default :
			throw "point_cube::move(): niespodziewany case";
	}
	int current_dist = this->distance();
	max_dist = (max_dist > current_dist) ? max_dist : current_dist;
}

int point_cube::distance() const {
	int max = abs(x);
	max = max > abs(y) ? max : abs(y);
	max = max > abs(z) ? max : abs(z);
	return max;
}

/* zalozenie: zawsze zaczynamy po przecinku i ustawiamy kursor za nim
 * note: dopisalismy przecinek do input.txt */
int parse(std::ifstream& input) {
	char parser;
//	bool stop = (input >> parser); czemu tak nie?
	if ( !(input >> parser) )
		return 0;

	bool north = (parser == 'n') ? true : false;

	input >> parser;
	switch (parser)	{
		case ',' :
			return north ? n : s;
		case 'e' :
			input >> parser;
			return north ? ne : se;
		case 'w' :
			input >> parser;
			return north ? nw : sw;
	}
	throw "parse(): niespodziewany case";
}
