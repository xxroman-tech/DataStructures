#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "../Enums/TerritorialUnitTypes.h"

using namespace std;

class TerritorialUnit {
public:
	TerritorialUnit(const string code, string officialTitle, string mediumTitle, string shortTitle, string note, TerritorialUnitType pType);
	~TerritorialUnit();
	string getCode();
	string getOfficialTitle();
	string getMediumTitle();
	string getShortTitle();
	string getNote();
	TerritorialUnitType getType();
	string toString();
private:
	TerritorialUnitType type;
	string code;
	string officialTitle;
	string mediumTitle;
	string shortTitle;
	string note;
};

TerritorialUnit::TerritorialUnit(const string pCode, string pOfficialTitle, string pMediumTitle, string pShortTitle, string pNote, TerritorialUnitType pType) : 
	code(pCode),
	officialTitle(pOfficialTitle),
	mediumTitle(pMediumTitle),
	shortTitle(pShortTitle),
	note(pNote),
	type(pType) { }

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

TerritorialUnitType TerritorialUnit::getType() {
	return this->type;
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