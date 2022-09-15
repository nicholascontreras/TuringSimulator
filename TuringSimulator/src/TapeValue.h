#pragma once

#include <stdexcept>

using std::logic_error;
using std::invalid_argument;

class TapeValue {
public:
	enum Value {
		BLANK, FALSE, TRUE, NUM_VALUES
	};

	inline TapeValue(Value v) : value(v) {};

	// Constructor that takes in a char to allow implicit parsing from their assocated symbols
	inline TapeValue(char c) {
		switch (c) {
		case '_':
			value = BLANK;
			break;
		case '0':
			value = FALSE;
			break;
		case '1':
			value = TRUE;
			break;
		default:
			throw invalid_argument("Invalid TapeValue char!");
		}
	}

	// Constructor taking an integer to allow accessing the enum values by index
	inline TapeValue(int i) {
		if (i >= 0 && i < NUM_VALUES) {
			value = Value(i);
		} else {
			throw invalid_argument("Invalid TapeValue index!");
		}
	}

	inline explicit operator char() const {
		switch (value) {
		case BLANK:
			return '_';
		case FALSE:
			return '0';
		case TRUE:
			return '1';
		default:
			throw logic_error("Invalid TapeValue value!");
		}
	};

	inline operator int() const {
		return value;
	}

	inline TapeValue operator++(int) { value = (Value)(value + 1); return value; };
private:
	Value value;
};