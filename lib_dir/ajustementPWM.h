////////////////////////////////////////////////////////////////////////////////
/// \file    ajustementPWM.h
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
/// Definitions du code et des structures pour controller les roues du robot.
////////////////////////////////////////////////////////////////////////////////

#ifndef AJUSTEMENTPWM_H
#define AJUSTEMENTPWM_H

#include <avr/interrupt.h>
#include <avr/io.h>

/**
 * \enum  Direction
 * \brief Direction des roues du Robot.
 * 
 * Nous permet de choisir la direction a laquelle les roues 
 * doivent toruner.
 */
enum Direction {
    avant,   //!<  Le robot roule en avant.
    arriere, //!<  le robot recule.
    gauche,  //!<  Le robot tourne a gauche.
    droite   //!<  Le robot tourne a droite.
};

// ajuster la vitesse des roues en initialisant les registresss
void ajustementPWM(uint8_t vitesseA, uint8_t vitesseB, Direction sens);

#endif /* AJUSTEMENTPWM_H */