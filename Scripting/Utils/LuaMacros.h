#ifndef LUAMACROS_H_INCLUDED
#define LUAMACROS_H_INCLUDED


// Funkcja udostepniona w skryptach lua
#define LUAF

// Funkcja udostepniona w skryptach lua, ktora wymaga uzycia sol::resolve
// nalezy uzyc jesli istnieja inne przeciazane funkcje o tej samej nazwie, ktore nie sa dostepne w lua
// jesli udostepnianych jest kilka przeciazanych funckji nalezy uzyc makra LUAF
#define LUAR

// Konstruktor udostepniony w skryptach lua - obecnie nieobslugiwane
#define LUAC

// Pole publiczne udostepnione w skryptach lua
#define LUAV


#endif // LUAMACROS_H_INCLUDED
