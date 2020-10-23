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
#include <string.h>
 
#define _XTAL_FREQ 8000000

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
 
uint8_t UART_GetC()
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



float grados,minutos;
float latitud,longitud;


char c_grados[10];   // grados en cadena

//   lo que se va a capturar se toma desde PGGA  HASTA *
//estados de la recepcion de variables
enum estados_  {cap_P,cap_G,cap_G2,cap_A,esp_final} ;
//estado inicial
 char estado_rec=cap_P;

//   lo que se va a capturar se toma desde lue=   value": 0.0}
float valor_rx;  //captura valor de lavariables
char trama_rx[70];
char captura_rx[70];
char texto[10]; // variable captura de cadeda auxiliar
char contador_trama=0;
unsigned long demora=0; //usado para alir de la lectura de datos
unsigned char item[10]; //Aux
unsigned short estado=0;


char lee_trama()   //funcio que retorna 1 si hay dato sino 0
{
char dato_rx;
//limpa todas las variables
memset(trama_rx,50,0);
demora=0;
contador_trama=0;
estado_rec=cap_P;
//borra errores de recepcion
RCIF=0;
CREN=0; //
CREN=1; //

while (demora<=96667)
{
   demora++;
   switch (estado_rec)
   {
   case cap_P:
             if (RCIF==1) //si hay un dat recibido
             {
               dato_rx=UART_GetC(); //lee el dato
               trama_rx[contador_trama]=dato_rx;
               if (dato_rx!='P') contador_trama=0;    //no fue el dato que me interesa
               else
               {
                  contador_trama++;//incrementa contador para proximo dato
                  estado_rec=cap_G; //pasa al estado de capturar la g
               }
              RCIF=0; //limpia la bandera de recepcion par esperar otro dato
             }
             break;
             
     case cap_G:
             if (RCIF==1) //si hay un dat recibido
             {
               dato_rx=UART_GetC(); //lee el dato
               trama_rx[contador_trama]=dato_rx;
               if (dato_rx!='G')
               {
               contador_trama=0;
               estado_rec=cap_P; //regresa al estado inical
               }
               else
               {
                  contador_trama++;//incrementa contador para proximo dato
                 estado_rec=cap_G2; //pasa al estado de capturar la g
               }
                RCIF=0; //limpia la bandera de recepcion par esperar otro dato
             }
             break;

   case cap_G2:
             if (RCIF==1) //si hay un dat recibido
             {
               dato_rx=UART_GetC(); //lee el dato
               trama_rx[contador_trama]=dato_rx;
               if (dato_rx!='G')
               {
               contador_trama=0;
               estado_rec=cap_P; //regresa al estado inical
               }
               else
               {
                  contador_trama++;//incrementa contador para proximo dato
                  estado_rec=cap_A; //pasa al estado de capturar la a
               }
              RCIF=0; //limpia la bandera de recepcion par esperar otro dato
             }
             break;


   case cap_A:
             if (RCIF==1) //si hay un dat recibido
             {
               dato_rx=UART_GetC(); //lee el dato
               trama_rx[contador_trama]=dato_rx;
               if (dato_rx!='A')
               {
               contador_trama=0;
               estado_rec=cap_P; //regresa al estado inical

               }
               else
               {
                  contador_trama++;//incrementa contador para proximo dato
                  estado_rec=esp_final; //pasa al estado de capturar la u
               }
               RCIF=0; //limpia la bandera de recepcion par esperar otro dato
             }

             break;

   case esp_final:
             if (RCIF==1) //si hay un dat recibido
             {
               dato_rx=UART_GetC(); //lee el dato
               trama_rx[contador_trama]=dato_rx;
               contador_trama++;
               if (dato_rx=='*')   return(1); //sale del procedimiento
             RCIF=0; //limpia la bandera de recepcion
             }
             break;


       }//fin del switch

   }//fin del while demora
   return(0);

}




void main(void) {
    
    ANSEL=ANSELH=0;
    
    TRISB  = 1;   
    PORTB  = 0;

    TRISCbits.TRISC6=0;             //Puertos para transmision EUSART TX RX
    TRISCbits.TRISC7=1;
    
    OSCCON = 0x70;              // set internal oscillator to 8MHz
    
    UART_Init(9600);  // initialize UART module with 9600 baud
    __delay_ms(2000);  // wait 2 seconds    

    LCD_Begin();    // initialize LCD module
    
    while(1) {
        
    if(RB0 == 1 || RB1 == 1){

        lee_trama();

        //captura la latitud
       strcpy(captura_rx, strtok(trama_rx, ","));  //inicia captura de tokens desde el =
       strcpy(captura_rx, strtok(0, ","));  //captura hasta segunda coma
        strcpy(captura_rx, strtok(0, ","));  //captura hasta lo que esta tercera coma es decir, latitud
           memset(c_grados,0,10);//limpía los grados
           strncpy(c_grados,captura_rx,2);//captura los grados
            grados=atof(c_grados);      // pasa de alfanumerico o cadena a flotante

              // apunta a los minutos
             minutos=atof(&captura_rx[2])/60.0;  //lo pasa a grados
               latitud=grados+minutos;

           strcpy(captura_rx, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la lat
           if (captura_rx[0]=='S') latitud=-latitud;
    //ACONDICIONA PARA EL LCD
            LCD_Goto(1, 1);
            LCD_Print("Latitud:");
            sprintf(texto,"%f",latitud);
                //BORRA  ESPACIA A ESCRIBIR
            LCD_Goto(1, 2);
            LCD_Print(texto);


      //  captura la longitud
           strcpy(captura_rx, strtok(0, ","));  //captura siguiente coma la longitud
         memset(c_grados,0,10);//limpía los grados
           strncpy(c_grados,captura_rx,3);//captura los grados  3 posiciones
              grados=atof(c_grados);
        // apunta a los minutos
             minutos=atof(&captura_rx[3])/60.0;  //lo pasa a grados
               longitud=grados+minutos;
           strcpy(captura_rx, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la long
           if (captura_rx[0]=='W') longitud=-longitud;
                LCD_Goto(1, 3);
                LCD_Print("Longitud:");
                sprintf(texto,"%f",longitud);
                LCD_Goto(1, 4);
                LCD_Print(texto);
            sprintf(item,"%f %f", latitud, longitud);
            UART_Print(item);
            UART_Print("\r\n");  // UART print message  
        }
    
    }
      
}
