#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>

using std::cout;	using std::string;
using std::endl;

#define taskA false
const int ADDITIONAL_LENGTHS = 5;
const int TASK_CAPACITY = 256;

/* https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c */
std::string int_to_hex(int i)
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(2)
  	  	 << std::hex << i;
  return stream.str();
}

template <class T>
class cvector {
friend class task;
private: /*custom iterators: https://www.youtube.com/watch?v=F9eDv-YIOQ0*/
	std::vector<T> tab;
public:
	void reverse(const typename std::vector<T>::iterator& it, const int& length) {
		assert(length < TASK_CAPACITY);
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

	void skip(typename std::vector<T>::iterator& it, int length) {
		if (length > TASK_CAPACITY)
			length -= TASK_CAPACITY;
		if (it+length < tab.end())
			it = std::next(it,length);
		else {
			int n = std::distance(it, tab.end());
			int m = length - n;
			it = tab.begin() + m;
		}
	}

	const T& operator[](int it) const  { return tab[it]; }

	void push_back(const T& elem) {
		tab.push_back(elem);
	}
};

class task {
private:
	const std::vector<int> lengths;
	const int Nelements;
	cvector<int> task_list;
	int skip = 0;
	std::vector<int>::iterator pos;
public:

	static std::vector<int> calculate_lengths(int* tab_lengths, int Nelements) {
		std::vector<int> tmp_lengths;
		for (int i = 0; i< Nelements;i++)
			tmp_lengths.push_back(tab_lengths[i]);
		return tmp_lengths;
	}

	static std::vector<int> calculate_lengths(const string& input) {
		std::vector<int> tmp_lengths;
		for (char x : input)
			tmp_lengths.push_back(static_cast<int>(x));
		tmp_lengths.push_back(17);
		tmp_lengths.push_back(31);
		tmp_lengths.push_back(73);
		tmp_lengths.push_back(47);
		tmp_lengths.push_back(23);
		return tmp_lengths;
	}

	string dense_hash() const {
		string tmp_dh;
		for (int i = 0; i < 16; i++) {
			const int begin = 16 * i;
			int tmp = task_list[begin];

			for (int j = 1; j < 16; j++)
				tmp = tmp ^ task_list[begin+j];

			tmp_dh.append(int_to_hex(tmp));
		}
		return tmp_dh;
	}

	void knot() {
		for (int i = 0; i < Nelements; i++) {
			task_list.reverse(pos, lengths[i]);
			task_list.skip(pos, lengths[i]+skip);
			skip++;
			if (skip == TASK_CAPACITY)
				skip = 0;
		}
	}

	task(int* _tab_lengths, int _Nelements) :
		lengths(calculate_lengths(_tab_lengths, _Nelements)), Nelements(_Nelements) {
		for (int i = 0; i < 256; i++)
			task_list.push_back(i);
		pos = task_list.tab.begin();
	}

	task(const string& _input) : lengths(calculate_lengths(_input)),
			Nelements(_input.size() + ADDITIONAL_LENGTHS) {
		for (int i = 0; i < 256; i++)
			task_list.push_back(i);
		pos = task_list.tab.begin();
	}

	void printA() const {
		cout << "check sum: " << task_list.tab[0]*task_list.tab[1] << endl; }
};


int main() {

#if taskA
	int Nelements = 16;
	int lengths[16] = {31,2,85,1,80,109,35,63,98,255,0,13,105,254,128,33};
	task partA(lengths, Nelements);
	partA.knot();
	partA.printA();
#else
	string input = "31,2,85,1,80,109,35,63,98,255,0,13,105,254,128,33";
	task partB(input);
	for (int i = 0; i < 64; i++)
		partB.knot();

	string x = partB.dense_hash();
	cout << "knot hash: " << x;
#endif

	return 0;
}
