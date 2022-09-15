#pragma once

#include <unordered_set>
#include <string>

#include "RuleSet.h"
#include "InfiniteTape.h"
#include "MachineState.h"

using std::unordered_set;
using std::string;

class TuringMachine {
public:
	static TuringMachine from_string(const string& machine_string, const string& starting_tape_string);

	inline InfiniteTape get_tape() const { return tape;  };
	inline MachineState get_state() const { return state;  } ;

	void verify_complete_from_current_state() const;

	void step();
	bool is_terminated() const;

	string to_string() const;
private:
	const RuleSet rules;
	const unordered_set<MachineState> terminating_states;

	InfiniteTape tape;
	MachineState state;

	TuringMachine(const RuleSet rules, InfiniteTape tape, const MachineState starting_state, const unordered_set<MachineState> terminating_states);

	static const unordered_set<MachineState> parse_terminating_states(const string& terminating_states_string);
};