#pragma once

#include <iostream>
#include <vector>
#include "Models/TerritorialUnit.h"

#include <libds/adt/table.h>
#include <libds/adt/list.h>

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

	ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>* regionTerrUnits;
	ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>* districtTerrUnits;
	ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>* municipalityTerrUnits;
private:
	vector<TerritorialUnit*> territorialUnits;
};

TerritorialStructure::TerritorialStructure() :
	territorialUnits(vector<TerritorialUnit*>()) { 
	this->regionTerrUnits = new ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>();
	this->districtTerrUnits = new ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>();
	this->municipalityTerrUnits = new ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>();
}

TerritorialStructure::~TerritorialStructure() {
	this->territorialUnits.clear();

	for (auto region : *this->regionTerrUnits) {
		delete region.data_;
	}

	for (auto district : *this->districtTerrUnits) {
		delete district.data_;
	}

	for (auto munic : *this->municipalityTerrUnits) {
		delete munic.data_;
	}

	this->regionTerrUnits->clear();
	this->districtTerrUnits->clear();
	this->municipalityTerrUnits->clear();

	delete this->regionTerrUnits;
	delete this->districtTerrUnits;
	delete this->municipalityTerrUnits;
}

void TerritorialStructure::add(TerritorialUnit* territorialUnit) {
	this->territorialUnits.push_back(territorialUnit);

	ds::adt::ImplicitList<TerritorialUnit*>* list2 = nullptr;
	ds::adt::ImplicitList<TerritorialUnit*>** list = &list2;

	switch (territorialUnit->getType())
	{
	case REGION:
		if (!this->regionTerrUnits->tryFind(territorialUnit->getOfficialTitle(), list)) {
			*list = new ds::adt::ImplicitList<TerritorialUnit*>();

			this->regionTerrUnits->insert(territorialUnit->getOfficialTitle(), *list);
		}
		break;
	case DISTRICT:
		if (!this->districtTerrUnits->tryFind(territorialUnit->getOfficialTitle(), list)) {
			*list = new ds::adt::ImplicitList<TerritorialUnit*>();

			this->districtTerrUnits->insert(territorialUnit->getOfficialTitle(), *list);
		}
		break;
	case MUNICIPALITY:
		if (!this->municipalityTerrUnits->tryFind(territorialUnit->getOfficialTitle(), list)) {
			*list = new ds::adt::ImplicitList<TerritorialUnit*>();

			this->municipalityTerrUnits->insert(territorialUnit->getOfficialTitle(), *list);
		}
		break;
	}


	(*list)->insertLast(territorialUnit);
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

