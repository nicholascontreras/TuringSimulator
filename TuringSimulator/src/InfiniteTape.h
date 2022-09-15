#pragma once

#include <string>
#include <vector>

#include "TapeValue.h"

using std::string;
using std::vector;

class InfiniteTape {
public:
	InfiniteTape(string bit_string);
	inline InfiniteTape(const vector<TapeValue>& tape) : tape(tape), index(0) {};

	inline TapeValue get_value() const { return tape.at(index);  };
	inline void set_value(TapeValue new_value) { tape.at(index) = new_value; };

	InfiniteTape get_normalized() const;

	void shift_index(bool direction);
	inline int get_index() const { return index; };

	bool operator==(const InfiniteTape& other) const;

	string to_string() const;
private:
	vector<TapeValue> tape;
	int index;
};

