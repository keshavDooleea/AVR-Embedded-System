////////////////////////////////////////////////////////////////////////////////
/// \file    son.cpp
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
///Definition du code pour controller la son.
////////////////////////////////////////////////////////////////////////////////

#ifndef SON_H
#define SON_H

#include <avr/interrupt.h>
#include <avr/io.h>

// jouer le son selon la note
void jouerNote(int note);

// assurer que c'est la bonne note
void trouverNote(int note);

// arreter le son
void arreterSon();

#endif /* SON_H */