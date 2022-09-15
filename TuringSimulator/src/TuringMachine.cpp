#include "TuringMachine.h"

#include "StringUtils.h"
#include "RuleSet.h"
#include "TuringSimulatorError.h"
#include "TapeValue.h"

using strutil::trim;
using strutil::extract_until;
using strutil::skip_past;
using strutil::split;

TuringMachine TuringMachine::from_string(const string& machine_string, const string& starting_tape_string) {
	string trimmed_machine_string = trim(machine_string);
	trimmed_machine_string = trim(skip_past(trimmed_machine_string, "rules:"));

	string rules_string = trim(extract_until(trimmed_machine_string, "starting state:"));
	trimmed_machine_string = trim(skip_past(trimmed_machine_string, "starting state:"));

	string starting_state_string = trim(extract_until(trimmed_machine_string, "terminating states:"));
	trimmed_machine_string = trim(skip_past(trimmed_machine_string, "terminating states:"));

	string terminating_states_string = trimmed_machine_string;

	const MachineState starting_state(starting_state_string);

	const RuleSet rules = RuleSet::from_string(rules_string);
	const unordered_set<MachineState> terminating_states = parse_terminating_states(terminating_states_string);

	return TuringMachine(rules, InfiniteTape(starting_tape_string), starting_state, terminating_states);
}

void TuringMachine::verify_complete_from_current_state() const {
	unordered_set<MachineState> cur_states;
	unordered_set<MachineState> processed_states;
	cur_states.insert(state);
	processed_states.insert(state);

	while (cur_states.size() > 0) {
		unordered_set<MachineState> next_states;

		for (const MachineState& cur_state : cur_states) {
			for (TapeValue cur_value = 0; cur_value < TapeValue::NUM_VALUES; cur_value++) {
				optional<TransitionRule> opt_rule = rules.get_rule(cur_state, cur_value);

				if (opt_rule.has_value()) {
					TransitionRule rule = opt_rule.value();
					MachineState new_state = rule.get_new_state();
					if (processed_states.find(new_state) == processed_states.end()) {
						if (terminating_states.find(new_state) == terminating_states.end()) {
							next_states.insert(new_state);
						}
						processed_states.insert(new_state);
					}
				} else {
					throw TuringSimulatorError("No transition rule for state \"" + cur_state.get_name() + "\" and input '" + (char) cur_value + "'");
				}
			}
		}

		cur_states.clear();
		cur_states.insert(next_states.begin(), next_states.end());
	}

	bool reached_terminating = false;
	for (const MachineState& cur_terminating_state : terminating_states) {
		if (processed_states.find(cur_terminating_state) != processed_states.end()) {
			return;
		}
	}

	throw TuringSimulatorError("No transition rule path to a terminating state, machine will never terminate");
}

void TuringMachine::step() {
	optional<TransitionRule> opt_rule = rules.get_rule(state, tape.get_value());
	if (opt_rule.has_value()) {
		TransitionRule rule = opt_rule.value();
		tape.set_value(rule.get_output_value());
		state = rule.get_new_state();
		tape.shift_index(rule.get_direction());
	} else {
		throw TuringSimulatorError("No transition rule for state \"" + state.get_name() + "\" and input '" + (char) tape.get_value() + "'");
	}
}

bool TuringMachine::is_terminated() const {
	return terminating_states.find(state) != terminating_states.end();
}

string TuringMachine::to_string() const {
	string machine_string = state.get_name() + "\n" + tape.to_string() + "\n";
	machine_string += string(tape.get_index(), ' ') + "^";
	return machine_string;
}

TuringMachine::TuringMachine(const RuleSet rules, InfiniteTape tape, const MachineState starting_state, const unordered_set<MachineState> terminating_states) :
	rules(rules), 
	tape(tape),
	state(starting_state),
	terminating_states(terminating_states) {
}

const unordered_set<MachineState> TuringMachine::parse_terminating_states(const string& terminating_states_string) {
	vector<string> split_terminating_states_string = split(trim(terminating_states_string), ",");
	unordered_set<MachineState> terminating_states;

	for (const string& cur_terminating_state_string : split_terminating_states_string) {
		MachineState cur_terminating_state = MachineState(cur_terminating_state_string);

		if (terminating_states.find(cur_terminating_state) == terminating_states.end()) {
			terminating_states.insert(cur_terminating_state);
		} else {
			throw TuringSimulatorError("Duplicate terminating state \"" + cur_terminating_state.get_name() + "\"");
		}
	}

	return terminating_states;
}
