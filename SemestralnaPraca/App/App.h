#pragma once

#include "../Structures/TerritorialUnitStructure.h"
#include "../DataLoader/DataLoader.h"
#include "../SemestralnaPraca/Structures/Enums/TerritorialUnitTypes.h"
#include "../SemestralnaPraca/Structures/HierarchyIterator.h"

// Trieda bude loadovat data do pozadovanych struktur

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
	void loadAndSaveBonusData(string filePath);
	void agregateBonusData();

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

	cout << "Data sa nacitavaju do tabuliek ....." << endl;

	this->loadFromFile("Resources/Data/kraje_c.csv", REGION);
	this->loadFromFile("Resources/Data/okresy_c.csv", DISTRICT);
	this->loadFromFile("Resources/Data/obce_c.csv", MUNICIPALITY);
	this->loadAndSaveBonusData("Resources/Data/bonus_c.csv");

	cout << "Data nacitane do tabuliek." << endl;

	cout << "Roztriedenie dat do hierarchie ....." << endl;

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

	cout << "Data ulozene v hierarchii." << endl;

	this->iterator = new HierarchyIterator(this->hierarchy, this->hierarchy->accessRoot());

	cout << "Prebieha agregacia dat ....." << endl;
	this->agregateBonusData();
	cout << "Agregacie dat o obyvateloch prebehla uspesne." << endl;
}

TerritorialStructure* App::getUnits() {
	return this->units;
}

ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* App::getHierarchy() {
	return this->hierarchy;
}

void App::vypisMozneCesty() {
	size_t index = 1;
	cout << endl;
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

	loader.closeFile();
}

void App::loadAndSaveBonusData(string filePath) {
	DataLoader loader = DataLoader(filePath);
	loader.openFile();
	loader.nextLine();

	while (loader.nextLine()) {
		string municCode = loader[0];
		string num = loader[4];
		int diffNum = 0;

		stringstream stream(num);
		stream >> diffNum;

		TerritorialUnit** unit = nullptr;

		if (this->units->municipalityTerrUnitsByCODE->tryFind(municCode, unit))
			(*unit)->setDifferenceOfPopulation(diffNum);
	}

	loader.closeFile();
}

void App::agregateBonusData() {

	int numInState = 0;

	for (ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* region : *this->hierarchy->accessRoot()->sons_) {

		int numInRegion = 0; // Sucet rozdielov obyvatelstva v roku 2022 a 21 v okresoch kraja

		for (ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* district : *region->sons_) {

			int numInDistrict = 0; // Sucet rozdielov obyvatelstva v roku 2022 a 21 v obciach daneho okresu

			for (ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* municip : *district->sons_) {

				numInDistrict += municip->data_->getDifferenceOfPopulation();

			}

			district->data_->setDifferenceOfPopulation(numInDistrict);
			numInRegion += numInDistrict;

		}

		region->data_->setDifferenceOfPopulation(numInRegion);
		numInState += numInRegion;

	}

	this->hierarchy->accessRoot()->data_->setDifferenceOfPopulation(numInState);

}