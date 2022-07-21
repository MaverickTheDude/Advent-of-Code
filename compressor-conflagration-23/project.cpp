#include "project.hpp"

// extern std::regex wzorzec;

static std::regex wzorzec("([a-z]+) ([a-z1-9]) ?(-?[a-z0-9]+)?");

argument::argument(std::string in_str) {

	/* argument doesn't exist */
	if (in_str.size() == 0) {
		value = 0;
		reg   = '0';
		state = _none_;
        std::cout << "case no arg " << std::endl;
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


int instruction::execute(reg_map *const ptr) {
	long long int new_val;
	switch (function) {
		case set :
			arg1.set(arg2.get_value(ptr), ptr);
			break;
		case sub :
			new_val = arg1.get_value(ptr) - arg2.get_value(ptr);
			arg1.set(new_val, ptr);
			break;
		case mul :
			new_val = arg1.get_value(ptr) * arg2.get_value(ptr);
			arg1.set(new_val, ptr);
            ++executions;
			break;
		case jnz :
			if (arg1.get_value(ptr) != 0)
				return arg2.get_value(ptr);
			break;
		default : 
            std::string msg = "execute: unresolved case " + std::to_string(function) + '\n';
            throw std::runtime_error(msg);
	}
	return 1;
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
		default : throw std::runtime_error("execute comm: unresolved case'\n");
	}
	return 1;
}


instruction::instruction(std::smatch arglist)
	: arg1(argument(arglist[2])), arg2(argument(arglist[3])) {
	std::string fun_str = arglist[1];
	const char* fun_chr = fun_str.c_str();
	switch (hash(fun_chr)) {
		case hash("snd") : function = snd;	break;
		case hash("set") : function = set;	break;
		case hash("add") : function = add;	break;
		case hash("sub") : function = sub;	break;
		case hash("mul") : function = mul;	break;
		case hash("mod") : function = mod;	break;
		case hash("rcv") : function = rcv;	break;
		case hash("jgz") : function = jgz;	break;
		case hash("jnz") : function = jnz;	break;
		default : throw std::runtime_error("intruction(): undefined case'\n");
	}
}

int instruction::print(int id, int line, reg_map *const reg_ptr) const {
	std::string fun;
	switch (function) {
		case snd : fun = "snd";	break;
		case set : fun = "set";	break;
		case add : fun = "add";	break;
		case sub : fun = "sub";	break;
		case mul : fun = "mul";	break;
		case mod : fun = "mod";	break;
		case rcv : fun = "rcv";	break;
		case jgz : fun = "jgz";	break;
		case jnz : fun = "jnz";	break;
		default  : throw std::runtime_error("print: error'\n");
	}
	cout << "id/line: " << id << "/" << line <<
			" function: " << fun << " " << arg1 << arg2;
	return arg1.get_value(reg_ptr);
}


program::program(std::ifstream& file, int _id_) : id(_id_), co_q_ptr(nullptr) {
	std::string line;
	std::smatch wynik;
	while ( getline(file, line) ) {
		std::regex_search(line, wynik, wzorzec);
		commands.emplace_back(wynik);
	}
	registers['a'] = 0;
}

std::ostream& operator<<(std::ostream& os, const argument& obj)  {
	switch (obj.state) {
		case _none_ 	: os << "\t[x] ";							break;
		case _value_ 	: os << "\tvalue: "	   << obj.value << " ";	break;
		case _register_ : os << "\tregister: " << obj.reg   << " ";	break;
	}
	return os;
}


