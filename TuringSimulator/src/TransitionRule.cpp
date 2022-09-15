#include "TransitionRule.h"

#include <stdexcept>

#include "StringUtils.h"

using std::invalid_argument;

using strutil::trim;
using strutil::extract_until;
using strutil::skip_past;

const TransitionRule TransitionRule::from_string(const string rule_string) {
	string trimmed_rule_string = trim(rule_string);

	string existing_state_string = trim(extract_until(trimmed_rule_string, " "));
	trimmed_rule_string = trim(skip_past(trimmed_rule_string, " "));

	string input_value_string = trim(extract_until(trimmed_rule_string, "->"));
	trimmed_rule_string = trim(skip_past(trimmed_rule_string, "->"));

	string new_state_string = trim(extract_until(trimmed_rule_string, " "));
	trimmed_rule_string = trim(skip_past(trimmed_rule_string, " "));

	string output_value_string = trim(extract_until(trimmed_rule_string, " "));
	trimmed_rule_string = trim(skip_past(trimmed_rule_string, " "));

	string direction_string = trimmed_rule_string;

	MachineState existing_state = existing_state_string;
	TapeValue input_value = input_value_string.at(0);
	MachineState new_state = new_state_string;
	TapeValue output_value = output_value_string.at(0);
	bool direction = parse_direction(direction_string);

	return TransitionRule(existing_state, input_value, new_state, output_value, direction);
}

TransitionRule::TransitionRule(const MachineState existing_state, const TapeValue input_value, const MachineState new_state, const TapeValue output_value, const bool direction) :
	existing_state(existing_state),
	input_value(input_value),
	new_state(new_state),
	output_value(output_value),
	direction(direction) {
}

bool TransitionRule::parse_direction(const string direction_string) {
	if (direction_string == "L") {
		return false;
	} else if (direction_string == "R") {
		return true;
	} else {
		throw invalid_argument("Invalid direction!");
	}
}

bool TransitionRule::is_rule_match(const MachineState state, const TapeValue input) const {
	return state == existing_state && input == input_value;
}

bool TransitionRule::operator==(const TransitionRule& other) const {
	return is_rule_match(other.existing_state, other.input_value);
}

size_t hash<TransitionRule>::operator()(TransitionRule const& rule) const noexcept {
	return hash<MachineState>{}(rule.existing_state);
}
