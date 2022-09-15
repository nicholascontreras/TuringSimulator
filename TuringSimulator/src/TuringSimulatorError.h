#pragma once

#include <stdexcept>
#include <string>

using std::runtime_error;
using std::string;

class TuringSimulatorError : public runtime_error {
public:
	inline TuringSimulatorError(string message) : runtime_error(message) {};
};
