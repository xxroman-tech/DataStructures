#pragma once

#include <vector>
#include "../Structures/Models/TerritorialUnit.h"
#include "../Structures/TerritorialUnitStructure.h"
#include<functional> 
#include <libds/amt/explicit_hierarchy.h>

using namespace std;

class Searcher {
public:
	static void getTerritorialUnitsByString(ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* territorialUnits, TerritorialStructure* filteredUnits, const function <bool(TerritorialUnit*)>& operation);
};

void Searcher::getTerritorialUnitsByString(
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* territorialUnits,
	TerritorialStructure* filteredUnits,
	const function <bool(TerritorialUnit*)>& operation) {
	
	for (TerritorialUnit* unit : *territorialUnits) {

		if (operation(unit)) {
			filteredUnits->add(unit);
		}

	}

}