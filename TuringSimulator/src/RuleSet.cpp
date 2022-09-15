#include "RuleSet.h"

#include "StringUtils.h"
#include "TuringSimulatorError.h"

using strutil::trim;
using strutil::split;

using std::nullopt;

const RuleSet RuleSet::from_string(const string& rule_set_string) {
	vector<string> split_rule_strings = split(trim(rule_set_string));
	unordered_set<TransitionRule> rules;
	
	for (const string& cur_rule_string : split_rule_strings) {
		TransitionRule cur_rule = TransitionRule::from_string(cur_rule_string);
		if (rules.find(cur_rule) == rules.end()) {
			rules.insert(cur_rule);
		} else {
			throw TuringSimulatorError("Multipule rule defintions for state \"" + cur_rule.get_existing_state().get_name() + "\" and input '" + (char) cur_rule.get_input_value() + "'");
		}
	}
	return RuleSet(rules);
}

const optional<TransitionRule> RuleSet::get_rule(const MachineState& state, const TapeValue& input) const {
	for (TransitionRule cur_rule : rules) {
		if (cur_rule.is_rule_match(state, input)) {
			return optional(cur_rule);
		}
	}
	return nullopt;
}

RuleSet::RuleSet(const unordered_set<TransitionRule> rules) : rules(rules) {
}
