#pragma once

#include "../Structures/TerritorialUnitStructure.h"
#include "../DataLoader/DataLoader.h"
#include "../SemestralnaPraca/Structures/Enums/TerritorialUnitTypes.h"
#include <libds/amt/explicit_hierarchy.h>

// Trieda bude loadovat data do pozadovanych struktur
// Zatial loaduje len do struktury TerritorialUnitStructure ktora ma vector 

class App {
public:
	App();
	~App();

	void load();
	TerritorialStructure* getUnits();
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* getHierarchy();
private:
	void loadFromFile(string filePath, TerritorialUnitType type);

	TerritorialStructure* units;
	TerritorialUnit* sk;
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* hierarchy;
};

App::App() :
	units(new TerritorialStructure())
{
	this->hierarchy = new ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>();

	this->sk = new TerritorialUnit("SK", "Slovenská republika", "", "SR", "", STATE);

	this->hierarchy->emplaceRoot().data_ = this->sk;
}

App::~App() {
	delete hierarchy;
	hierarchy = nullptr;

	delete this->sk;
	this->sk = nullptr;

	for (TerritorialUnit* unit : *units) {
		delete unit;
	}

	delete units;
	units = nullptr;
}

void App::load() {

	this->loadFromFile("Resources/Data/kraje_c.csv", REGION);
	this->loadFromFile("Resources/Data/okresy_c.csv", DISTRICT);
	this->loadFromFile("Resources/Data/obce_c.csv", MUNICIPALITY);

	for (TerritorialUnit* unit : *this->units) {
		if (unit->getType() == REGION) {
			this->hierarchy->emplaceSon(*this->hierarchy->accessRoot(), this->hierarchy->accessRoot()->sons_->size()).data_ = unit;
		}

		if (unit->getType() == DISTRICT) {
			for (auto region : *this->hierarchy->accessRoot()->sons_) {
				if (unit->getCode().substr(0, 5) == region->data_->getCode()) {
					this->hierarchy->emplaceSon(*region, region->sons_->size()).data_ = unit;
				}
			}
		}

		if (unit->getType() == MUNICIPALITY) {
			for (auto region : *this->hierarchy->accessRoot()->sons_) {
				for (auto district : *region->sons_) {
					if (unit->getCode().substr(0, 6) == district->data_->getCode()) {
						this->hierarchy->emplaceSon(*district, district->sons_->size()).data_ = unit;
					}
				}
			}
		}
	}

	// Vypis
	//this->hierarchy->processPreOrder(this->hierarchy->accessRoot(), [&](const ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* terrUnit) {
	//	cout << terrUnit->data_->toString() << endl;
	//});
}

TerritorialStructure* App::getUnits() {
	return this->units;
}

ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* App::getHierarchy() {
	return this->hierarchy;
}

void App::loadFromFile(string filePath, TerritorialUnitType type) {
	DataLoader loader = DataLoader(filePath);
	loader.openFile();
	loader.nextLine();

	while (loader.nextLine()) {
		TerritorialUnit* unit = new TerritorialUnit(
			loader[1],
			loader[2],
			loader[3],
			loader[4],
			loader[5],
			type
		);

		this->units->add(unit);
	}
}