#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

class DataLoader {
public:
	DataLoader(const string fName);
	~DataLoader();
	bool isFileOpen();
	void openFile();
	void closeFile();
	bool nextLine();
	string operator[](int index);
private:
	string fileName;
	fstream* file;
	vector<string> line;
};

DataLoader::DataLoader(const string fName) :
	fileName(fName),
	file(new fstream()),
	line(vector<string>()) { }

DataLoader::~DataLoader() {
	this->closeFile();

	delete this->file;
	this->file = nullptr;
}

bool DataLoader::isFileOpen() {
	return this->file->is_open();
}

void DataLoader::openFile() {
	if (this->isFileOpen() == false) {
		this->file->open(fileName, ios::in);
	}
}

void DataLoader::closeFile() {
	if (this->isFileOpen() == true) {
		this->file->close();
		this->line.clear();
	}
}

bool DataLoader::nextLine() {
	if (this->isFileOpen() == false) {
		return false;
	}

	string text;

	if (getline(*this->file, text)) {
		stringstream line(text);

		this->line.clear();

		while (getline(line, text, ';')) {
			this->line.push_back(text);
		}

		return true;
	} else {
		this->closeFile();

		return false;
	}
}

string DataLoader::operator[](int index) {
	return this->line.at(index);
}
