#include <iostream>
#include <fstream>
using std::cout;	//using std::map;
using std::endl;	using std::string;

/*Argument moze przyjac dwa stany: open curly brace (T) vs closed curly brace (F)*/
int open(bool openCB) {
	static int counter = 0;
	return (openCB) ? counter++ : counter--;
}

int main() {
	char pointer;
	std::ifstream myfile("input.txt");
	int score = 0;
	int litter = 0;
	bool garbage = false;

	while (myfile >> pointer) {
		if (garbage) {
			switch (pointer) {
				case '>' :
					garbage = false;
					break;
				case '!' :
					myfile >> pointer;
					break;
				default :
					litter++;
			}
		}
		else {
			switch (pointer) {
				case '<' :
					garbage = true;
					break;
				case '{' :
					open(true);
					break;
				case '}' :
					score += open(false);
					break;
			}
		}
	}

	cout << "score = " << score << endl;
	cout << "garbage elements = " << litter << endl;

	return 0;
}
