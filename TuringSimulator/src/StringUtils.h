#pragma once

#include <string>
#include <stdexcept>

using std::string;

namespace strutil {
	inline string trim(const string& s) {
		string WHITESPACE = " \t\r\n";

		if (s.length() == 0) {
			return s;
		}

		size_t start_index = s.find_first_not_of(WHITESPACE);
		size_t end_index = s.find_last_not_of(WHITESPACE);
		return s.substr(start_index, end_index - start_index + 1);
	}

	inline string extract_until(const string source, const string target) {
		size_t index = source.find(target);
		if (index != string::npos) {
			return source.substr(0, index);
		} else {
			throw std::invalid_argument("No occurance of target in source!");
		}
	}

	inline string skip_past(const string source, const string target) {
		size_t index = source.find(target);
		if (index != string::npos) {
			return source.substr(index + target.length());
		} else {
			throw std::invalid_argument("No occurance of target in source!");
		}
	}

	inline vector<string> split(string source, const string& delim = "\n", const bool& trim = true, const bool& remove_empty = true) {
		vector<string> split_string;

		while (source.length() > 0) {
			size_t index = source.find(delim);
			if (index == string::npos) {
				index = source.length();
			}

			string cur_split = source.substr(0, index);
			if (trim) {
				cur_split = strutil::trim(cur_split);
			}
			if (!remove_empty || cur_split.length() > 0) {
				split_string.push_back(cur_split);
			}
			source.erase(0, index + delim.length());
		}

		return split_string;
	}
};