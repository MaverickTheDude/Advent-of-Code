#include "knot-hash.hpp"
#include "union-find.hpp"

#define task10 false


int main() {

#if task10
	int Nelements = 16;
	int lengths[16] = {31,2,85,1,80,109,35,63,98,255,0,13,105,254,128,33};
	task partA(lengths, Nelements);
	partA.knot();
	partA.printA();

	string input = "31,2,85,1,80,109,35,63,98,255,0,13,105,254,128,33";
	task partB(input);
	for (int i = 0; i < 64; i++)
		partB.knot();

	string x = partB.dense_hash();
	cout << "knot hash: " << x;
#else
	/* disk-defragmentation-14 a,b */
	const string input("nbysizxe");
	int used_squares = 0;
	int COLS = 128;
	int ROWS = COLS;
	int tab[ROWS][COLS];

	for (int i = 0; i < COLS; i++) {
		const string hash_in = input + "-" + std::to_string(i);

		task partC(hash_in);
		string hash_out = partC.knot_hash();
		map_row(hash_out, tab[i]);				 // tab for part B
		used_squares += hex_positive(hash_out);  // part A
	}

	// wydrukuj tablice uzywanych / nieuzywanych partycji
/*	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			cout << tab[i][j] << " ";
		}
		cout << endl;
	}*/

	/* tab ustawione (myslowo) domyslne tak, ze wszystkie wartosci sa rowne 0.
	 * Testujemy tylko te rozne od 0 */
	QuickUnionUF UF(ROWS*COLS);

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {

			/* current sector not used - no union (only decrease # of common components) */
			if (tab[i][j] == 0) {
				UF.disable_element();
				continue;
			}

			std::vector<std::pair<int,int>> nbrs = get_neighbours(i, j, ROWS, COLS);
			for (std::pair<int,int>& x : nbrs) {
				int p = x.first;
				int q = x.second;

				/* neighbour not used */
				if (tab[p][q] == 0)
					continue;

				int ij_1D = translate_to_1D(i, j, ROWS, COLS);
				int pq_1D = translate_to_1D(p, q, ROWS, COLS);
				UF._union(ij_1D, pq_1D);
			}
		}
	}

	cout << "used squares: " << used_squares 			 << endl;
	cout << "regions: "      << UF.get_components_sets() << endl;

#endif

	return 0;
}
