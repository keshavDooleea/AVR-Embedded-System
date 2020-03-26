////////////////////////////////////////////////////////////////////////////////
/// \file    ajustementPWM.cpp
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
/// L'implémentation du code pour controller les roues du robots.
////////////////////////////////////////////////////////////////////////////////

#include "ajustementPWM.h"

/**
 * Ajuste le PWM pour pouvoir faire tourner les roues du robot.
 * 
 * 
 * \param [in] vitesseA
 *        Vitesse de la roue \c droite.
 * 
 * \param [in] vitesseB
 *        Vitesse de la roue \c gauche.
 * 
 * \param [in] sens
 *        Direction du robot.
 * 
 * 
 */
void ajustementPWM(uint8_t vitesseA, uint8_t vitesseB, Direction sens)
{
    // PORT D en mode sortie
    DDRD = 0xFF;  

    // mise à un des sorties OC1A et OC1B sur comparaison
    OCR1A = vitesseA;
    OCR1B = vitesseB;

    switch (sens) {

        // avancer
        case avant:
            PORTD &= ~(1 << PD6) & ~(1 << PD7); // avancer
            break;
        
        // reculer
        case arriere:
            PORTD |= (1 << PD6) | (1 << PD7); // reculer
            break;

        // virage a gauche
        case gauche:
            PORTD &= ~(1 << PD6) & ~(1 << PD7); 
            OCR1A = vitesseA;
            OCR1B = 0;
            break;

        // virage a droite
        case droite:
            PORTD &= ~(1 << PD6) & ~(1 << PD7); 
            OCR1A = 0;
            OCR1B = vitesseB;
            break;
    }

    // compteur
    TCNT1 = 0;    

    // division d'horloge par 8 - implique une frequence de PWM fixe
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B |= (1 << CS11);
    TCCR1C = 0; 
}