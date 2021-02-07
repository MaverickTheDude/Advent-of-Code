#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using std::cout;	using std::string;
using std::endl;


template <class T>
class cvector {
public: /*custom iterators: https://www.youtube.com/watch?v=F9eDv-YIOQ0*/
	std::vector<T> tab;
public:
	void reverse(const typename std::vector<T>::iterator& it, const int& length) {
		if (it+length <= tab.end())
			std::reverse(it, it+length);  		// iterator + int [ok] dla random access iterators
		else {
			/* 1 2 3) 4 5 6 7 ([8] 9 10 11 */
			/*  (m)            [it] (n)    */
			/* -----length      *--------- */
			int n = std::distance(it, tab.end());
			int m = length - n;
			typename std::vector<T> tmp(it, std::next(it,n)); // std::next
			tmp.resize(length);
			std::copy(tab.begin(), next(tab.begin(),m), next(tmp.begin(), n));
			std::reverse(tmp.begin(), tmp.end());
			//copy from tmp to tab
			std::copy(tmp.begin()+n, tmp.end(), tab.begin());
			std::copy(tmp.begin(), next(tmp.begin(),n), it);
		}
	}

	void skip(typename std::vector<T>::iterator& it, const int& length) {
		if (it+length < tab.end())
			it = std::next(it,length);
		else {
			int n = std::distance(it, tab.end());
			int m = length - n;
			it = tab.begin() + m;
		}
	}

	void push_back(const T& elem) {
		tab.push_back(elem);
	}

};


int main() {
	int lengths[16] = {31,2,85,1,80,109,35,63,98,255,0,13,105,254,128,33};
	cvector<int> the_list;
	int Nelements = 16;
	int skip = 0;
	for (int i = 0; i < 256; i++)
		the_list.push_back(i);

	std::vector<int>::iterator it = the_list.tab.begin();
	for (int i = 0; i < Nelements; i++) {
		the_list.reverse(it, lengths[i]);
		the_list.skip(it, lengths[i]+skip);
		skip++;
	}

	for (auto x : the_list.tab)
		cout << x << " ";

	cout << endl << "wynik = " << the_list.tab[0]*the_list.tab[1] << endl;
	return 0;
}
