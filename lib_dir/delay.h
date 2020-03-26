////////////////////////////////////////////////////////////////////////////////
/// \file    delay.h
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
///Definition du code pour assigner les delais .
////////////////////////////////////////////////////////////////////////////////
#ifndef DELAY_H
#define DELAY_H
#define F_CPU 8000000UL

#include <util/delay.h>

void delayMs(double duree);
void delayUs(double duree);

#endif /* DELAY_H */