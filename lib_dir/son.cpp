////////////////////////////////////////////////////////////////////////////////
/// \file    son.cpp
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani 
///
///L'implementation du code pour controller la son.
////////////////////////////////////////////////////////////////////////////////

#include "son.h"

/**
 * On initialise les registres pour declencher la minuterie 
 * en mode CTC et on assigne a l'OCR la \c note a jouer apres 
 * conversion grace a la formule .
 * 
 * \param [in] note
 *        Note MIDI.
 */

void jouerNote(int note) {
  DDRB = 0xFF;
  // conversion du numero de note vers la frequence 
  double frequence = (2^(( note - 69 ) / 12 ) * 440);

  //Convertion de la frequence en periode
  //On utilise la fonction: F =Fcpu/2*prescaler*(1+OCRnA) et on tire OCRnA
  //prescaler=256;Fcpu=8000000
  double periode = (F_CPU/(2*frequence*256)) - 1;
  OCR0A = periode;  //Valeur du top
  TCCR0A |= (1 << WGM01) | (1 << COM0A0);//Activer le toggle et le mode CTC
  TCCR0B |= (1 << CS02);        //on utilise un prescaleur a 256
  TCNT0 = 0;
}

/**
 * On verifie si la \c note  est jouable , puis la fait
 * jouer grace a la fonction \c jouerNote() .
 * 
 * \param [in] note
 *        Note MIDI.
 */
void trouverNote(int note) {
  //Verifie que la note qu'on veut jouer est bien dans le tableau
  if (note > 44 && note < 82)
  {
    //allumerDelVert(500);
    //On met -45 car il faut aller chercher le code dans le tableau qui commence a position 0
    //PWM(Hertztemps(note - 45), Periode(note - 45), Temps(note - 45));
    jouerNote(note);
  }
}

/**
 * On desactive les registres
 * pour arrerter le son.
 */
void arreterSon(){
    OCR0A = 0;  
    TCCR0A = 0;     
    TCCR0B = 0;        
    TCNT0  = 0;
}