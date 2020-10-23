/*
 * File:   Smart.c
 * Author: juradoceronfrancisco
 *
 * Created on 5 de octubre de 2020, 10:35 PM
 */

#pragma config CONFIG1 = 0x2CD4
#pragma config CONFIG2 = 0x0700
 
//LCD module connections
#define LCD_RS       RD0
#define LCD_EN       RD1
#define LCD_D4       RD2
#define LCD_D5       RD3
#define LCD_D6       RD4
#define LCD_D7       RD5
#define LCD_RS_DIR   TRISD0
#define LCD_EN_DIR   TRISD1
#define LCD_D4_DIR   TRISD2
#define LCD_D5_DIR   TRISD3
#define LCD_D6_DIR   TRISD4
#define LCD_D7_DIR   TRISD5
//End LCD module connections

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD_lib.h"


/********************** UART functions **************************/
void UART_Init(const uint32_t baud_rate)
{
  int16_t n = ( _XTAL_FREQ / (16 * baud_rate) ) - 1;
  
  if (n < 0)
    n = 0;
 
  if (n > 255)  // low speed
  {
    n = ( _XTAL_FREQ / (64 * baud_rate) ) - 1;
    if (n > 255)
      n = 255;
    SPBRG = n;
    TXSTA = 0x20;  // transmit enabled, low speed mode
  }
 
  else   // high speed
  {
    SPBRG = n;
    TXSTA = 0x24;  // transmit enabled, high speed mode
  }
 
  RCSTA = 0x90;  // serial port enabled, continues receive enabled
 
}
 
__bit UART_Data_Ready()
{
  return RCIF;  // return RCIF bit (register PIR1, bit 5)
}
 
char UART_GetC()
{
  while (RCIF == 0) ;  // wait for data receive
  if (OERR)  // if there is overrun error
  {  // clear overrun error bit
    CREN = 0;
    CREN = 1;
  }
  return RCREG;        // read from EUSART receive data register
}
 
void UART_PutC(const char data)
{
  while (TRMT == 0);  // wait for transmit shift register to be empty
  TXREG = data;       // update EUSART transmit data register
}
 
void UART_Print(const char *data)
{
  uint8_t i = 0;
  while (data[i] != '\0')
    UART_PutC (data[i++]);
}
/********************** end UART functions **************************/


void main(void) {
    
    unsigned char iteml[20];
    char datos[90];
    unsigned char cantidadMaximaPasjeros = 22;
    unsigned int totalPasajeros = 0;
    unsigned int cont=0;
    unsigned int valorPasaje=2000;
    long ingresoTotal=0;
    unsigned char cuposDisponibles =(cantidadMaximaPasjeros *70) /100;
    unsigned short estadoDatos=0;
    char dato_rx;
    
    ANSEL=ANSELH=0;
    
    TRISB  = 1;   
    PORTB  = 0;

    TRISCbits.TRISC6=0;             //Puertos para transmision EUSART TX RX
    TRISCbits.TRISC7=1;
    
    OSCCON = 0x70;              // set internal oscillator to 8MHz
    
    UART_Init(9600);  // initialize UART module with 9600 baud
    __delay_ms(2000);  // wait 2 seconds    

    LCD_Begin();       // initialize LCD module para que prenda inicialisar LCD
    
    
    while(1){
        LCD_Goto(1, 1);   //UBICAR EL CURSOR EN CULUMNA 1, FILA 1
        sprintf(iteml,"Fecha",""); //Mensaje
        LCD_Print(iteml);//imprimir
        
        LCD_Goto(12, 1);   //UBICAR EL CURSOR EN CULUMNA 12, FILA 1
        sprintf(iteml,"Hora",""); //Mensaje
        LCD_Print(iteml);//imprimir
        
        LCD_Goto(1, 2);   //UBICAR EL CURSOR EN CULUMNA 1, FILA 3
        sprintf(iteml,"Total Pasajeros: %2d",totalPasajeros); //Mensaje
        LCD_Print(iteml);//imprimir
        
        LCD_Goto(1, 3);   //UBICAR EL CURSOR EN CULUMNA 1, FILA 4
        sprintf(iteml,"Cupos: %2d",cuposDisponibles); //Mensaje
        LCD_Print(iteml);//imprimir
        
        LCD_Goto(1, 4);   //UBICAR EL CURSOR EN CULUMNA 1, FILA 4
        sprintf(iteml,"cot: %2d",cont); //Mensaje
        LCD_Print(iteml);//imprimir
      
        
        LCD_Goto(10, 4);   //UBICAR EL CURSOR EN CULUMNA 1, FILA 4
        sprintf(iteml,"dine: %8d",ingresoTotal); //Mensaje
        LCD_Print(iteml);//imprimir
      
        if(RA0 == 1){
            while(RA0 == 1){}
            if(cuposDisponibles!=0){
               totalPasajeros++;
               cuposDisponibles--;
               cont++;
               ingresoTotal=(cont*valorPasaje);
               sprintf(datos,"Datos: ingresoTotal %8d  cont %2d totalPasajeros %2d cuposDisponibles %2d",ingresoTotal, cont, cuposDisponibles, totalPasajeros); //Mensaje
               estadoDatos=1;  
            }   
        }
        if(RA1 ==1){
            while(RA1 == 1){}
            if(totalPasajeros!=0){
               totalPasajeros--;
               cuposDisponibles++;
               sprintf(datos,"Datos: ingresoTotal %8d  cont %2d totalPasajeros %2d cuposDisponibles %2d",ingresoTotal, cont, cuposDisponibles, totalPasajeros); //Mensaje
               estadoDatos=1;  
            }
        }
        
        if(estadoDatos){
            if ( RCIF == 1 ){  // if a character available
                char c = UART_GetC();  // read from UART and store in 'c'
                UART_PutC(c);  // send 'c' via UART (return the received character back)
            }
            UART_Print("\r\n");
            UART_Print(datos);
            UART_Print("\r\n");
            estadoDatos=0;
        }
    } 
}
