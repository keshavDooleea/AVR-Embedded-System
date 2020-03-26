////////////////////////////////////////////////////////////////////////////////
/// \file    del.cpp
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
///L'implementation du code pour controller la del..
////////////////////////////////////////////////////////////////////////////////

#include "del.h"

/**
 * On allume la del en \c ROUGE pendant la  \c duree desiree.
 * 
 * \param [in] duree
 *        La duree pour laquelle la del reste allumer.
 * 
 * .
 */

void allumerDelRouge(double duree) {

    // ici, PORT B toujours en sortie
    DDRB = 0xFF;
    
    // on allume PORT B 
    PORTB = ROUGE;

    // on fait allumer la DEL pendant une certaine durée
    delayMs(duree);

    eteindreDel();
}

/**
 * On allume la del en \c ROUGE .
 * 
 */
void allumerDelRouge() {
    // ici, PORT B toujours en sortie
    DDRB = 0xFF;
    
    // on allume PORT B 
    PORTB = ROUGE;
}

/**
 * On allume la del en \c VERT pendant la \c duree desiree.
 * 
 * \param [in] duree
 *        La duree pour laquelle la del reste allumer.
 * 
 * .
 */
void allumerDelVert(double duree) {

    // ici, PORT B toujours en sortie
    DDRB = 0xFF;

    // on allume PORT B 
    PORTB = VERT;

    // on fait allumer la DEL pendant une certaine durée
    delayMs(duree);

    eteindreDel();
}

/**
 * On allume la del en \c VERT .
 * 
 */
void allumerDelVert() {
    // ici, PORT B toujours en sortie
    DDRB = 0xFF;
    
    // on allume PORT B 
    PORTB = VERT;
}

/**
 * On allume la del en \c ROUGE puis en \c VERT pendant une \c duree 
 * desire pour obtenir la couleur Ambre.
 * 
 * \param [in] duree
 *        La duree pour laquelle la del reste allumer.
 * 
 * .
 */

void allumerDelAmbre(double duree) {

    // ici, PORT B toujours en sortie
    DDRB = 0xFF;

    // on fait allumer la DEL pendant une durée
    for (int i = 0; i < duree; i++) {
        PORTB = ROUGE;
        _delay_ms(1);
        PORTB = VERT;
        _delay_ms(1);
    }

    eteindreDel();
}

/**
 * On allume la del en \c Rouge puis en \c VERT .
 * 
 */
void allumerDelAmbre() {
    // ici, PORT B toujours en sortie
    DDRB = 0xFF;
    
    // on allume PORT B 
    for (;;) {
        PORTB = ROUGE;
        _delay_ms(1);
        PORTB = VERT;
        _delay_ms(1);
    }
}

/**
 * On eteint la del.
 * 
 */
void eteindreDel() {

    // ici, PORT B toujours en sortie
    DDRB = 0xFF;

    // on éteint PORT B 
    PORTB = OFF;
}