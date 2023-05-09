#pragma once

#include <iostream>
#include <Windows.h>
#include "App/App.h"
#include "Services/Searcher.h"
#include <algorithm>
#include <libds/heap_monitor.h>
#include "Structures/Enums/TerritorialUnitTypes.h"

void mainMenu();
void vyhladavanieVHierarchii();
void vyhladavanie();
void vyhladanieVTabulkach();
void findInformation(int s);
void startsWith(string vstup);
void contains(string vstup);
void hasType(TerritorialUnitType type);
void hasTypeTabulky(TerritorialUnitType type, string text);
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

    mainMenu();
    
    delete app;
    return 0;

}

void mainMenu() {
    int vstupMenu = -1;

    while (vstupMenu != 0) {
        cout << "Zvolte typ vyhladavania:" << endl;
        cout << "  1. Vyhladavanie v Tabulkach" << endl;
        cout << "  2. Vyhladavanie v Hierarchii" << endl;

        bool koniec = false;
        int typeVstup = -1;

        while (!koniec) {
            cin >> typeVstup;
            if (typeVstup > 2 || typeVstup < 0) {
                cout << "Zly vstup!" << endl;
            }
            else {
                koniec = true;
            }
        }

        system("cls");

        if (typeVstup == 0)
            vstupMenu = 0;
        else if (typeVstup == 1)
            vyhladanieVTabulkach();
        else if (typeVstup == 2)
            vyhladavanieVHierarchii();

    }
}

// 2.Uroven
void vyhladavanieVHierarchii() {
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
}

// 3.Uroven
void vyhladanieVTabulkach() {
    int vstupVyhladavania = -1;

    while (vstupVyhladavania != 0) {
        cout << "Zvolte z troch typov 'uzemnych jednotiek' ktore chcete vyhladat:" << endl;
        cout << "  1. Obec" << endl;
        cout << "  2. Okres" << endl;
        cout << "  3. Kraj'" << endl;

        bool koniec = false;
        int typeVstup = -1;

        while (!koniec) {
            cin >> typeVstup;
            if (typeVstup > 3 || typeVstup < 0) {
                cout << "Zly vstup!" << endl;
            }
            else {
                koniec = true;
            }
        }
        
        cout << "Zadajte nazov 'uzmenej jednotky', ktoru chcete vyhladat:" << endl;
        string textVstup = "";
        cin.ignore();
        getline(cin, textVstup);

        switch (typeVstup)
        {
            case 1:
                hasTypeTabulky(MUNICIPALITY, textVstup);
                break;
            case 2:
                hasTypeTabulky(DISTRICT, textVstup);
                break;
            case 3:
                hasTypeTabulky(REGION, textVstup);
                break;
        }

        coutToConsole();

        vstupVyhladavania = 0;
    }
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
        cout << "\nPre navrat stlacte '0'" << endl;
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
     
    Searcher<ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator,
        ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>>::PreOrderHierarchyIterator>
        ::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
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

    Searcher<ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator,
        ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>>::PreOrderHierarchyIterator>
        ::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
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

    Searcher<ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator,
        ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>>::PreOrderHierarchyIterator>
        ::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        return territorialUnit->getType() == type;
    });
}

void hasTypeTabulky(TerritorialUnitType type, string text) {

    filteredUnits = new TerritorialStructure();

    switch (type)
    {
    case REGION:
        Searcher <ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>::HashTableIterator,
            ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>::HashTableIterator>
            ::getTerritorialUnitsFromTable(app->getUnits()->regionTerrUnits, filteredUnits, text);
        break;
    case DISTRICT:
        Searcher <ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>::HashTableIterator,
            ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>::HashTableIterator>
            ::getTerritorialUnitsFromTable(app->getUnits()->districtTerrUnits, filteredUnits, text);
        break;
        break;
    case MUNICIPALITY:
        Searcher <ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>::HashTableIterator,
            ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>::HashTableIterator>
            ::getTerritorialUnitsFromTable(app->getUnits()->municipalityTerrUnits, filteredUnits, text);
        break;
        break;
    }
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
