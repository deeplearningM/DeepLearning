#ifndef QLEARNING_FILEREADER_H
#define QLEARNING_FILEREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using std::string;
using std::cout;
using std::endl;
using std::vector;

int lineNumber = 0;
const string filename = string("constants.txt");

class FileReader {
public:
	FileReader() { init(); }

	void readFile(string name);

	void init();
	void test(); // Some unit tests. Here because I'm lazy

	std::map<string, float> parsedFloat;
	std::map<string, bool> parsedBool;
	std::map<string, int> parsedInt;
private:
};
//Helpers
string cut(string& str);
std::vector<string>& split(string& str);


#endif
