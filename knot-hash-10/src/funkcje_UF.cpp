#include "union-find.hpp"
#include <bitset>

QuickUnionUF::QuickUnionUF(int N) : components_sets(N) {
	id   = new unsigned int[N];
	size = new unsigned int[N];
	for (int i = 0; i < N; i++)
		id[i] = i;
	for (int i = 0; i < N; i++)
		size[i] = 1;
}

QuickUnionUF::~QuickUnionUF() {
	delete [] id;
	delete [] size;
}

int QuickUnionUF::root(unsigned int i) {
	while (i != id[i]) {
		id[i] = id[id[i]];
		i = id[i];
	}
	return i;
}

void QuickUnionUF::_union(int p, int q) {
		int i = root(p);
		int j = root(q);

		if (i == j) return;

		if (size[i] < size[j]) {
			id[i] = j;
			size[j] += size[i];
		}
		else {
			id[j] = i;
			size[i] += size[j];
		}
		components_sets--;
	}

int hex_positive(const string& hash) {
	int occurs = 0;
	for (char x : hash) {
		int intval = (x >= 'A') ? (x - 'a' + 10) : (x - '0');
		std::string bin = std::bitset<4>(intval).to_string();
		occurs += std::count(bin.begin(), bin.end(), '1');
	}

	return occurs;
}

void map_row(const string& hash, int* map) {
	int col = 0;
	for (char x : hash) {
		int intval = (x >= 'A') ? (x - 'a' + 10) : (x - '0');
		std::string bin = std::bitset<4>(intval).to_string();
		for (int i = 0; i < 4; i++) {
			map[col] = bin[i] - '0';
			col++;
		}
	}
}

std::vector<std::pair<int,int>> get_neighbours(int i, int j, int Nrows, int Ncols) {
	std::vector<std::pair<int,int>> tmp;
	/* dodajemy do zbioru tylko "na prawo" i "w dol" (lazy) */
//	if (i != 0) 	   tmp.push_back(std::make_pair(i-1, j));
	if (i != Nrows-1)  tmp.push_back(std::make_pair(i+1, j));
//	if (j != 0) 	   tmp.push_back(std::make_pair(i  , j-1));
	if (j != Ncols-1)  tmp.push_back(std::make_pair(i  , j+1));
	return tmp;
}

/* columns-within-rows translation from 2d to 1d description */
int translate_to_1D(int i, int j, int Nrows, int Ncols)
	{ return Ncols * i + j;}

std::pair<int,int> translate_to_2D(int n, int Ncols) {
	int j = n % Ncols;
	int i = (n - j) / Ncols;
	return std::make_pair(i,j);
}
