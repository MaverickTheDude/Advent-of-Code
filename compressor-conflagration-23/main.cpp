#include "project.hpp"

int executions = 0;

int main() {
	std::ifstream myfile("/home/maverick/code/Advent-of-Code/compressor-conflagration-23/input.txt");
	std::ofstream outfile("/home/maverick/code/Advent-of-Code/compressor-conflagration-23/output.txt");
    if (!myfile || !outfile) {
        throw std::runtime_error("unexpected status with i/o\n"); }

	program prog0(myfile);
	prog0.print_commands(outfile);

	int current_line0 = 0;
	while (true) {
		const int next0 = prog0.execute_line(current_line0);
		current_line0 += next0;

		if (current_line0 < 0 || current_line0 > NCOMMANDS-1) break;
	}


	cout << "send exectuions by program: " << executions <<  endl;
	myfile.close();
	return 0;
}
