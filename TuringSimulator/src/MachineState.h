#pragma once

#include <string>

using std::string;
using std::hash;

class MachineState {
public:
	MachineState(string name) : name(name) {};
	inline string get_name() const { return name; };
	inline bool operator==(const MachineState& other) const { return name == other.name; };

	friend hash<MachineState>;
private:
	string name;
};

template<> struct hash<MachineState> {
	size_t operator()(MachineState const& state) const noexcept;
};