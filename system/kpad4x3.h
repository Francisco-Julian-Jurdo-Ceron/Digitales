/* 
 * File:   kPad4x3.h
 * Author: juradoceronfrancisco
 *
 * Created on 11 April 2016, 1:29 PM
 */

#ifndef KPAD4X3_H
//------------------------------------------------------------------------------
/*
  Nibble superior (conectado a las columnas del teclado) = SALIDA,
 *Nibble inferior (conectado a las filas del teclado) = ENTRADA */
void keypad_init()
{
  KeypadPort_dir = 0x0F;
}
//------------------Escanee con un 0 desde RD4 hasta RD7--------------------------
char keypad_scan()
{
  char key = ' ';    //La tecla presionada se utiliza como índice para la salida.

  keypadPort = column0;
     if( row0 == 0 ) key = 1;
     if( row1 == 0 ) key = 4;
     if( row2 == 0 ) key = 7;
     if( row3 == 0 ) key = 10;

  keypadPort = column1;
     if( row0 == 0 ) key = 2;
     if( row1 == 0 ) key = 5;
     if( row2 == 0 ) key = 8;
     if( row3 == 0 ) key = 0;

  keypadPort = column2;
     if( row0 == 0 ) key = 3;
     if( row1 == 0 ) key = 6;
     if( row2 == 0 ) key = 9;
     if( row3 == 0 ) key = 11;

  return key;
}
//------------------------------------------------------------------------------
#define	KPAD4X3_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* KPAD4X3_H */