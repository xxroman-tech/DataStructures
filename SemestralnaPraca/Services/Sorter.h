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

	static std::string toLowerCase(std::string text);
};

void Sorter::compareAlphabetical(
	ds::amt::ImplicitSequence<TerritorialUnit*>* sequence,
	bool asc) {

	ds::adt::QuickSort<TerritorialUnit*> sorter = ds::adt::QuickSort<TerritorialUnit*>();

	sorter.sort(*sequence, [&](TerritorialUnit* firstUnit, TerritorialUnit* secondUnit) {
		string firstUnitName = firstUnit->getOfficialTitle();
		string secondUnitName = secondUnit->getOfficialTitle();

		// Pre tento algoritmus mi dal zaklad OpenAi chat bot, nijak inak sa mi to nepodarilo naimplementovat
		const char* sk_alphabet = asc ? "aAáÁäÄbBcCèÈdDïÏeEéÉfFgGhHiIíÍjJkKlLåÅ¾¼mMnNòÒoOóÓôÔpPqQrRàÀsSšŠtTuUúÚvVwWxXyYıİzZ"
									  : "ZzİıYyXxWwVvÚúUuTtŠšSsÀàRrQqPpÔôÓóOoÒòNnMm¼¾ÅåLlKkJjÍíIiHhGgFfÉéEeÏïDdÈèCcBbÄäÁáAa";
		const char* c1 = firstUnitName.c_str();
		const char* c2 = secondUnitName.c_str();
		while (*c1 && *c2) {
			char ch1 = *c1;
			char ch2 = *c2;
			if (ch1 >= 'A' && ch1 <= 'Z') {
				ch1 = ch1 - 'A' + 'a';
			}
			if (ch2 >= 'A' && ch2 <= 'Z') {
				ch2 = ch2 - 'A' + 'a';
			}
			const char* pos1 = strchr(sk_alphabet, ch1);
			const char* pos2 = strchr(sk_alphabet, ch2);
			if (pos1 && pos2) {
				if (pos1 < pos2) {
					return true;
				}
				else if (pos1 > pos2) {
					return false;
				}
			}
			else if (pos1) {
				return true;
			}
			else if (pos2) {
				return false;
			}
			else {
				if (ch1 < ch2) {
					return true;
				}
				else if (ch1 > ch2) {
					return false;
				}
			}
			c1++;
			c2++;
		}
		return *c1 < *c2;
	});

}

void Sorter::compareVowelsCount(
	ds::amt::ImplicitSequence<TerritorialUnit*>* sequence,
	bool asc) {

	ds::adt::QuickSort<TerritorialUnit*> sorter = ds::adt::QuickSort<TerritorialUnit*>();
	
	sorter.sort(*sequence, [&](TerritorialUnit* firstUnit, TerritorialUnit* secondUnit) {
		char samohlasky[] = { 'a', 'e', 'ä', 'i', 'o', 'u', 'y', 'á', 'é', 'í', 'ó', 'ú', 'ı', 'ô' };

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

std::string Sorter::toLowerCase(std::string text) {

	std::string sk_alphabet_lower = "aáäbcèdïdzdeéfghiíjklå¾mnòoóôpqràsštuúvwxyız";
	std::string sk_alphabet_upper = "AÁÄBCÈDÏDZDEÉFGHIÍJKLÅ¼MNÒOÓÔPQRÀSŠTUÚVWXYİZ";

	for (int c = 0; c < text.size(); c++) {
		for (auto i = 0; i < sk_alphabet_upper.size(); i++) {
			if (text.at(c) == sk_alphabet_upper.at(i))
				text.at(c) = sk_alphabet_lower.at(i);
		}
	}

	return text;

}
