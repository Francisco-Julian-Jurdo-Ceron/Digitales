/* 
 * File:   delay.h
 * Author: juradoceronfrancisco
 *
 * Created on 11 April 2016, 1:35 PM
 */

#ifndef DELAY_H
//------------------------------------------------------------------------------
void delay(unsigned int nMilliseconds)
{
  #define CYCLES_PER_MS 100 /* Número de ciclos de prueba y decremento. */
  unsigned long nCycles = nMilliseconds * CYCLES_PER_MS;
  while (nCycles--);
}
//------------------------------------------------------------------------------
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */

