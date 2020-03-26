////////////////////////////////////////////////////////////////////////////////
/// \file    bouton.h
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
/// Definitions du code pour gerer les interruptions .
////////////////////////////////////////////////////////////////////////////////

#ifndef BOUTON_H
#define BOUTON_H

#define REBOND (PIND & 0X04)
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// verifier l'anti-rebond
bool boutonAppuye();

void initialisationBoutonExterne();

void initialisationBoutonInt();

#endif /* BOUTON_H */
