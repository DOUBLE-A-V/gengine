#pragma once
#include <string>
#include <iostream>
using namespace std;
class Logger {
public:
	static void log(string text, string prefix) {
		cout << "Log:" << "[" << prefix << "]" << " > " << text << endl;
	}
	static void error(string text, string prefix) {
		cout << "Error:" << "[" << prefix << "]" << " > " << text << endl;
	}
	static void warning(string text, string prefix) {
		cout << "Warning:" << "[" << prefix << "]" << " > " << text << endl;
	}
};