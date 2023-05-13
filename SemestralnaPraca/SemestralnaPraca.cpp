#pragma once

#include <iostream>
#include <Windows.h>
#include "App/App.h"
#include "Services/Searcher.h"
#include "Services/Sorter.h"
#include <algorithm>
#include <libds/heap_monitor.h>
#include "Structures/Enums/TerritorialUnitTypes.h"
#include <libds/adt/sorts.h>

void mainMenu();
void vyhladavanieVHierarchii();
void vyhladavanie();
void vyhladanieVTabulkach();
void findInformation(int s);
void startsWith(string vstup);
void contains(string vstup);
void samePopulation();
void hasType(TerritorialUnitType type);
void hasTypeTabulky(TerritorialUnitType type, string text);
void coutToConsole(bool filtering);

void vypisAktPoziciu();

App* app;
ds::adt::ImplicitList<TerritorialUnit*>* filteredUnits;

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
        cout << "\nPre ukoncenie stlacte nulu '0'" << endl;
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

        cout << "\nZvolte akciu:" << endl;
        cout << "  -2. Vyhladajte v podstrome" << endl;
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
        cout << "\nZvolte z troch typov 'uzemnych jednotiek' ktore chcete vyhladat:" << endl;
        cout << "  1. Obec" << endl;
        cout << "  2. Okres" << endl;
        cout << "  3. Kraj'" << endl;

        bool koniec = false;
        int typeVstup = -1;

        while (!koniec) {
            cin >> typeVstup;
            if (typeVstup > 0 && typeVstup <= 3) {
                system("cls");
                koniec = true;
            } else if (typeVstup == 0) {
                system("cls");
                return;
            } else {
                cout << "Zly vstup!" << endl;
            }
        }
        
        cout << "\nZadajte nazov 'uzmenej jednotky', ktoru chcete vyhladat:" << endl;
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

        coutToConsole(false);

        vstupVyhladavania = 0;
    }
}

void vyhladavanie() {
    vypisAktPoziciu();
    int vstupVyhladavnia = -1;

    while (vstupVyhladavnia != 0) {
        cout << "\nPre navrat stlacte '0'" << endl;
        cout << "Zadajte akciu (Vyhladat informacie): " << endl;
        cout << "  1. startsWith" << endl;
        cout << "  2. cotains" << endl;
        cout << "  3. hasType" << endl;
        cout << "  4. hasSamePopulation ( 2022 - 2021 )" << endl;

        cin >> vstupVyhladavnia;
        system("cls");

        if (vstupVyhladavnia > 4) {
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
        if (s == 4) {
            samePopulation();
        } else if (s != 3) {
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
        } else {
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

        coutToConsole(true);

        vstup = "q";
    }
}

void vypisAktPoziciu() { 
    cout << "Aktualna pozicia: " << app->getIterator()->getPosition()->data_->getOfficialTitle() 
        << " | Rozdiel obyv. ( 2022 - 2021 ): " << app->getIterator()->getPosition()->data_->getDifferenceOfPopulation() << endl;
}

void startsWith(string vstup) {

    filteredUnits = new ds::adt::ImplicitList<TerritorialUnit*>();

    // Zacina na znak
    //https://www.techiedelight.com/check-if-a-string-starts-with-a-certain-string-in-cpp/
     
    Searcher<ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator,
        ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>>::PreOrderHierarchyIterator>
        ::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        string name = territorialUnit->getShortTitle();

        Sorter::toLowerCase(name.begin(), name.end());

        return name.rfind(vstup, 0) == 0;
    });
}

void contains(string vstup) {

    filteredUnits = new ds::adt::ImplicitList<TerritorialUnit*>();

    // Coitains
    // https://www.techiedelight.com/check-if-a-string-contains-another-string-in-cpp/

    Searcher<ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator,
        ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>>::PreOrderHierarchyIterator>
        ::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        string name = territorialUnit->getShortTitle();

        Sorter::toLowerCase(name.begin(), name.end());

        return name.find(vstup) != string::npos;
    });
}

void samePopulation() {
    filteredUnits = new ds::adt::ImplicitList<TerritorialUnit*>();

    Searcher<ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator,
        ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>>::PreOrderHierarchyIterator>
        ::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        
        return territorialUnit->getDifferenceOfPopulation() == 0;
    });
}

void hasType(TerritorialUnitType type) {

    filteredUnits = new ds::adt::ImplicitList<TerritorialUnit*>();

    // Ma typ uzmenej jednotky podla Enumu 

    Searcher<ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator,
        ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>>::PreOrderHierarchyIterator>
        ::getTerritorialUnitsByString(app->getIterator()->begin(), app->getHierarchy()->end(), filteredUnits, [&](TerritorialUnit* territorialUnit) {
        
        return territorialUnit->getType() == type;
    });
}

void hasTypeTabulky(TerritorialUnitType type, string text) {

    filteredUnits = new ds::adt::ImplicitList<TerritorialUnit*>();

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
    case MUNICIPALITY:
        Searcher <ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>::HashTableIterator,
            ds::adt::HashTable<string, ds::adt::ImplicitList<TerritorialUnit*>*>::HashTableIterator>
            ::getTerritorialUnitsFromTable(app->getUnits()->municipalityTerrUnits, filteredUnits, text);
        break;
    }
}

void coutToConsole(bool filtering) {

    ds::amt::ImplicitSequence<TerritorialUnit*>* sequence = new ds::amt::ImplicitSequence<TerritorialUnit*>();

    for (TerritorialUnit* unit : *filteredUnits) {
        sequence->insertLast().data_ = unit;
    }

    if (filtering) {
        int typeOfCout = 0;
        int ascChoose = 1;

        cout << "Zadajte typ triedenia: " << endl;
        cout << "  1. Utriedit 'compareAlphabetical'" << endl;
        cout << "  2. Utriedit 'compareVowelsCount'" << endl;
        cin >> typeOfCout;

        cout << "Vzostupne alebo Zostupne: " << endl;
        cout << "  1. Vzostupne" << endl;
        cout << "  2. Zostupne" << endl;
        cin >> ascChoose;

        switch (typeOfCout) {
        case 1:
            Sorter::compareAlphabetical(sequence, ascChoose == 1);
            break;
        case 2:
            Sorter::compareVowelsCount(sequence, ascChoose == 1);
            break;
        }
    }

    int i = 1;
    system("cls");
    cout << "\n" << endl;

    for (TerritorialUnit* filUnit : *sequence) {
        cout << i << ". | " << filUnit->toString() << endl;
        i++;
    }

    delete filteredUnits;
    delete sequence;
}
