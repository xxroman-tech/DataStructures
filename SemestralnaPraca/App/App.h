#pragma once

#include "../Structures/TerritorialUnitStructure.h"
#include "../DataLoader/DataLoader.h"

// Trieda bude loadovat data do pozadovanych struktur
// Zatial loaduje len do struktury TerritorialUnitStructure ktora ma vector 

class App {
public:
	App();
	~App();

	void load();
	TerritorialStructure* getUnits();
private:
	void loadFromFile(string filePath);
	TerritorialStructure* units;
};

App::App() :
	units(new TerritorialStructure()) { }

App::~App() {
	for (TerritorialUnit* unit : *units) {
		delete unit;
	}

	delete units;
	units = nullptr;
}

void App::load() {
	this->loadFromFile("Resources/Data/kraje_c.csv");
	this->loadFromFile("Resources/Data/okresy_c.csv");
	this->loadFromFile("Resources/Data/obce_c.csv");
}

TerritorialStructure* App::getUnits() {
	return this->units;
}

void App::loadFromFile(string filePath) {
	DataLoader loader = DataLoader(filePath);
	loader.openFile();
	loader.nextLine();

	while (loader.nextLine()) {
		TerritorialUnit* unit = new TerritorialUnit(
			loader[1],
			loader[2],
			loader[3],
			loader[4],
			loader[5]
		);

		this->units->add(unit);
	}
}