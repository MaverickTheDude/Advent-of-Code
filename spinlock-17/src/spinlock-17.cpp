#include <iostream>
#include <list>
#include <time.h>
using std::string;
using std::cout;
using std::endl;

const int steps = 376;
const int NinsertionsA = 2017;
const int NinsertionsB = 50000000;

class interface {
	virtual void push_next(int val) = 0;
	virtual void move()             = 0;
	virtual void skip(int)          = 0;
	virtual void print() const      = 0;
public:
	virtual ~interface() {}
};

class clist : private interface {
public:
	clist();
	void push_next(int val);
	void move();
	void skip(int steps) { for (int i = 0; i < steps; i++)  move(); }
	void print() const;
	int get_next()  const { return *next(it, 1); }
	int get_first() const { return *next(list.begin(), 1); }
public:
	std::list<int> list;
	std::list<int>::iterator it;
};

class cphantom : private interface {
public:
	cphantom() : current(0), cap(1), first(0) {}
	void move() { current = (current == cap-1) ? 0 : current+1; }
	void skip(int steps) { for (int i = 0; i < steps; i++)  move(); }
	void push_next(int val);
	void print() const { cout << "pierwszy element: " << first << ". Kursor: "
							  << current     << "/"   << cap   << endl;}
private:
	int current, cap, first;
};

int main() {
	clock_t start, end;
	clist spinlock;
	cphantom spinlock_p;

	for (int i = 0; i < NinsertionsA; i++) {
		spinlock.skip(steps);
		spinlock.push_next(i+1);
	}
	cout << "partA. nastepny element: " << spinlock.get_next()  << endl;

	start = clock();

	for (int i = 0; i < NinsertionsB; i++) {
		spinlock_p.skip(steps);
		spinlock_p.push_next(i+1);
	}
	end = clock();
	cout << "part B: " << endl;
	spinlock_p.print();

	cout << "czas pracy: " << double(end - start) / double(CLOCKS_PER_SEC);

	return 0;
}

void cphantom::push_next(int val) {
	++cap;
	if (current == 0)
		first = val;
	++current; // move when false
}


void clist::push_next(int val) {
	list.insert(std::next(it,1), val);
	++it;
}
clist::clist() {
	list.push_back(0);
	it = list.begin();
}
void clist::move() {
	if (std::next(it,1) == list.end())
		it = list.begin();
	else
		++it;
}

void clist::print() const {
	for (std::list<int>::const_iterator i = list.begin(); i != list.end(); i++) {
		if (i == it)
			cout << "(" << *i << ")" << "\t";
		else
			cout << *i << "\t";
		if (std::distance(list.begin(), i) % 10 == 0)
			cout << endl;
	}
	cout << endl;
}
