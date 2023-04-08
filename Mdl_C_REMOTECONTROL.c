/*
 * File:   REMOTECONTROL.c
 * Author: Ronnie Eriksson
 *
 * Created on den 27 november 2021, 00:29
 */


#include <xc.h>
//#include <stdio.h>
//#include <stdarg.h>
//#include <math.h>

#include "Mdl_C_REMOTECONTROL.h"

#define _XTAL_FREQ 16000000

#define SR PORTAbits.RA4


unsigned int REMOTE_CODE_HI = 0;
unsigned int REMOTE_CODE_LO = 0;
unsigned char BIT_COUNT = 0;
unsigned char RS_FUNC_CODE;

//  =================== READ REMOTE CONTROL ROUTINE ============================

void READ_REMOTE_CONTROL(void)
{
    unsigned int TIMER_1;

RESTART:    
    while(SR == 0)
    {
        NOP();
    }
    
    TIMER_1 = 0;
    while((SR == 1) & (TIMER_1 <= 45))
    {
        __delay_us(100);
        TIMER_1++;
    }
    if(TIMER_1 >= 45)
    {
        return;
    }
    else if(TIMER_1 < 35)
    {
        return;
        goto RESTART;
    }

    while(SR == 0)
    {
        NOP();
    }

//  ------------------- DECODE REMOTE CONTROL ROUTINE --------------------------

    BIT_COUNT = 0;
    while(BIT_COUNT < 32)
    {
        TIMER_1 = 0;
        while(SR == 1) // & (TIMER_1 <= 20))
        {
            __delay_us(100);
            TIMER_1++;
        }
        if(TIMER_1 >= 100)
        {
            return;
        }
        else if(TIMER_1 >= 10)
        {
            if(BIT_COUNT < 16)
            {
                REMOTE_CODE_HI++;
                REMOTE_CODE_HI = REMOTE_CODE_HI << 1;
                BIT_COUNT++;
            }
            else
            {
                REMOTE_CODE_LO++;
                REMOTE_CODE_LO = REMOTE_CODE_LO << 1;
                BIT_COUNT++;
                
            }
        }
        else if(TIMER_1 >= 2)
        {
            if(BIT_COUNT < 16)
            {
                REMOTE_CODE_HI = REMOTE_CODE_HI << 1;
                BIT_COUNT++;
            }
            else
            {            
                REMOTE_CODE_LO = REMOTE_CODE_LO << 1;
                BIT_COUNT++;
            }  
        }
        while(SR == 0)
        {
            NOP();
        } 
    }
   //printf("%s%X%s", "   READ_PACKAGE : ", READ_PACKAGE, "\n\r\n\r" ); 
    //printf("%s%s", "CODE_HI_IS : ", "\n\r\n\r");
    //NOP();
    //printf("%s%s", "CODE_LO_IS : ", "\n\r\n\r");
    //NOP();
    //printf("%s%X%s", "   REMOTE CODE IS HI : ", REMOTE_CODE_HI, "\n\r\n\r");
    ///NOP();
    //printf("%s%X%s", "   REMOTE CODE IS LO : ", REMOTE_CODE_LO, "\n\r\n\r");
    //NOP();

    
//  =================== SET REMOTE CONTROL FUNCTION ROUTINE ====================

    if((REMOTE_CODE_HI == 0xAB54) & (REMOTE_CODE_LO == 0x718E)) // MMENU EXI - TV Power
    {
        RS_FUNC_CODE = 0x01;
    }
    else if((REMOTE_CODE_HI == 0xAB54) & (REMOTE_CODE_LO == 0x906E)) // MENU - FUNC
    {
        RS_FUNC_CODE = 0x02;
    }
    else if((REMOTE_CODE_HI == 0xAB54) & (REMOTE_CODE_LO == 0x10EE)) // TONE/ PHONE - - Channel -
    {
        RS_FUNC_CODE = 0x03;
    }
    else if((REMOTE_CODE_HI == 0xAB54) & (REMOTE_CODE_LO == 0x11EE))  // TONE/ PHONE + - Channel +      
    {  
        RS_FUNC_CODE = 0x04;
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0x708E)) // TAPE - Tape 2       
    {
        RS_FUNC_CODE = 0x05; 
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0xE51A)) // BBLUETOOTH - Tape 1
    {
        RS_FUNC_CODE = 0x06;
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0x659A)) // CD
    {
        RS_FUNC_CODE = 0x07;
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0x649A)) // PHONO       
    {
        RS_FUNC_CODE = 0x08;
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0xC43A)) // TUNER       
    {
        RS_FUNC_CODE = 0x09;
    }
    else if((REMOTE_CODE_HI == 0x4AB4)  & (REMOTE_CODE_LO == 0xE41A)) // LINE 1 - 3       
    {
        RS_FUNC_CODE = 0x0A;
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0xA05E)) // VOLUME -
    {
        RS_FUNC_CODE = 0x0B;
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0xA15E)) // VOLUME +       
    {
        RS_FUNC_CODE = 0x0C;
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0x718E)) // AMP POWER
    {
        RS_FUNC_CODE = 0x0D;
    }
    else if((REMOTE_CODE_HI == 0x4AB4) & (REMOTE_CODE_LO == 0x916E)) // BASS FREQUENCY SET
    {
        RS_FUNC_CODE = 0x0E;
    }
    else if((REMOTE_CODE_HI == 0x2BD4) & (REMOTE_CODE_LO == 0x847A)) // RX
    {
        RS_FUNC_CODE = 0x0F;
    }
    else if((REMOTE_CODE_HI == 0x2BD4) & (REMOTE_CODE_LO == 0x31CE)) // TX
    {
        RS_FUNC_CODE = 0x10;
    }
    else if((REMOTE_CODE_HI == 0x2BD4) & (REMOTE_CODE_LO == 0x946A)) // BACKWARDS
    {
        RS_FUNC_CODE = 0x11;
    }
        else if((REMOTE_CODE_HI == 0x2BD4) & (REMOTE_CODE_LO == 0x956A)) // FORWARDS
    {
        RS_FUNC_CODE = 0x12;
    }
    else if((REMOTE_CODE_HI == 0x2BD4) & (REMOTE_CODE_LO == 0x718E)) // BT PWR
    {
        RS_FUNC_CODE = 0x13;
    }
    else if((REMOTE_CODE_HI == 0x2BD4) & (REMOTE_CODE_LO == 0xD12E)) // PAIRING
    {
        RS_FUNC_CODE = 0x14;
    }
    else if((REMOTE_CODE_HI == 0x2BD4) & (REMOTE_CODE_LO == 0xD02E)) // PLAY/ PAUSE
    {
        RS_FUNC_CODE = 0x15;
    }
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x718E)) // BLUETOOTH CLEAR
    {
        RS_FUNC_CODE = 0x16;
    }



    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x946A)) // [DEC I]
    {
        RS_FUNC_CODE = 0x17;
    }      
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x956A)) // [DEC II]
    {
        RS_FUNC_CODE = 0x18;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x10EE)) // [TAPE FREW]
    {
        RS_FUNC_CODE = 0x19;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x11EE)) // [TAPE FF]
    {
        RS_FUNC_CODE = 0x1A;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x718E)) // DOESN'T WORK IN VIDEO MODE  [TAPE SELECT]
    {
        RS_FUNC_CODE = 0x1B;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x31CE)) // [TAPE PAUSE]
    {
        RS_FUNC_CODE = 0x1C;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x51AE)) // ONLY WORKS COMBINED WITH RED REC [TAPE REC BLACK]
    {
        RS_FUNC_CODE = 0x1D;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0x51AE)) // ONLY WORKS COMBINED WITH BLACK REC [TAPE REC RED]
    {
        RS_FUNC_CODE = 0x1E;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0xE01E)) // [TAPE PLAY BACKWARDS]
    {
        RS_FUNC_CODE = 0x1F;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0xD12E)) // [TAPE STOP]
    {
        RS_FUNC_CODE = 0x20;
    }  
    else if((REMOTE_CODE_HI == 0xAA54) & (REMOTE_CODE_LO == 0xD02E)) // [TAPE PLAY FORWARDS]
    {
        RS_FUNC_CODE = 0x21;
    }  


    
    else
    {
         RS_FUNC_CODE = 0xA0;
    }    
    
    __delay_ms(20);
    
    TMR1H = 0x00;
    TMR1L = (0xFF - 0x1B);    
    
    return;
}
