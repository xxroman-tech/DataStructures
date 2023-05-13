#pragma once

#include <vector>
#include "../Structures/Models/TerritorialUnit.h"
#include "../Structures/TerritorialUnitStructure.h"
#include<functional> 
#include <libds/amt/explicit_hierarchy.h>

using namespace std;

template<typename T, typename BlockType>
class Searcher {
public:
	static void getTerritorialUnitsByString(
		T begin, T end,
		ds::adt::ImplicitList<TerritorialUnit*>* filteredUnits,
		const function <bool(TerritorialUnit*)>& operation);

	static void getTerritorialUnitsFromTable(
		ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>* list, 
		ds::adt::ImplicitList<TerritorialUnit*>* filteredUnits,
		string officialTitle);
};

template<typename T, typename BlockType>
void Searcher<T, BlockType>::getTerritorialUnitsByString(
	T begin, T end,
	ds::adt::ImplicitList<TerritorialUnit*>* filteredUnits,
	const function <bool(TerritorialUnit*)>& operation) {

	for (BlockType it = begin; it != end; ++it) {
		if (operation(*it)) {
			filteredUnits->insertLast(*it);
		}
	}
}

template<typename T, typename BlockType>
void Searcher<T, BlockType>::getTerritorialUnitsFromTable(
	ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>* list,
	ds::adt::ImplicitList<TerritorialUnit*>* filteredUnits,
	string officialTitle) {

	ds::adt::ImplicitList<TerritorialUnit*>* list2 = nullptr;
	ds::adt::ImplicitList<TerritorialUnit*>** data = &list2;
	
	if (!list->tryFind(officialTitle, data)) {
		return;
	}

	for (auto unit : **data) {
		filteredUnits->insertLast(unit);
	}
}