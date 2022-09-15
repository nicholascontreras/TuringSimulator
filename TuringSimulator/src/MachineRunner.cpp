#include "MachineRunner.h"

#include <iostream>
#include <conio.h>

#include "StringUtils.h"

using std::cout;
using std::endl;
using std::cin;

bool MachineRunner::VERIFY_BEFORE = false;
int MachineRunner::MAX_NUM_STEPS = 10000;

void MachineRunner::run_on_input(string machine_string, string input, bool step) {
	TuringMachine machine = TuringMachine::from_string(machine_string, input);

	if (VERIFY_BEFORE) {
		machine.verify_complete_from_current_state();
	}

	if (step) {
		cout << "Running in stepper mode, press any key to step\n" << endl;
	}

	for (int i = 0; i <= MAX_NUM_STEPS; i++) {
		cout << "Step: " << i << ": " << machine.to_string() << endl;
		if (machine.is_terminated()) {
			cout << "Machine reached termination state \"(" << machine.get_state().get_name() << ") " << machine.get_tape().get_normalized().to_string() << "\"" << endl;
			return;
		}
		if (step) {
			while (_kbhit()) _getch();
			_getch();
		}
		machine.step();
	}
	cout << "Machine execution exceeded the maximum number of allowed steps" << endl;
}

void MachineRunner::run_on_tests(string machine_string, vector<string> tests) {
	using strutil::trim;
	using strutil::extract_until;
	using strutil::skip_past;

	int num_tests = 0, num_failed = 0;
	
	for (string cur_test : tests) {
		string starting_tape = trim(extract_until(cur_test, "->"));
		string expected_ending_tape = trim(skip_past(cur_test, "->"));

		TuringMachine machine = TuringMachine::from_string(machine_string, starting_tape);

		if (VERIFY_BEFORE) {
			machine.verify_complete_from_current_state();
		}

		num_tests++;

		for (int i = 0; i <= MAX_NUM_STEPS; i++) {
			if (machine.is_terminated()) {
				if (machine.get_tape().get_normalized() == expected_ending_tape) {
					cout << "[PASSED] \"" << cur_test << "\"" << endl;
				} else {
					cout << "[FAILED] \"" << cur_test << "\"\n";
					cout << "    - actual result was \"(" << machine.get_state().get_name() << ") " << machine.get_tape().get_normalized().to_string() << "\"" << endl;
					num_failed++;
				}
				break;
			}
			machine.step();
		}

		if (!machine.is_terminated()) {
			cout << "[FAILED] \"" << cur_test << "\"\n";
			cout << "    - Machine execution exceeded the maximum number of allowed steps\n";
			cout << "    - ending result was \"(" << machine.get_state().get_name() << ") " << machine.get_tape().to_string() << "\"" << endl;
			num_failed++;
		}
	}

	cout << "\n";
	if (num_tests == 0) {
		cout << "No tests run";
	} else if (num_failed > 0) {
		cout << "Number of tests failed: (" << num_failed << "/" << num_tests << ")";
	} else {
		cout << "All tests passed (" << num_tests << ")";
	}
	cout << endl;
}
