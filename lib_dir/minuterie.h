////////////////////////////////////////////////////////////////////////////////
/// \file    minuterie.h
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
///Definition du code pour controller la minuterie.
////////////////////////////////////////////////////////////////////////////////

#ifndef MINUTERIE_H
#define MINUTERIE_H

#include <avr/io.h>
#include <avr/interrupt.h>

// faire partir la minuterie
void lancerMinuterie(uint16_t duree);

// arreter la minuterie
void arreterMinuterie();

//partir le timer 2
void partirMinuterie2();

//arreter le timer 2
void arreterMinuterie2();

#endif /* MINUTERIE_H */