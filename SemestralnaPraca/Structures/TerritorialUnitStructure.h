#pragma once

#include <iostream>
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

	ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>* regionTerrUnits;
	ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>* districtTerrUnits;
	ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>* municipalityTerrUnits;
};

TerritorialStructure::TerritorialStructure() { 
	this->regionTerrUnits = new ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>();
	this->districtTerrUnits = new ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>();
	this->municipalityTerrUnits = new ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>();
}

TerritorialStructure::~TerritorialStructure() {
	for (auto region : *this->regionTerrUnits) {
		for (auto x : *region.data_) {
			delete x;
		}
		region.data_->clear();
		delete region.data_;
	}

	for (auto district : *this->districtTerrUnits) {
		for (auto x : *district.data_) {
			delete x;
		}
		district.data_->clear();
		delete district.data_;
	}

	for (auto munic : *this->municipalityTerrUnits) {
		for (auto x : *munic.data_) {
			delete x;
		}
		munic.data_->clear();
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

