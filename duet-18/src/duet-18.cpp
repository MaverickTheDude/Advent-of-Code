#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
#include <queue>
#include <unistd.h>
using std::cout;
using std::endl;

// switch - case dla const char*
/*https://hbfs.wordpress.com/2017/01/10/strings-in-c-switchcase-statements/*/
uint64_t constexpr mix(char m, uint64_t s) {return ((s<<7) + ~(s>>3)) + ~m;      }
uint64_t constexpr hash(const char * m)    {return (*m) ? mix(*m,hash(m+1)) : 0; }
enum commands {snd, set, add, mul, mod, rcv, jgz};
enum states {_none_, _value_, _register_};

static int executions = 0;
const int NCOMMANDS = 41; // hardcoded
static std::regex wzorzec("([a-z]+) ([a-z1-9]) ?(-?[a-z0-9]+)?");
using reg_map = std::map<char, long long int>;
using reg_queue = std::queue<long long int>;

class argument {
friend std::ostream& operator<<(std::ostream& os, const argument& obj);
public:
	argument(std::string in_str);

	argument() : value(0), reg('0'), state(_none_) {
		cout << "unused, right?" << endl; }  // note: tego konstruktora dotyczy obserwacja ponizej

	long long int get_value(reg_map *const reg_ptr) const {
		if (state == _none_) throw "this argument is empty!";
		return state == _value_ ? value : (*reg_ptr)[reg];
	}

	void set(long long int new_value, reg_map *const reg_ptr) {
		if (state == _none_)  throw "this argument is empty!";
		if (state == _value_) throw "there is no register to set!";
		(*reg_ptr)[reg] = new_value;
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

	int print(int id, int line, reg_map *const reg_ptr) const;

	void print(std::ofstream& file) const
		{file << "function id: " << function << arg1 << arg2 << endl; }

	int execute(reg_queue& ours, reg_queue *const theirs, reg_map *const ptr, const int&);

private:
	int function;
	argument arg1;
	argument arg2;
};

class program {
public:
	program(std::ifstream& file, int _id_ = 0);

	int execute_line(int line) {
		return commands[line].execute(queue, co_q_ptr, &registers, id);
	}

	void print_line(int line) {
		long long int x = commands[line].print(id, line, &registers);
		cout << "reg. value: " << x << endl;
	}

	void print_commands(std::ofstream& outfile) const {
		for (auto &x : commands)
			x.print(outfile);
	}

	void connect(program& program) {
		co_q_ptr = &program.queue;
		program.co_q_ptr = &this->queue;
	}

private:
	void add_to_map(char);

	int id;
	std::map<char, long long int> registers;
	std::vector<instruction> commands;
	std::queue<long long int> queue;
	std::queue<long long int>* co_q_ptr;
};

int main() {
	std::ifstream myfile("src/input.txt");
	std::ofstream outfile("src/output.txt");

	program prog0(myfile);
	myfile.clear();				 // re-read input file
	myfile.seekg(0, myfile.beg);
	program prog1(myfile, 1);
	prog0.connect(prog1);

	int current_line0 = 0;
	int current_line1 = 0;
	while (true) {

		const int next0 = prog0.execute_line(current_line0);
		const int next1 = prog1.execute_line(current_line1);
		current_line0 += next0;
		current_line1 += next1;

		if (next0 == 0 && next1 == 0)	/* deadlock */		break;
		if (current_line0 < 0 || current_line0 > NCOMMANDS) break;
		if (current_line1 < 0 || current_line1 > NCOMMANDS) break;
	}

	prog1.print_commands(outfile);

	cout << "send exectuions by program #1: " << executions <<  endl;
	myfile.close();
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
	value = 0;
	state = _register_;
}

int instruction::execute(reg_queue& ours, reg_queue *const theirs, reg_map *const ptr, const int& id) {
	long long int new_val;
	switch (function) {
		case snd :
			/* wysylamy do kolejki partnera */
			theirs->push( arg1.get_value(ptr) );
			if (id == 1)	++executions;
			break;
		case rcv :
			/* pobieramy ze swojej kolejki */
			if (ours.size() > 0) {
				arg1.set(ours.front(), ptr);
				ours.pop();
				return 1;
			}
			else return 0;
		case set :
			arg1.set(arg2.get_value(ptr), ptr);
			break;
		case add :
			new_val = arg1.get_value(ptr) + arg2.get_value(ptr);
			arg1.set(new_val, ptr);
			break;
		case mul :
			new_val = arg1.get_value(ptr) * arg2.get_value(ptr);
			arg1.set(new_val, ptr);
			break;
		case mod :
			new_val = arg1.get_value(ptr) % arg2.get_value(ptr);
			arg1.set(new_val, ptr);
			break;
		case jgz :
			if (arg1.get_value(ptr) > 0)
				return arg2.get_value(ptr);
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

int instruction::print(int id, int line, reg_map *const reg_ptr) const {
	std::string fun;
	switch (function) {
		case snd : fun = "snd";	break;
		case set : fun = "set";	break;
		case add : fun = "add";	break;
		case mul : fun = "mul";	break;
		case mod : fun = "mod";	break;
		case rcv : fun = "rcv";	break;
		case jgz : fun = "jgz";	break;
		default  : throw "error";
	}
	cout << "id/line: " << id << "/" << line <<
			" function: " << fun << " " << arg1 << arg2;
	return arg1.get_value(reg_ptr);
}

void program::add_to_map (char reg) {
	reg_map::const_iterator it;
	it = registers.find(reg);
	if (it == registers.end())
		registers[reg] = 0;
}

program::program(std::ifstream& file, int _id_) : id(_id_), co_q_ptr(nullptr) {
	std::string line;
	std::smatch wynik;
	while ( getline(file, line) ) {
		std::regex_search(line, wynik, wzorzec);
		commands.emplace_back(wynik);
	}
	registers['p'] = _id_;
}

std::ostream& operator<<(std::ostream& os, const argument& obj)  {
	switch (obj.state) {
		case _none_ 	: os << "\t[x] ";							break;
		case _value_ 	: os << "\tvalue: "	   << obj.value << " ";	break;
		case _register_ : os << "\tregister: " << obj.reg   << " ";	break;
	}
	return os;
}


