////////////////////////////////////////////////////////////////////////////////
/// \file    del.h
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
///Definitions du code pour controller la del.
////////////////////////////////////////////////////////////////////////////////

#ifndef DEL_H
#define DEL_H

#include <avr/io.h>
#include <delay.h>

/// Valeur pour obtenir une del \c ROUGE.
#define ROUGE 0X01
/// Valeur pour obtenir une del \c VERT.
#define VERT 0X02
/// Valeur pour eteindre la del.
#define OFF 0X00

// Allumer la DEL en rouge pendant une durée ou a l'infini
void allumerDelRouge(double duree);
void allumerDelRouge();

// Allumer la DEL en vert pendant une durée ou a l'infini
void allumerDelVert(double duree);
void allumerDelVert();

// Allumer la DEL en Ambre pendant une durée ou a l'infini
void allumerDelAmbre(double duree);
void allumerDelAmbre();

// Éteindre la DEL
void eteindreDel();

#endif /* DEL_H */