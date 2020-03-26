////////////////////////////////////////////////////////////////////////////////
/// \file    bouton.cpp
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
/// L'implementation du code pour gerer les interruptions.
////////////////////////////////////////////////////////////////////////////////

#include "bouton.h"

/**
 * Verifie si le bouton interupt a ete appuye.
 * 
 * \return Vrai si le bouton interupt a ete appuye.
 */
bool boutonAppuye() {

    // anti-rebond
    if (REBOND) {
         _delay_ms(10);

         if(REBOND) {
             return true;
        }
    }
    return false;
}

/**
 * Initialise les registres pour autoriser 
 * les interuptions du bouton blanc.
 * 
 * 
 */

void initialisationBoutonExterne() {
    cli(); // bloquer toutes les interruptions

	DDRD = 0X00; // PORTD est en mode entree
    
	EIMSK |= (1 << INT1) ; // permettre les interruptions externes
	EICRA |= (1 << ISC11); // niveau du bouton-poussoir est assigné à "falling edge"

	sei(); // permet de recevoir à nouveau des interruptions
}

/**
 * Initialise les registres pour autoriser les 
 * interuptions du bouton interupt.
 * 
 * 
 */
void initialisationBoutonInt() {
    cli(); // bloquer toutes les interruptions

	DDRD = 0X00; // PORTD est en mode entree
    
	EIMSK |= (1 << INT0) ; // permettre les interruptions externes
	EICRA |= (1 << ISC01) | (1 << ISC00); // niveau du bouton-poussoir est assigné à "falling edge"

	sei(); // permet de recevoir à nouveau des interruptions

}

