/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011-2012  Warren D Greenway

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include "pygmy_profile.h"
#include "gems.h"

void main( void )  
{ 
    sysInit();
    
    //
    streamEnableEcho( COM3 );
    streamEnableBackspace( COM3 );
    streamEnableActionChars( COM3 );
    //fileOpenResource( &fileFont, PYGMY_orbitron18 );
    //    guiSetFont( &fileFont, &fontOrbitron18 );
    guiSetColor( 0x00, 0x00, 0xFF );
    guiSetBackColor( 0xFF, 0xFF, 0xFF );
    //drawImage( 110, 0, &fileFont, 'A' - 32 );    
    //guiSetFontColor( 0x00, 0x00, 0xFF );
    //guiSetFontBackColor( 0xFF, 0xFF, 0xFF );
    //drawRect( 1, 1, 62, 62, 8, 0 );
    gemInit( );
    //gemDrawMenu();
    //lcdSetCursor( 0, 0 );
    //print( LCD, "Hello" );
    //if( fileOpen( &fileFont, "orbitron18", READ ) ){
    //    guiSetFont( &fileFont, &fontOrbitron18 );
        
        //guiSetFontColor( 0xFF, 0 , 0 );
        //drawImage( 110, 0, &fileFont, 'A' - 32 );
        
    //} else{
    //    print( COM3, "\rFile failed to open" );    
    //} // else
    // GSM Shield Test Code
    //streamEnableEcho( COM2 );
    
    //fileOpenResource( &imgBusy, (u8*)PYGMY_busy );
    
    //guiCreateSprite( &imgBusy, 56, 56, 10 );
    //drawPoly( uiPoly, 10, 0 );
    /*drawLine( 16, 32, 32, 16, 0 );
    drawLine( 32, 16, 64, 32, 0 );
    drawLine( 64, 32, 32, 64, 0 );
    drawLine( 32, 64, 16, 32, 0 );*/
    /*if( fileOpen( &pygmyImage, "ruby.pbm", READ ) ){
        drawImage( 100, 0, &pygmyImage, 0 );
    } else{
        print( COM3, "\rImage failed to open!" );
    } // else
    */
    
    //print( COM3, "\rChannel4 Voltage: %d",  adcSingleSample( A4 ) );
    //voltShieldInit( );
    //print( COM3, "\rChannel1 Voltage: %d\r> ", voltShieldGetVoltage( 1 ) );
    //print( COM3, "\rChannel2 Resistance: %d\r> ", voltShieldGetResistance( 2 ) );
    //print( COM3, "\rChannel3 V%d R%d\r> ", voltShieldGetVoltage( 3 ), voltShieldGetResistance( 3 ) );
    //print( COM3, "\rChannel4 V%d R%d\r> ", voltShieldGetVoltage( 4 ), voltShieldGetResistance( 4 ) );
    
    //print( COM3, "\rWiper0: %d", voltShieldGetGain( 0 ) );
    //shieldGetPressure();
   
    //pinAddSoftPWM( D1, 30, 50 );
    
    //pinPWM( LED1, 100, 75 );
    //pinPWM( LED0, 100, 50 );

    //rfInit();
    /*ulID = rfGetID();
    print( COM3, "\rRFID: %X", ulID );
    if( ulID == 0x41B40AA8 ){
        rfOpenSocket( 0x1FC60435, RF_COMLINK );
    } else {
        rfOpenSocket( 0x41B40AA8, RF_COMLINK );
    } // else
    rfPutString( "Hello World" );
    print( COM3, "\r> " );*/

    while( 1 ){ ; }
} 

void blink1( void )
{
    static s8 i = 0, cStep = 1;
    
    i += cStep;
    if( i > 80 ){
        cStep = -1;  
    } // if
    if( i < 1 ){
        cStep = 1;
    } // if
    pinPWM( LED0, 8000, i );
}

void blink2( void )
{
	if( pinGet( LED1 ) ){
		pinSet( LED1, LOW );
	} else {
		pinSet( LED1, HIGH );
	}
}
/*
void WWDG_IRQHandler (void) 
{
    print( COM3, "." );
}
void PVD_IRQHandler (void) 
{
    print( COM3, "." );
}
void TAMPER_IRQHandler (void)
{
    print( COM3, "." );
}
void RTC_IRQHandler (void) 
{
    print( COM3, "." );
}
void FLASH_IRQHandler (void) 
{
    print( COM3, "." );
}
void RCC_IRQHandler (void) 
{
    print( COM3, "." );
}

void USB_HP_CAN_TX_IRQHandler (void) 
{
    print( COM3, "." );
}
void USB_LP_CAN_RX0_IRQHandler (void) 
{
    print( COM3, "." );
}
void CAN_RX1_IRQHandler (void) 
{
    print( COM3, "." );
}
void CAN_SCE_IRQHandler (void) 
{
    print( COM3, "." );
}

void TIM1_BRK_IRQHandler (void)
{
    print( COM3, "BRK " );
}

void TIM1_CC_IRQHandler (void) 
{
    print( COM3, "CC " );
}
void TIM2_IRQHandler (void) 
{
    print( COM3, "." );
}
void TIM3_IRQHandler (void)
{
    print( COM3, "." );
}
void TIM4_IRQHandler (void)
{
    print( COM3, "." );
}
void I2C1_EV_IRQHandler (void) 
{
    print( COM3, "." );
}
void I2C1_ER_IRQHandler (void) 
{
    print( COM3, "." );
}
void I2C2_EV_IRQHandler (void) 
{
    print( COM3, "." );
}
void I2C2_ER_IRQHandler (void) 
{
    print( COM3, "." );
}
void SPI1_IRQHandler (void) 
{
    print( COM3, "." );
}
void SPI2_IRQHandler (void) 
{
    print( COM3, "." );
}

void RTCAlarm_IRQHandler (void) 
{
    print( COM3, "." );
}
void USBWakeUp_IRQHandler (void)
{
    print( COM3, "." );
}
void TIM8_BRK_IRQHandler (void) 
{
    print( COM3, "." );
}
void TIM8_UP_IRQHandler (void)
{
    print( COM3, "." );
}
void TIM8_TRG_COM_IRQHandler (void)
{
    print( COM3, "." );
}
void TIM8_CC_IRQHandler (void)
{
    print( COM3, "." );
}
void ADC3_IRQHandler (void)
{
    print( COM3, "." );
}
void FSMC_IRQHandler (void)
{
    print( COM3, "." );
}
void SDIO_IRQHandler (void)
{
    print( COM3, "." );
}
void TIM5_IRQHandler (void)
{
    print( COM3, "." );
}
void SPI3_IRQHandler (void) 
{
    print( COM3, "." );
}
void UART4_IRQHandler (void)
{
    print( COM3, "." );
}
void UART5_IRQHandler (void)
{
    print( COM3, "." );
}
void TIM6_IRQHandler (void)
{
    print( COM3, "." );
}
void TIM7_IRQHandler (void)
{
    print( COM3, "." );
}
void DMA2_Channel1_IRQHandler (void)
{
    print( COM3, "." );
}
void DMA2_Channel2_IRQHandler (void)
{
    print( COM3, "." );
}
void DMA2_Channel3_IRQHandler (void)
{
    print( COM3, "." );
}
void DMA2_Channel4_5_IRQHandler (void)
{
    print( COM3, "." );
}*/

