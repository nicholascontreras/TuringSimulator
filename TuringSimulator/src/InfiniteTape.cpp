#include "InfiniteTape.h"

#include <algorithm>
#include <stdexcept>

using std::invalid_argument;

InfiniteTape::InfiniteTape(string bit_string) : index(0) {
	for (char cur_bit : bit_string) {
		tape.push_back(cur_bit);
	}
}

InfiniteTape InfiniteTape::get_normalized() const {
	vector<TapeValue> normalized = tape;
	if (index > 0) {
		normalized.erase(normalized.begin(), normalized.begin() + index);
	}
	return InfiniteTape(normalized);
}

void InfiniteTape::shift_index(bool direction) {
	if (direction) {
		index++;
		if (index == tape.size()) {
			tape.push_back(TapeValue::BLANK);
		}
	} else {
		if (index > 0) {
			if (index == tape.size() - 1) {
				if (tape.at(tape.size() - 1) == TapeValue::BLANK) {
					// Remove trailing blank value
					tape.pop_back();
				}
			}

			index--;
		}
	}
}

bool InfiniteTape::operator==(const InfiniteTape& other) const {
	if (index > 0) {
		// All tape comparisons must be normalized
		return other == this->get_normalized();
	}
	
	if (other.tape.size() - other.index > tape.size() - index) {
		// If the provided tape is longer than us, reverse the equality check
		return other == *this;
	}

	for (int i = 0; i < tape.size(); i++) {
		if (i < other.tape.size()) {
			// If in the range of the other tape, ensure indentical values
			if (tape.at(i) != other.tape.at(i)) {
				return false;
			}
		} else {
			// Otherwise ensure any extra tape length we have is blank
			if (tape.at(i) != TapeValue::BLANK) {
				return false;
			}
		}
	}

	return true;
}

string InfiniteTape::to_string() const {
	string contents_as_string;
	for (TapeValue cur_value : tape) {
		contents_as_string.push_back((char) cur_value);
	}
	return contents_as_string + "...";
}
