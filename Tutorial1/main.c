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
#include "icons/busy.h"
#include "ruby.h"
#include "emerald.h"
#include "topaz.h"
#include "ytopaz.h"

PYGMYFILE imgBusy, imgRuby, imgEmerald, imgTopaz, imgYTopaz;
u16 globalBusyCoords[]      = {48, 48, 0, 0};
u16 globalRubyCoords[]      = { 0, 0, 16, 0, 32, 0, 48, 0, 64, 0, 80, 0, 96, 0, 112, 0,
                                0, 16, 16, 16, 32, 16, 48, 16, 64, 16, 80, 16, 96, 16, 112, 16};
 u16 globalEmeraldCoords[]  = { 0, 32, 16, 32, 32, 32, 48, 32, 64, 32, 80, 32, 96, 32, 112, 32,
                                0, 48, 16, 48, 32, 48, 48, 48, 64, 48, 80, 48, 96, 48, 112, 48};
u16 globalTopazCoords[]     = { 0, 64, 16, 64, 32, 64, 48, 64, 64, 64, 80, 64, 96, 64, 112, 64,
                                0, 80, 16, 80, 32, 80, 48, 80, 64, 80, 80, 80, 96, 80, 112, 80};
u16 globalYTopazCoords[]    = { 0, 96, 16, 96, 32, 96, 48, 96, 64, 96, 80, 96, 96, 96, 112, 96,
                                0, 112, 16, 112, 32, 112, 48, 112, 64, 112, 80, 112, 96, 112, 112, 112};

void main( void )  
{ 
    PYGMYFILE pygmyImage;
    //PYGMYI2CPORT pygmyI2C;
    //u16 uiPoly[] = { 16, 32, 32, 16, 64, 32, 32, 64, 16, 32 };

    //u32 ulID;
    sysInit();
   
    streamEnableEcho( COM3 );
    streamEnableBackspace( COM3 );
    streamEnableActionChars( COM3 );
    // GSM Shield Test Code
    //streamEnableEcho( COM2 );
    
    //fileOpenResource( &imgBusy, (u8*)PYGMY_busy );
    fileOpenResource( &imgRuby, (u8*)PYGMY_ruby );
    fileOpenResource( &imgEmerald, (u8*)PYGMY_emerald );
    fileOpenResource( &imgTopaz, (u8*)PYGMY_topaz );
    fileOpenResource( &imgYTopaz, (u8*)PYGMY_ytopaz );
    guiInitSprites();
    //guiCreateSprite( &imgBusy, 1, globalBusyCoords, 2, 100, 0 );
    guiCreateSprite( &imgRuby, 2, globalRubyCoords, 32, 100, 0 );
    guiCreateSprite( &imgEmerald, 3, globalEmeraldCoords, 32, 100, 0 );
    guiCreateSprite( &imgTopaz, 4, globalTopazCoords, 32, 100, 0 );
    guiCreateSprite( &imgYTopaz, 5, globalYTopazCoords, 32, 100, 0 );
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

