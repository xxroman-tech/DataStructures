#pragma once

#include <vector>
#include "../Structures/Models/TerritorialUnit.h"
#include "../Structures/TerritorialUnitStructure.h"
#include<functional> 

using namespace std;

class Searcher {
public:
	static void getTerritorialUnitsByString(TerritorialStructure* territorialUnits, TerritorialStructure* filteredUnits, const function <bool(TerritorialUnit*)>& operation);
};

void Searcher::getTerritorialUnitsByString(
	TerritorialStructure* territorialUnits,
	TerritorialStructure* filteredUnits,
	const function <bool(TerritorialUnit*)>& operation) {
	
	for (TerritorialUnit* unit : *territorialUnits) {

		if (operation(unit)) {
			filteredUnits->add(unit);
		}

	}

}