#include "knot-hash.hpp"

/* https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c */
std::string int_to_hex(int i)
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(2)
  	  	 << std::hex << i;
  return stream.str();
}

/* wlasciwosc static zapisana wewnatrz deklaracji klasy task*/
std::vector<int> task::calculate_lengths(int* tab_lengths, int Nelements) {
	std::vector<int> tmp_lengths;
	for (int i = 0; i< Nelements;i++)
		tmp_lengths.push_back(tab_lengths[i]);
	return tmp_lengths;
}

std::vector<int> task::calculate_lengths(const string& input) {
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

string task::dense_hash() const {
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

void task::knot() {
	for (int i = 0; i < Nelements; i++) {
		task_list.reverse(pos, lengths[i]);
		task_list.skip(pos, lengths[i]+skip);
		skip++;
		if (skip == TASK_CAPACITY)
			skip = 0;
	}
}

task::task(int* _tab_lengths, int _Nelements) : // problem 10, part A
	lengths(calculate_lengths(_tab_lengths, _Nelements)), Nelements(_Nelements) {
	for (int i = 0; i < 256; i++)
		task_list.push_back(i);
	pos = task_list.tab.begin();
}

task::task(const string& _input) : lengths(calculate_lengths(_input)),
		Nelements(_input.size() + ADDITIONAL_LENGTHS) {
	for (int i = 0; i < 256; i++)
		task_list.push_back(i);
	pos = task_list.tab.begin();
}

void task::printA() const
	{ cout << "check sum: " << task_list.tab[0]*task_list.tab[1] << endl; }
