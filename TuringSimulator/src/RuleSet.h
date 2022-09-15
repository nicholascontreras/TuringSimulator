#pragma once

#include <string>
#include <unordered_set>
#include <optional>

#include "TransitionRule.h"
#include "MachineState.h"

using std::string;
using std::unordered_set;
using std::optional;

class RuleSet {
public:
	static const RuleSet from_string(const string& rule_set_string);
	const optional<TransitionRule> get_rule(const MachineState& state, const TapeValue& input) const;

private:
	const unordered_set<TransitionRule> rules;

	RuleSet(const unordered_set<TransitionRule> rules);
};