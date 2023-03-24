#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class TerritorialUnit {
public:
	TerritorialUnit(const string code, string officialTitle, string mediumTitle, string shortTitle, string note);
	~TerritorialUnit();
	string getCode();
	string getOfficialTitle();
	string getMediumTitle();
	string getShortTitle();
	string getNote();
	string toString();
private:
	string code;
	string officialTitle;
	string mediumTitle;
	string shortTitle;
	string note;
};

TerritorialUnit::TerritorialUnit(const string pCode, string pOfficialTitle, string pMediumTitle, string pShortTitle, string pNote) : 
	code(pCode),
	officialTitle(pOfficialTitle),
	mediumTitle(pMediumTitle),
	shortTitle(pShortTitle),
	note(pNote) { }

TerritorialUnit::~TerritorialUnit() { }

string TerritorialUnit::getCode() {
	return this->code;
}

string TerritorialUnit::getOfficialTitle() {
	return this->officialTitle;
}

string TerritorialUnit::getMediumTitle() {
	return this->mediumTitle;
}

string TerritorialUnit::getShortTitle() {
	return this->shortTitle;
}

string TerritorialUnit::getNote() {
	return this->note;
}

string TerritorialUnit::toString() {
	stringstream stream;

	stream << " " << this->code;
	stream << " | " << this->officialTitle;
	stream << " | " << this->mediumTitle;
	stream << " | " << this->shortTitle;
	stream << " | " << this->note;

	return stream.str();
}