#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>
using std::string;
using std::cout;
using std::endl;

#ifndef KNOT_HASH_HPP_
#define KNOT_HASH_HPP_

const int ADDITIONAL_LENGTHS = 5;
const int TASK_CAPACITY = 256;

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

	void push_back(const T& elem) { tab.push_back(elem); }
};

/* https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c */
std::string int_to_hex(int i);


class task {
private:
	const std::vector<int> lengths;
	const int Nelements;
	cvector<int> task_list;
	int skip = 0;
	std::vector<int>::iterator pos;
public:

	static std::vector<int> calculate_lengths(int* tab_lengths, int Nelements);

	static std::vector<int> calculate_lengths(const string& input);

	void knot();

	string dense_hash() const;

	string knot_hash() {
		for (int i = 0; i < 64; i++)
			this->knot();

		return this->dense_hash();
	}

	task(int* _tab_lengths, int _Nelements);

	task(const string& _input);

	void printA() const;
};


#endif /* KNOT_HASH_HPP_ */
