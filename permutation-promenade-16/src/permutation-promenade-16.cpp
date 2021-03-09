#include <iostream>
#include <fstream>
#include <vector>
using std::string;
using std::cout;
using std::endl;

const int Nprograms = 16;
const int Ndances   = 1; // partA: Ndances = 1
enum commands {spin, exchange, partner};

string parse(std::ifstream& file, bool* stop);
bool repeated(int* tab);
class command {
public:
	command(string& str);
	void execute(int* tab) const;
	void print() const;
private:
	int par1, par2;
	int com;
};


int main() {
	bool stop = false;
	std::ifstream myfile("input.txt");
	std::vector<command> commands;

	int* tab = new int[Nprograms];
	for (int i = 0; i < Nprograms; i++) tab[i] = i;

	while (true) {
		string tmp_com = parse(myfile, &stop);
		if (stop)
			break;
		commands.emplace_back(tmp_com);
	}
	// part A
	for (unsigned int j = 0; j < commands.size(); j++)
		commands[j].execute(tab);

	for (int i = 0; i < Nprograms; i++) cout << tab[i] << " ";
	cout << endl << "programs (part A): " << endl;

	for (int i = 0; i < Nprograms; i++)
		cout << (char)(tab[i]+'a');
	cout << endl;

	// part B
	cout << endl;
	int reps = 1;
	while (true) {
		for (unsigned int j = 0; j < commands.size(); j++)
			commands[j].execute(tab);

		++reps;
		if (repeated(tab))
			break;
	}

	int reszta = 1000000000 % reps;
	cout << "powtorka po: " << reps << " iteracjach / tancach" << endl;
	cout << "Pozostala liczba tancow do wykonania: " << reszta << endl;

	for (int i = 0; i < reszta; i++)
		for (unsigned int j = 0; j < commands.size(); j++)
			commands[j].execute(tab);

	cout << endl << "programs (part B): " << endl;
	for (int i = 0; i < Nprograms; i++)
		cout << (char)(tab[i]+'a');
	cout << endl;

	delete [] tab;
	return 0;
}


string parse(std::ifstream& file, bool* stop) {
	char next;
	string command;
	if (!(file >> next)) {
		*stop = true;
		return command;
	}

	while (next != ',') {
		command += next;
		file >> next;
	}
	return command;
}

void command::execute(int* tab) const {
	int j, n, m, tmp;
	int * tmp_container = new int[par1];
	switch (com) {
		case spin :

			n = par1;
			m = Nprograms - n;
			j = 0;
			/* [0] [1] | [2] [3] [4] [5]
			 * ---m---- --------n------ */
			/* kopia ostatnich n elementow do tmp_container */
			for (int i = 0; i < n; i++)
				tmp_container[j++] = tab[m+i];

			/* kopia pierwszych m elementow do przodu tablicy (reverse) */
			j = Nprograms-1;
			for (int i = m-1; i > -1; i--)
				tab[j--] = tab[i];

			/* kopia z tmp_container na pierwsze n elementow tablicy */
			for (int i = 0; i < n; i++)
				tab[i] = tmp_container[i];

			break;
		case exchange :
			tmp 	  = tab[par1];
			tab[par1] = tab[par2];
			tab[par2] = tmp;
			break;
		case partner :
			for (int i = 0; i < Nprograms; i++) {
				if (tab[i] == par1) n = i;
				if (tab[i] == par2) m = i;
			}
			tmp = tab[n];
			tab[n] = tab[m];
			tab[m] = tmp;
	}
	delete [] tmp_container;
}


command::command(string& str) {
	switch (str[0]) {
		case 's' : com = spin; 		break;
		case 'x' : com = exchange; 	break;
		case 'p' : com = partner; 	break;
		default  : throw "wrong command";
	}

	string parameters(str.begin()+1, str.end());
	if (com == spin) {
		par1 = stoi(parameters);
		return;
	}

	if (com == partner) {
		par1 = str[1] - 'a';
		par2 = str[3] - 'a';
		return;
	}

	/* com == exchange */
	std::size_t delim = parameters.find('/');
	string s1(parameters.begin(), parameters.begin() + delim);
	string s2(parameters.begin() + delim+1, parameters.end());
	par1 = stoi(s1);
	par2 = stoi(s2);
}

void command::print() const {
	cout << "command id: " << com << " parameter 1 = " << par1 << " parameter 2 = " << par2 << endl; }

/* sprawdza, czy wrocilismy do pozycji wyjsciowej: 0,1,2...,15 */
bool repeated(int* tab) {
	for (int i = 0; i < Nprograms; i++)
		if (tab[i] != i)
			return false;
	return true;
}
