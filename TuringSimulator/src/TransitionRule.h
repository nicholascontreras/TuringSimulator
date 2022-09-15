#pragma once

#include <string>
#include "MachineState.h"
#include "InfiniteTape.h"

using std::string;
using std::hash;

class TransitionRule {
public:
	static const TransitionRule from_string(const string rule_string);

	bool is_rule_match(const MachineState state, const TapeValue input) const;

	inline MachineState get_existing_state() const { return existing_state; };
	inline TapeValue get_input_value() const { return input_value; };

	inline MachineState get_new_state() const { return new_state; };
	inline TapeValue get_output_value() const { return output_value; };
	inline bool get_direction() const { return direction; };

	bool operator==(const TransitionRule& other) const;

	friend hash<TransitionRule>;

private:
	const MachineState existing_state;
	const TapeValue input_value;

	const MachineState new_state;
	const TapeValue output_value;
	const bool direction;

	TransitionRule(const MachineState existing_state, const TapeValue input_value, const MachineState new_state, const TapeValue output_value, const bool direciton);

	static bool parse_direction(const string direction_string);
};

template<> struct hash<TransitionRule> {
	size_t operator()(TransitionRule const& rule) const noexcept;
};