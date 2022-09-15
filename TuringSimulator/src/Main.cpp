#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <stdlib.h>
#include <cstring>

#include "MachineRunner.h"
#include "TuringSimulatorError.h"
#include "StringUtils.h"

using std::cout;
using std::endl;

using std::ifstream;
using std::getline;

using std::transform;

using std::invalid_argument;

void show_help() {
	cout << "Command line help for Turing Machine Simulator:\n";
	cout << "Usage:\n";
	cout << "    - Single Run Mode: <MACHINE> -r <INPUT> [OPTIONS]\n";
	cout << "    - Stepper Mode:    <MACHINE> -s <INPUT> [OPTIONS]\n";
	cout << "    - Test Suite Mode: <MACHINE> -t <TESTS> [OPTIONS]\n";
	cout << "\nAvaliable Options:\n";
	cout << "    -v                Verifies that the machine is both complete and able to terminate before simulating\n";
	cout << "    -l <NUM_STEPS>    Sets the maximum number of computation steps before raising an error (default: 10,000)\n";
	cout << endl;
}

string read_entire_file(const string file) {
	ifstream input_file(file);
	
	if (!input_file.is_open()) {
		throw invalid_argument("Unable to open the given file!");
	}

	string cur_line;
	string file_contents;
	while (getline(input_file, cur_line)) {
		file_contents += cur_line;
		file_contents.push_back('\n');
	}	
	input_file.close();

	return file_contents;
}

bool parse_options(char* options[], int num_options) {
	for (int i = 0; i < num_options; i++) {
		if (strcmp(options[i], "-v") == 0) {
			MachineRunner::VERIFY_BEFORE = true;
		} else if (strcmp(options[i], "-l") == 0) {
			if (i + 1 < num_options) {
				MachineRunner::MAX_NUM_STEPS = atoi(options[i + 1]);
				i++;
			} else {
				cout << "Option \"-l\" requires <NUM_STEPS>\n";
				return false;
			}
		} else {
			cout << "Unrecognized option \"" << options[i] << "\"\n";
			return false;
		}
	}

	return true;
}

bool run(int argc, char* argv[]) {
	if (argc < 4) {
		show_help();
		return false;
	} else {
		string machine_string = read_entire_file(argv[1]);

		string mode_string = argv[2];
		transform(mode_string.begin(), mode_string.end(), mode_string.begin(), tolower);

		string input_string(argv[3]);

		if (mode_string.length() != 2 || mode_string.at(0) != '-') {
			// Invalid mode format
			show_help();
			return false;
		}

		if (argc > 4) {
			if (!parse_options(&argv[4], argc - 4)) {
				cout << "Use -h or --help to show help info" << endl;
				return false;
			}
		}

		switch (mode_string.at(1)) {
		case 'r':
			MachineRunner::run_on_input(machine_string, input_string, false);
			break;
		case 's':
			MachineRunner::run_on_input(machine_string, input_string, true);
			break;
		case 't':
		{
			string test_cases_string = read_entire_file(input_string);
			vector<string> test_cases = strutil::split(test_cases_string);
			MachineRunner::run_on_tests(machine_string, test_cases);
		}
		break;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	try {
		return run(argc, argv) ? 0 : 1;
	} catch (TuringSimulatorError e) {
		cout << "Simulator Error\n    - " << e.what() << endl;
		return 2;
	}
}