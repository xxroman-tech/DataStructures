#pragma once

#include <vector>
#include "../Structures/Models/TerritorialUnit.h"
#include "../Structures/TerritorialUnitStructure.h"
#include<functional> 
#include <libds/amt/explicit_hierarchy.h>

using namespace std;

class Searcher {
public:
	static void getTerritorialUnitsByString(
		ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator begin,
		ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator end,
		TerritorialStructure* filteredUnits, 
		const function <bool(TerritorialUnit*)>& operation);
};

void Searcher::getTerritorialUnitsByString(
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator begin,
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator end,
	TerritorialStructure* filteredUnits,
	const function <bool(TerritorialUnit*)>& operation) {

	for (ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>>::PreOrderHierarchyIterator it = begin; it != end; ++it) {
		if (operation(*it)) {
			filteredUnits->add(*it);
		}
	}
}