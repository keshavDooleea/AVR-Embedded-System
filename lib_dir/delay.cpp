////////////////////////////////////////////////////////////////////////////////
/// \file    delay.h
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
/// L'implementation du code pour assigner les delais .
////////////////////////////////////////////////////////////////////////////////

#include "delay.h"

/**
 * On effectue un delay d'une \c duree desiree en ms.
 * 
 * \param [in] duree
 *        La duree pour laquelle le delay est effectue.
 * .
 */
void delayMs(double duree) {
    for (int i = 0; i < duree; i++) {
        _delay_ms(1);
    }    
}

/**
 * On effectue un delay d'une \c duree desiree en us.
 * 
 * \param [in] duree
 *        La duree pour laquelle le delay est effectue.
 * .
 */
void delayUs(double duree) {
    for (int i = 0; i < duree; i++) {
        _delay_us(1);
    }    
}