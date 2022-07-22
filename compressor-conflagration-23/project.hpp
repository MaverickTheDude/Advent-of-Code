#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
#include <queue>
#include <unistd.h>
using std::cout;
using std::endl;

constexpr bool caseB = true; // false for case A
constexpr int INITIAL_REG_A = caseB ? 1 : 0;

// switch - case dla const char*
/*https://hbfs.wordpress.com/2017/01/10/strings-in-c-switchcase-statements/*/
uint64_t constexpr mix(char m, uint64_t s) {return ((s<<7) + ~(s>>3)) + ~m;      }
uint64_t constexpr hash(const char * m)    {return (*m) ? mix(*m,hash(m+1)) : 0; }
enum commands {set=1, add, mul, mod, snd, rcv, jgz, jnz, sub};
enum states {_none_, _value_, _register_};

const int NCOMMANDS = 32; // hardcoded
extern int executions;
using reg_map = std::map<char, long long int>;
using reg_queue = std::queue<long long int>;

// int elideLoop(int line, program& prog);

class argument {
friend std::ostream& operator<<(std::ostream& os, const argument& obj);
public:
	argument(std::string in_str);
	argument() = delete;

	long long int get_value(reg_map *const reg_ptr) const {
		if (state == _none_) throw std::runtime_error("argument::get_value(): state is none'\n");
		return state == _value_ ? value : (*reg_ptr)[reg];
	}

	void set(long long int new_value, reg_map *const reg_ptr) {
		if (state == _none_)  throw std::runtime_error("argument::set(): this argument is empty!\n");
		if (state == _value_) throw std::runtime_error("argument::set(): there is no register to set!\n");
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
	instruction() = delete;

	// note: copy - constructor powinien przyjmowac argument const& obj
	instruction(const instruction& ins) : function(ins.function), arg1(ins.arg1), arg2(ins.arg2) {}

	int print(int id, int line, reg_map *const reg_ptr) const;

	void print(std::ofstream& file) const
		{file << "function id: " << function << arg1 << arg2 << endl; }

	int execute(reg_queue& ours, reg_queue *const theirs, reg_map *const ptr, const int&);
	int execute(reg_map *const ptr);

private:
	int function;
	argument arg1;
	argument arg2;
};

class program {
	friend int main();
public:
	program(std::ifstream& file, int _id_ = 0);

	int execute_line(int line) {
        if (co_q_ptr == nullptr)
            return commands[line].execute(&registers);
		else
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

	int id;
	std::map<char, long long int> registers;
	std::vector<instruction> commands;
	std::queue<long long int> queue;
	std::queue<long long int>* co_q_ptr;
};