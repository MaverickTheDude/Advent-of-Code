#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
#include <string>
using std::cout;
using std::endl;

// switch - case dla const char*
/*https://hbfs.wordpress.com/2017/01/10/strings-in-c-switchcase-statements/*/
uint64_t constexpr mix(char m, uint64_t s) {return ((s<<7) + ~(s>>3)) + ~m;      }
uint64_t constexpr hash(const char * m)    {return (*m) ? mix(*m,hash(m+1)) : 0; }
enum commands {snd, set, add, mul, mod, rcv, jgz};
enum states {_none_, _value_, _register_};

static std::map<char, long long int> registers;
static int frequency = 0;

void add_to_map (char reg);


class argument {
friend std::ostream& operator<<(std::ostream& os, const argument& obj);
public:
	argument(std::string in_str);

	argument() : value(0), reg('0'), state(_none_) {
		cout << "unused, right?" << endl; }  // note: tego konstruktora dotyczy obserwacja ponizej

	long long int get_value() const {
		if (state == _none_) throw "this argument is empty!";
		return state == _value_ ? value : registers[reg];
	}

	void set(long long int new_value) {
		if (state == _none_)  throw "this argument is empty!";
		if (state == _value_) throw "there is no register to set!";
		registers[reg] = new_value;
	}

private:
	long long int value;
	char reg;
	int state;
};
std::ostream& operator<<(std::ostream& os, const argument& obj);

class instruction {
public:
	instruction(std::smatch arglist);

	// note: copy - constructor powinien przyjmowac argument const& obj
	instruction(const instruction& ins) : function(ins.function), arg1(ins.arg1), arg2(ins.arg2) {}

	void print() const  {cout << "function id: " << function << arg1 << arg2 << endl; }
	void print(std::ofstream& file) const
		{file << "function id: " << function << arg1 << arg2 << endl; }

	int execute(bool* continue_);

private:
	int function;
	argument arg1;
	argument arg2;
};

int main() {
	std::string line;
	std::ifstream myfile("src/input.txt");
	std::ofstream outfile("src/output.txt");
	std::smatch wynik;
	std::regex wzorzec("([a-z]+) ([a-z1-9]) ?(-?[a-z0-9]+)?");

	std::vector<instruction> commands;

	while ( getline(myfile, line) ) {
		std::regex_search(line, wynik, wzorzec);
		commands.emplace_back(wynik);
	}

	const int coms_size = commands.size();
	bool work = true;
	int current_line = 0;
	while (work) {
		current_line += commands[current_line].execute(&work);

		// debug statements:
/*		for (auto& x : registers)
			cout << x.first << " " << x.second << '\t';
		cout << endl << endl;*/

		if (current_line < 0 || current_line > coms_size)
			break;
	}


	for (auto &x : commands)
		x.print(outfile);

	cout << "last frequency: " << frequency << endl;
	return 0;
}


argument::argument(std::string in_str) {

	/* argument doesn't exist */
	if (in_str.size() == 0) {
		value = 0;
		reg   = '0';
		state = _none_;
		return;
	}

	reg   = *in_str.c_str();
	bool number_is_first = (0 <= reg - '0' && reg - '0' <= 9);

	/* argument is a value */
	if (in_str.size() > 1 || number_is_first) {
		value = stoi(in_str);
		state = _value_;
		reg   = '0';
		return;
	}
	/* argument is a register */
	add_to_map(reg);
	value = 0;
	state = _register_;
}

int instruction::execute(bool* continue_) {
	long long int new_val;
	switch (function) {
		case snd :
			frequency = arg1.get_value();
			break;
		case set :
			arg1.set(arg2.get_value());
			break;
		case add :
			new_val = arg1.get_value() + arg2.get_value();
			arg1.set(new_val);
			break;
		case mul :
			new_val = arg1.get_value() * arg2.get_value();
			arg1.set(new_val);
			break;
		case mod :
			new_val = arg1.get_value() % arg2.get_value();
			arg1.set(new_val);
			break;
		case rcv :
			if (arg1.get_value() > 0)
				*continue_ = false;
			break;
		case jgz :
			if (arg1.get_value() > 0)
				return arg2.get_value();
			break;
		default : throw "execute: unresolved case";
	}
	return 1;
}

/* obserwacja: konstruktor defaultowy argument() jest wywolywany jezeli arg1/arg2 nie sa
 * podane w liscie inicjalizacyjnej. Dlaczego musi byc wywolywany w przeciwnym wypadku
 * tzn. kiedy arg1/arg2 wywolujemy wewnatrz konstrukotra instruction(...) ?
 * https://en.cppreference.com/w/cpp/language/constructor (ale odp. nie znalazlem) */

instruction::instruction(std::smatch arglist)
	: arg1(argument(arglist[2])), arg2(argument(arglist[3])) {
	std::string fun_str = arglist[1];
	const char* fun_chr = fun_str.c_str();
	switch (hash(fun_chr)) {
		case hash("snd") : function = snd;	break;
		case hash("set") : function = set;	break;
		case hash("add") : function = add;	break;
		case hash("mul") : function = mul;	break;
		case hash("mod") : function = mod;	break;
		case hash("rcv") : function = rcv;	break;
		case hash("jgz") : function = jgz;	break;
		default : throw "wrong function operand!";
	}

//	arg1 = argument(arglist[2]);
//	arg2 = argument(arglist[3]);
}

void add_to_map (char reg) {
	std::map<char, long long int>::const_iterator it;
	it = registers.find(reg);
	if (it == registers.end())
		registers[reg] = 0;
}

std::ostream& operator<<(std::ostream& os, const argument& obj)  {
	switch (obj.state) {
		case _none_ 	: os << "\t[x] ";							break;
		case _value_ 	: os << "\tvalue: "	   << obj.value << " ";	break;
		case _register_ : os << "\tregister: " << obj.reg   << " ";	break;
	}
	return os;
}
