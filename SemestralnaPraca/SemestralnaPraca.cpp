#pragma once

#include <iostream>
#include <Windows.h>
#include "App/App.h"
#include "Services/Searcher.h"
#include <algorithm>
#include <libds/heap_monitor.h>
#include "Structures/Enums/TerritorialUnitTypes.h"

void vyhladavanie();
void findInformation(int s);
void startsWith(string vstup);
void contains(string vstup);
void hasType(TerritorialUnitType type);
void coutToConsole();

void vypisAktPoziciu();

App* app;
TerritorialStructure* filteredUnits;

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
        vypisAktPoziciu();

        cout << "  -2. Vyhladajte v podstrome" << endl;
        cout << "Zvolte akciu:" << endl;
        if (app->getIterator()->getPosition()->parent_ != nullptr) {
            cout << "  -1. Presun na: " << app->getIterator()->getPosition()->parent_->data_->getOfficialTitle() << endl;
        }

        if (app->getIterator()->getPosition()->data_->getType() != DISTRICT) {
            app->vypisMozneCesty();
        }

        cin >> vstup;
        system("cls");

        if (vstup == -2) {
            vyhladavanie();
        } 
        else {
            if (vstup == -1) {
                app->getIterator()->up();
            }
            else {
                app->getIterator()->down(vstup);
            }
        }
    }

    delete app;
    return 0;

}

void vyhladavanie() {
    vypisAktPoziciu();
    int vstupVyhladavnia = -1;

    while (vstupVyhladavnia != 0) {
        cout << "Zadajte akciu: " << endl;
        cout << "  1. Vyhladat informacie 'startsWith'" << endl;
        cout << "  2. Vyhladat informacie 'cotains'" << endl;
        cout << "  3. Vyhladat informacie podla 'hasType'" << endl;

        cin >> vstupVyhladavnia;
        system("cls");

        if (vstupVyhladavnia > 3) {
            cout << "Zly vstup!" << endl;
        }
        else if (vstupVyhladavnia == 0) {
            break;
        }
        else {
            findInformation(vstupVyhladavnia);
        }
    }
}

void findInformation(int s) {
    string vstup;

    while (vstup != "q") {
        vypisAktPoziciu();
        cout << "\nPre navrat stlacte 'q'" << endl;
        if (s != 3) {
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
        }
        else {
            cout << "Zvolte z troch typov 'uzemnych jednotiek' ktore chcete vyhladat:" << endl;
            cout << "  1. Obec" << endl;
            cout << "  2. Okres" << endl;
            cout << "  3. Kraj'" << endl;

            bool koniec = false;
            int typeVstup = -1;

            while (!koniec) {
                cin >> typeVstup;
                if (typeVstup > 3 || typeVstup < 1) {
                    cout << "Zly vstup!" << endl;
                }
                else {
                    koniec = true;
                }
            }

            switch (typeVstup) {
                case 1:
                    hasType(MUNICIPALITY);
                    break;
                case 2:
                    hasType(DISTRICT);
                    break;
                case 3:
                    hasType(REGION);
                    break;
            }
        }

        coutToConsole();

        vstup = "q";
    }
}

void vypisAktPoziciu() { 
    cout << "Aktualna pozicia: " << app->getIterator()->getPosition()->data_->getOfficialTitle() << endl << endl; 
}

void startsWith(string vstup) {

    filteredUnits = new TerritorialStructure();

    // Zacina na znak
    //https://www.techiedelight.com/check-if-a-string-starts-with-a-certain-string-in-cpp/
     
    Searcher::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        string name = territorialUnit->getShortTitle();

        for_each(name.begin(), name.end(), [](char& c) {
            c = ::tolower(c);
        });

        return name.rfind(vstup, 0) == 0;
    });
}

void contains(string vstup) {

    filteredUnits = new TerritorialStructure();

    // Coitains
    // https://www.techiedelight.com/check-if-a-string-contains-another-string-in-cpp/

    Searcher::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        string name = territorialUnit->getShortTitle();

        for_each(name.begin(), name.end(), [](char& c) {
            c = ::tolower(c);
        });

        return name.find(vstup) != string::npos;
    });
}

void hasType(TerritorialUnitType type) {

    filteredUnits = new TerritorialStructure();

    // Zacina na znak
    //https://www.techiedelight.com/check-if-a-string-starts-with-a-certain-string-in-cpp/

    Searcher::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        return territorialUnit->getType() == type;
    });
}

void coutToConsole() {
    int i = 1;
    cout << "\n" << endl;
    for (TerritorialUnit* filUnit : *filteredUnits) {
        cout << i << ". | " << filUnit->toString() << endl;
        i++;
    }

    delete filteredUnits;
}
