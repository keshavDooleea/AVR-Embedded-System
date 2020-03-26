////////////////////////////////////////////////////////////////////////////////
/// \file    minuterie.cpp
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
///L'implementation  du code pour controller la minuterie.
////////////////////////////////////////////////////////////////////////////////

#include "minuterie.h"

/**
 * On initialise les registres pour declencher la minuterie 
 * pendant la \c duree desiree.
 * 
 * \param [in] duree
 *        Duree de la minuterie.
 */
void lancerMinuterie(uint16_t duree) {
	
    TCNT1 = 0;
	OCR1A = duree; 

	TCCR1A |= (1 << COM1A1) | (1 << COM1A0);	
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);	
	TCCR1C = 0; 	
	TIMSK1 |= (1 << OCIE1A); 
}

/**
 * On initialise les registres pour declencher la minuterie 
 * en mode CTC pendant la \c duree desiree.
 * 
 * \param [in] duree
 *        Duree de la minuterie.
 */
void lancerMinuterieCTC(uint16_t duree) {
	/* mode CTC du timer 1 avec horloge divisée par 1024
       interruption après la durée spécifiée */
    TCNT1 = 0;
	OCR1A = duree; 

	TCCR1A |= (1 << COM1A1);	
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);	
	TCCR1C = 0; 	
	TIMSK1 |= (1 << OCIE1A); 
}

/**
 * On arrete la minuteire en desactivant les registres.
 */
void arreterMinuterie() {
    // désactiver tous les registres pour stopper la minuterie
	OCR1A  = 0;	
	TCCR1A = 0;
    TCCR1B = 0;
	TCCR1C = 0; 	
	TIMSK1 = 0;
}

/**
 * On initialise les registres pour 
 * declencher la minuterie du timer 2.
 * 
 */
void partirMinuterie2()
{   
    TCCR2A = 0; // on set le registre TCCR2A a 0
    TCCR2B = 0; // pareil pour TCCR2B
    TCNT2  = 0; //on initialise la valeur du compteur a 0
    OCR2A = 255; 
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // prescaler de 1024
}

/**
 * On arrete la minuteire du timer 2
 * en desactivant les registres.
 */
void arreterMinuterie2() { 
    // désactiver tous les registres pour stopper la minuterie
    OCR2A  = 0;
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
}