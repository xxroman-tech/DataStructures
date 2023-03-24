#pragma once

#include <iostream>
#include <Windows.h>
#include "App/App.h"

#include "Services/Searcher.h"
#include <algorithm>
#include "../DataStructures/libds/heap_monitor.h"

void findInformation(int s);
void startsWith(string vstup);
void contains(string vstup);
void coutToConsole();

App* app;
TerritorialStructure* filteredUnits = new TerritorialStructure();

int main()
{
    initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    app = new App();
    app->load();
    cout << "Data sa nacitali!" << endl;

    int vstup = -1;

    while (vstup != 0) {
        cout << "\nPre ukoncenie stlacte nulu '0'" << endl;
        cout << "Zadajte akciu: " << endl;
        cout << "  1. Vyhladat informacie 'startsWith'" << endl;
        cout << "  2. Vyhladat informacie 'cotains'" << endl;

        cin >> vstup;

        if (vstup > 2) {
            cout << "Zly vstup!" << endl;
        }
        else if (vstup == 0) {
            break;
        }
        else {
            findInformation(vstup);
        }
    }

    delete filteredUnits;
    delete app;
    return 0;

}

void findInformation(int s) {
    string vstup;

    while (vstup != "q") {
        cout << "\nPre navrat stlacte 'q'" << endl;
        cout << "Zadajte text ktory chcete vyhladat: " << endl;

        cin >> vstup;

        // Change to lowecase
        for_each(vstup.begin(), vstup.end(), [](char& c) {
            c = ::tolower(c);
            });

        if (s == 1) {
            startsWith(vstup);
        }
        else if (s == 2) {
            contains(vstup);
        }

        coutToConsole();

        vstup = "q";
    }
}

void startsWith(string vstup) {
    // Zacina na znak
    //https://www.techiedelight.com/check-if-a-string-starts-with-a-certain-string-in-cpp/
     
    Searcher::getTerritorialUnitsByString(app->getUnits(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        string name = territorialUnit->getShortTitle();

        for_each(name.begin(), name.end(), [](char& c) {
            c = ::tolower(c);
        });

        return name.rfind(vstup, 0) == 0;
    });
}

void contains(string vstup) {
    // Coitains
    // https://www.techiedelight.com/check-if-a-string-contains-another-string-in-cpp/

    Searcher::getTerritorialUnitsByString(app->getUnits(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        string name = territorialUnit->getShortTitle();

        for_each(name.begin(), name.end(), [](char& c) {
            c = ::tolower(c);
        });

        return name.find(vstup) != string::npos;
    });
}

void coutToConsole() {
    int i = 1;
    cout << "\n" << endl;
    for (TerritorialUnit* filUnit : *filteredUnits) {
        cout << i << ". | " << filUnit->toString() << endl;
        i++;
    }
}
