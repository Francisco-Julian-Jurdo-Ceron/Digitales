/*
 * File:   system.c
 * Author: juradoceronfrancisco
 *
 * Created on 17 de octubre de 2020, 07:33 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>        // include stdint header
#include <pic.h>

//--------------------------------Keypad port-----------------------------------
#define keypadPort     PORTB
#define KeypadPort_dir TRISB
//--------------------------------4 Rows----------------------------------------
#define row0 RB0
#define row1 RB1
#define row2 RB2
#define row3 RB3

//--------------------------------3 Columns-------------------------------------
//NB: RD7 is a "Don't care" bit because it is unused.
/*These codes are used for scanning the columns of the keypad for a Press.
  A Press = LOW (For this design. Alternatively, HIGH can be used if so wished)*/
char column0 = 0x60;              // Column 0 LOW: RD6 = 1 , RD5 = 1 , RD4 = 0
char column1 = 0x50;              // Column 1 LOW: RD6 = 1 , RD5 = 0 , RD4 = 1
char column2 = 0x30;              // Colmun 2 LOW: RD6 = 0 , RD5 = 1 , RD4 = 1

#include "conf.h"
#include "delay.h"
#include "kPad4x3.h"
#include "lcd_lib.h"


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


char VAL = 0 ; //int8==unsigned
int posNum=1;
long num = 0, cupo, capacidadMax, valorp ;
unsigned char item[10];

void getKey() {
    VAL = keypad_scan();
    num = VAL;
}

void clear_lcd(){
    LCD_Goto(1, 1);
    LCD_Print("                     ");
    LCD_Goto(1, 2);
    LCD_Print("                    ");
    LCD_Goto(1, 3);
    LCD_Print("                    ");
    LCD_Goto(1, 4);
    LCD_Print("                    ");
}

void main(void) {
    
    unsigned char item1[20];
    ANSEL=ANSELH=0;
    
    TRISB  = 1;   
    PORTB  = 0;
    
    TRISCbits.TRISC6=0;             //Puertos para transmision EUSART TX RX
    TRISCbits.TRISC7=1;
    
    OSCCON = 0x70;              // set internal oscillator to 8MHz
    
    UART_Init(9600);  // initialize UART module with 9600 baud
    __delay_ms(2000);  // wait 2 seconds    

    
    LCD_Begin();    // initialize LCD module
    keypad_init();
    
    
    while(1) {

        LCD_Goto(1, 1);
        LCD_Print("System");
        LCD_Goto(1, 2);
        LCD_Print("1.Conf. cupo");
        LCD_Goto(1, 3);
        LCD_Print("2.Conf. capacidad maxima");
        LCD_Goto(1, 4);
        LCD_Print("3.Conf. valor pasaje");  
        
        getKey();
        if(num==1){
            clear_lcd();
            LCD_Goto(1, 1);
            LCD_Print("Ingrese numero cupo");        
            getKey();
            while(num != 11){
                getKey();
                switch(posNum){
                    case 1: 
                        if(num!=32){
                            getKey();
                            sprintf(item1,"%1d",num);
                            LCD_Goto(1, 2);
                            LCD_Print(item1);
                            posNum++;
                        }
                        break;
                    case 2:
                        if(num!=32){
                            getKey();
                            sprintf(item1,"%1d",num);
                            LCD_Goto(2, 2);
                            LCD_Print(item1);
                            posNum=1;
                        }
                        break;       
                }
            }
            
        }
        if(num==2){
            clear_lcd();
            LCD_Goto(1, 1);
            LCD_Print("Ingrese numero ");
        }
         if(num==3){
            clear_lcd();
            LCD_Goto(1, 1);
            LCD_Print("Ingrese capacidad maxima");
        }
        
         if ( RCIF == 1 ){  // if a character available
                //int32_t c = UART_GetC();  // read from UART and store in 'c'
                //UART_PutC(c);  // send 'c' via UART (return the received character back)
         }
        
    }
}
