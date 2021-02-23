#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>
using std::string;
using std::cout;
using std::endl;

#ifndef UNION_FIND_HPP_
#define UNION_FIND_HPP_

class QuickUnionUF
{
public:
	QuickUnionUF(int N);
	~QuickUnionUF();

	int root(unsigned int i);

	bool connected(int p, int q) { return root(p) == root(q); }

	void _union(int p, int q);

	void disable_element() { components_sets--; }
	int get_components_sets() { return components_sets; }
private:
	unsigned int* id;
	unsigned int* size;
	unsigned int components_sets;
};

int hex_positive(const string& hash);

void map_row(const string& hash, int* map);

std::vector<std::pair<int,int>> get_neighbours(int i, int j, int Nrows, int Ncols);

/* columns-within-rows translation from 2d to 1d description */
int translate_to_1D(int i, int j, int Nrows, int Ncols);

std::pair<int,int> translate_to_2D(int n, int Ncols);

#endif /* UNION_FIND_HPP_ */
