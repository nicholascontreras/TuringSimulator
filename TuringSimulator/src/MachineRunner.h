#pragma once

#include <vector>

#include "TuringMachine.h"
#include "StringUtils.h"

using std::vector;

using strutil::trim;

class MachineRunner {

public:
	static bool VERIFY_BEFORE;
	static int MAX_NUM_STEPS;

	static void run_on_input(string machine_string, string input, bool step);
	static void run_on_tests(string machine_string, vector<string> tests);
};

