#include "project.hpp"

int executions = 0;

int elideLoop(int line, reg_map& registers) {
// Criterion: lines  11 ... 14,16 ... 19 are executed consequently -> quick-jump to line 20
// otherwise: return line. Additionally: check if register f should be swithed to 0

	static int prevLine = 0;
	static bool prevCondition = false; // true: previous subcondition was met

	/* reset stacking subcriteria (induction base) */
	if (line == 11) {
		prevCondition = true;
		prevLine = line;
		return line;
	}
	/* stack positive subcriteria */
	if (line == prevLine+1 && prevCondition) { // current + previous conditions are met
		prevLine = line;
		if (line == 19) {
			prevCondition = false;
			if (registers['b'] % registers['d'] == 0) // aditional condition inside optimize loop
				registers['f'] = 0;
			return 20;
		}
		return line;
	}
	/* case 14 --> 16 */
	if (line == 16 && prevCondition && prevLine==14) {
		prevLine = line;
		return line;
	}
	/* cancel stacking */
	prevCondition = false;
	prevLine = line;
	return line;
}

int main() {
	std::ifstream myfile("/home/maverick/code/Advent-of-Code/compressor-conflagration-23/input.txt");
	std::ofstream outfile("/home/maverick/code/Advent-of-Code/compressor-conflagration-23/output.txt");
    if (!myfile || !outfile) {
        throw std::runtime_error("unexpected status with i/o\n"); }

	program prog0(myfile);
	prog0.print_commands(outfile);

	int current_line0 = 0;
	while (true) {

		if constexpr (caseB)
			current_line0 = elideLoop(current_line0, prog0.registers);

		const int next0 = prog0.execute_line(current_line0);
		current_line0 += next0;
		if (current_line0 < 0 || current_line0 > NCOMMANDS-1) break;
	}

	cout << "(A) send exectuions by program: " << executions << endl; // check caseB <-- false in project.hpp
	cout << "(B) register h = " << prog0.registers['h'] << endl;
	myfile.close();
	return 0;
}
