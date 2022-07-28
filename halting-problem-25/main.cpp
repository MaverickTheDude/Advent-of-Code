#include <iostream>
#include <map>
#include <vector>
using std::cout;
using std::endl;
enum states {A = 0, B, C, D ,E, F};
enum directions {left = -1, right = 1};
const long long int Nsteps = 12208951;

class instructionSet {
	friend class state;
public:
	instructionSet() = delete;
	instructionSet(const instructionSet&) = default;
	instructionSet(int activeState, int newValue, int move, int nextState)
		: m_activeState(activeState), m_newValue(newValue), m_move(move), m_nextState(nextState) {	}

private:
	int execute(int& cursor, std::map<int,int>& tape) {
		tape[cursor] = m_newValue;
		cursor += m_move;
		return m_nextState;
	}

	int m_activeState;
	int m_newValue;
	int m_move;
	int m_nextState;
};

class state {
public:
	state(int newValue0, int move0, int nextState0,
		  int newValue1, int move1, int nextState1)
		  : instr0(0, newValue0, move0, nextState0),
		    instr1(1, newValue1, move1, nextState1) { }

	int execute(int& cursor, std::map<int,int>& tape) {
		int nextState;
		if (tape[cursor] == 1)
			nextState = instr1.execute(cursor, tape);
		else
			nextState = instr0.execute(cursor, tape);
		
		std::map<int,int>::iterator it = tape.find(cursor);
		if (it == tape.end()) /* cursor not found in tape */
			tape[cursor] = 0;
		
		return nextState;
	}

private:
	instructionSet instr0;
	instructionSet instr1;
};

int main() {
	std::vector<state> states;
	states.reserve(6);
	states.emplace_back(1, right, B, 0, left , E);
	states.emplace_back(1, left , C, 0, right, A);
	states.emplace_back(1, left , D, 0, right, C);
	states.emplace_back(1, left , E, 0, left , F);
	states.emplace_back(1, left , A, 1, left , C);
	states.emplace_back(1, left , E, 1, right, A);

	std::map<int,int> tape; /* position / value: 0 or 1 */
	tape[0] = 0;
	int nextState = A;
	int cursor = 0;

	for (int i = 0; i < Nsteps; ++i) {
		nextState = states[nextState].execute(cursor, tape);
	}

	int cnt = 0;
	for (std::map<int,int>::iterator it = tape.begin(); it != tape.end(); ++it) {
		if (it->second == 1)
			++cnt;
	}

	cout << "diagnostic checksum, i.e. sum of 1's on tape: " << cnt << std::endl;
	return 0;
}