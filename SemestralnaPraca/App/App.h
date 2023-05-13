#pragma once

#include "../Structures/TerritorialUnitStructure.h"
#include "../DataLoader/DataLoader.h"
#include "../SemestralnaPraca/Structures/Enums/TerritorialUnitTypes.h"
#include <libds/amt/explicit_hierarchy.h>

// Trieda bude loadovat data do pozadovanych struktur
// Zatial loaduje len do struktury TerritorialUnitStructure ktora ma vector 


class HierarchyIterator {
public:
	HierarchyIterator(ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* pHierarchy, ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* pPosition);
	ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* up();
	ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* down(size_t index);

	ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* getPosition();
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator begin();
private:
	ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* position;
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* hierarchy;
};

HierarchyIterator::HierarchyIterator(ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* pHierarchy, ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* pPosition) :
	hierarchy(pHierarchy),
	position(pPosition)
{ }

ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* HierarchyIterator::up() {
	return this->position = this->hierarchy->accessParent(*position);
}

ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* HierarchyIterator::down(size_t index) {
	return this->position = this->hierarchy->accessSon(*position, index - 1);
}

ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* HierarchyIterator::getPosition() {
	return this->position;
}

ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator HierarchyIterator::begin() {
	return ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator(this->hierarchy, this->position); 
}


class App {
public:
	App();
	~App();

	void load();
	TerritorialStructure* getUnits();
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* getHierarchy();
	HierarchyIterator* getIterator() { return this->iterator; }

	void vypisMozneCesty();
private:
	void loadFromFile(string filePath, TerritorialUnitType type);

	TerritorialStructure* units;
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* hierarchy;
	HierarchyIterator* iterator;
};

App::App() :
	units(new TerritorialStructure())
{
	this->hierarchy = new ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>();

	this->hierarchy->emplaceRoot().data_ = new TerritorialUnit("SK", "Slovenská republika", "", "SR", "", STATE);
}

App::~App() {
	delete iterator;
	iterator = nullptr;

	delete hierarchy->accessRoot()->data_;
	delete hierarchy;
	hierarchy = nullptr;

	delete units;
	units = nullptr;
}

void App::load() {

	this->loadFromFile("Resources/Data/kraje_c.csv", REGION);
	this->loadFromFile("Resources/Data/okresy_c.csv", DISTRICT);
	this->loadFromFile("Resources/Data/obce_c.csv", MUNICIPALITY);

	for (auto region : *this->units->regionTerrUnits) {
		for (auto regionInsideBlock : *region.data_) {
			this->hierarchy->emplaceSon(*this->hierarchy->accessRoot(), this->hierarchy->accessRoot()->sons_->size()).data_ = regionInsideBlock;
		}
	}

	for (auto district : *this->units->districtTerrUnits) {
		for (auto districtInsideBlock : *district.data_) {
			for (ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* region : *this->hierarchy->accessRoot()->sons_) {
				if (districtInsideBlock->getCode().substr(0, 5) == region->data_->getCode()) {
					this->hierarchy->emplaceSon(*region, region->sons_->size()).data_ = districtInsideBlock;
				}
			}
		}
	}

	for (auto municipality : *this->units->municipalityTerrUnits) {
		for (auto municInsideBlock : *municipality.data_) {
			for (ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* region : *this->hierarchy->accessRoot()->sons_) {
				for (ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* district : *region->sons_) {
					if (municInsideBlock->getCode().substr(0, 6) == district->data_->getCode()) {
						this->hierarchy->emplaceSon(*district, district->sons_->size()).data_ = municInsideBlock;
					}
				}
			}
		}
	}


	// Vypis
	//this->hierarchy->processPreOrder(this->hierarchy->accessRoot(), [&](const ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* terrUnit) {
	//	cout << terrUnit->data_->toString() << endl;
	//});

	this->iterator = new HierarchyIterator(this->hierarchy, this->hierarchy->accessRoot());
}

TerritorialStructure* App::getUnits() {
	return this->units;
}

ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* App::getHierarchy() {
	return this->hierarchy;
}

void App::vypisMozneCesty() {
	size_t index = 1;
	for (ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* unit : *this->iterator->getPosition()->sons_) {
		cout << index++ << ". " << unit->data_->toString() << endl;
	}
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