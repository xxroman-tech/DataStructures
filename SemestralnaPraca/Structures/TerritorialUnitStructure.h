#pragma once

#include <iostream>
#include <vector>
#include "Models/TerritorialUnit.h"

using namespace std;

// Tato trieda sluzi ako vyhladavaci "kontainer" struktura 

class TerritorialStructure {
public:
	TerritorialStructure();
	~TerritorialStructure();
	void add(TerritorialUnit* territorialUnit);

	// https://stackoverflow.com/questions/65324393/making-a-class-iterable-with-iteration-order-depending-on-class-implementation?fbclid=IwAR2HEXxtbgHtVdR2p8DUZEIjPtkVcv-IVd_-nIyQ4PDsqXPFAyuoVJH7pNM
	auto begin();
	auto begin() const;
	auto end();
	auto end() const;
private:
	vector<TerritorialUnit*> territorialUnits;
};

TerritorialStructure::TerritorialStructure() :
	territorialUnits(vector<TerritorialUnit*>()) { }

TerritorialStructure::~TerritorialStructure() {
	this->territorialUnits.clear();
}

void TerritorialStructure::add(TerritorialUnit* territorialUnit) {
	this->territorialUnits.push_back(territorialUnit);
}

auto TerritorialStructure::begin() {
	return this->territorialUnits.begin();
}

auto TerritorialStructure::begin() const {
	return this->territorialUnits.begin();
}

auto TerritorialStructure::end() {
	return this->territorialUnits.end();
}

auto TerritorialStructure::end() const {
	return this->territorialUnits.end();
}

