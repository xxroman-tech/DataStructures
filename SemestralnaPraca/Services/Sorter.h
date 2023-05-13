#pragma once

#include <libds/amt/implicit_sequence.h>
#include "../SemestralnaPraca/Structures/Models/TerritorialUnit.h"
#include <libds/adt/sorts.h>

class Sorter {
public:
	static void compareAlphabetical(
		ds::amt::ImplicitSequence<TerritorialUnit*>* sequence,
		bool asc // Vzostupne
	);

	static void compareVowelsCount(
		ds::amt::ImplicitSequence<TerritorialUnit*>* sequence,
		bool asc
	);

	static void toLowerCase(
		std::string::iterator beginIterator,
		std::string::iterator endIterator
	);
};

void Sorter::compareAlphabetical(
	ds::amt::ImplicitSequence<TerritorialUnit*>* sequence,
	bool asc) {

	ds::adt::QuickSort<TerritorialUnit*> sorter = ds::adt::QuickSort<TerritorialUnit*>();

	sorter.sort(*sequence, [&](TerritorialUnit* firstUnit, TerritorialUnit* secondUnit) {
		string firstUnitName = firstUnit->getOfficialTitle();
		string secondUnitName = secondUnit->getOfficialTitle();

		Sorter::toLowerCase(firstUnitName.begin(), firstUnitName.end());
		Sorter::toLowerCase(secondUnitName.begin(), secondUnitName.end());

		return firstUnitName < secondUnitName;
	});

}

void Sorter::compareVowelsCount(
	ds::amt::ImplicitSequence<TerritorialUnit*>* sequence,
	bool asc) {

	ds::adt::QuickSort<TerritorialUnit*> sorter = ds::adt::QuickSort<TerritorialUnit*>();
	
	sorter.sort(*sequence, [&](TerritorialUnit* firstUnit, TerritorialUnit* secondUnit) {
		char samohlasky[] = { 'a', 'e', 'ä', 'i', 'o', 'u', 'y', 'á', 'é', 'í', 'ó', 'ú', 'ý', 'ô' };

		size_t numOfWovelsFirst = 0;
		size_t numOfWovelsSecond = 0;

		string firstUnitName = firstUnit->getOfficialTitle();
		string secondUnitName = secondUnit->getOfficialTitle();

		for_each(firstUnitName.begin(), firstUnitName.end(), [&](char& c) {
			c = ::tolower(c);
			for (size_t i = 0; i < sizeof(samohlasky); i++) {
				if (samohlasky[i] == c)
					numOfWovelsFirst++;
			}
		});

		for_each(secondUnitName.begin(), secondUnitName.end(), [&](char& c) {
			c = ::tolower(c);
			for (size_t i = 0; i < sizeof(samohlasky); i++) {
				if (samohlasky[i] == c)
					numOfWovelsSecond++;
			}
		});

		return asc ? numOfWovelsFirst < numOfWovelsSecond
			: numOfWovelsFirst > numOfWovelsSecond;
	});

}

void Sorter::toLowerCase(
	std::string::iterator beginIterator,
	std::string::iterator endIterator) {
	
	for_each(beginIterator, endIterator, [&](char& c) {
		c = ::tolower(c);
	});

}
