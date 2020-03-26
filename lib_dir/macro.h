////////////////////////////////////////////////////////////////////////////////
/// \file    macro.h
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
///Definition de toutes les macro utilise dans le code.
////////////////////////////////////////////////////////////////////////////////

#ifndef MACRO_H
#define MACRO_H

/// Valeur pour obtenir une del \c ROUGE.
#define ROUGE 0X01 
/// Valeur pour obtenir une del \c VERT.
#define VERT 0X02
/// Valeur pour assigner un port en mdoe \c SORTIE.
#define SORTIE 0XFF
/// Valeur pour eteindre la del.
#define ETEINT 0XFF
/// Valeur pour assigner un port en mdoe \c ENTREE.
#define ENTREE 0X00

// pour l'ecran
#define DEMO_DDR	DDRC /// `Data Direction Register' AVR occupe par l'aff.
#define DEMO_PORT	PORTC /// Port AVR occupe par l'afficheur

// pour les capteurs infrarouges

/// Signal recu par DS5 assigne a \c POS5.
#define POS5 (PINA & _BV(PA1)) 
/// Signal recu par DS4 assigne a \c POS4.
#define POS4 (PINA & _BV(PA2))
/// Signal recu par DS3 assigne a \c POS3.
#define POS3 (PINA & _BV(PA3)) 
/// Signal recu par DS2 assigne a \c POS2.
#define POS2 (PINA & _BV(PA4)) 
/// Signal recu par DS1 assigne a \c POS1.
#define POS1 (PINA & _BV(PA5))

#endif /* MACRO_H */