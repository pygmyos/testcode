/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011-2012  Warren D Greenway

    This file is part of PygmyOS.

    PygmyOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PygmyOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PygmyOS.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/
#include <malloc.h>
#include <stdlib.h>
#include <math.h> 
#include "pygmy_profile.h"
#include "fonts/orbitron18.h"
#include "gems.h"
#include "picon.h"

#include "profiles/nebula/shields/gasSensor.h"
#include "profiles/digipots/mcp443x.h"
#include "profiles/eeprom/at24hc02b.h"

PYGMYFILE fileFont;
PYGMYFONT fontOrbitron18;
void drawMainMenu( void );
void drawRFTest( void );
void eventRFBack( void );
void threadRFHumidity( void );
void threadRFHumiditySave( void );

void drawLogoCanvas( void );
void drawLogoForm( void );

void volumeSet( void );

u8 globalHumidity[ 20 ];
PYGMYI2CPORT globalGasSensorDigipot;

void main( void ) 
{
    u8 i, ucChar, *ucEeproms, ucBuffer[ 40 ];
 
    sysInit();
    //taskNewSimple( "volume", 5000, (void*)volumeSet );
    //taskNew( "volume", 5000, 20000, 0, (void*)volumeSet );
    /*socketInit();
    rfInit();
    guiInitSprites();
    print( COM3, "\rV2 RFID: 0x%X", socketGetID());
    
    
    fileOpenResource( &fileFont, (u8*)PYGMY_orbitron18 );
    fontLoad( &fileFont, &fontOrbitron18 );
    fontSetAll( &fontOrbitron18 );
    
    drawLogoCanvas();
    delay( 300000 );

    pinConfig( A0, PULLUP );
    pinConfig( D3, PULLUP );
    pinConfig( TA0, PULLUP );
    pinConfig( DAC1, PULLUP ); 
    pinConfig( DAC2, PULLUP );
    pinConfig( TX2, PULLUP );
    pinConfig( RX2, PULLUP );
    pinInterrupt( eventMouseMoveUp, A0, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveDown, D3, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveLeft, TA0, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveRight, DAC1, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickLeft, DAC2, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickCenter, TX2, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickRight, RX2, TRIGGER_RISING, 1 );
    drawMainMenu();
    */
    
    //mpl115a2Init( TX1, RX1, NONE, NONE );
    //print( LCD, "\rPressure: %f", mpl115a2ReadkPa() );
    //print( COM3, "\rTemp: %f", mpl115a2ReadTemp() );
    //i2cConfig( &globalGasSensorDigipot, DIGIPOT_BASEADDRESS|3, TX1, RX1, I2CSPEEDFAST );
    //digipotSetWiper( &globalGasSensorDigipot, 2, 0 );
    //eepromOpen( 0x2F, TX1, RX1, NONE );
    //eepromPutString( 0, "Test" );
    //pinConfig( MCO, OUT );
    //pinSet( MCO, HIGH );
    //gasSensorInit();
    //print( COM3, "\rGas Sensor: %f", gasSensorRead() );
    //gasSensorSetGain( 0,0 );
    //print( COM3, "\rGas Sensor: %f", gasSensorRead() );
    //gasSensorSetGain( 200,200 );
    //print( COM3, "\rGas Sensor: %f", gasSensorRead() );
    //eepromGetBuffer( 0, ucBuffer, len( "test" ) );
    //print( COM3, "\rEeprom: ucBuffer );
    //print( COM3, "\rEEPROM Direct Read: " );
    /*for( i = 0; i < len( "test" ); i++ ){
        ucChar = eepromGetChar( i );
        if( isAlphaOrNumeric( ucChar ) ) {
            print( COM3, "%c", ucChar );
        } else{
            print( COM3, "(%d)", ucChar  );//ucBuffer, len( "humidity" ) );
        } // else
    } // for
    */
    /* 
    // Test for LCD code
    if( isStringSame( ucBuffer, "<shield lcd>" ) ){
        print( COM3, "\rLCD Shield Detected" );
        fileOpenResource( &fileFont, (u8*)PYGMY_orbitron18 );
        fontLoad( &fileFont, &fontOrbitron18 );
        fontSetAll( &fontOrbitron18 );
    
        pinConfig( A0, PULLUP );
        pinConfig( D3, PULLUP );
        pinConfig( TA0, PULLUP );
        pinConfig( DAC1, PULLUP );
        pinConfig( DAC2, PULLUP );
        pinConfig( TX2, PULLUP );
        pinConfig( RX2, PULLUP );
        drawMainMenu();
    } else{
        hih5030Init( A2, A3 );
    } // else
    */
    
    //taskNewSimple( "humidity", 1000, (void*)threadRFHumiditySave );
    
    //gemInit();
    //fileOpen( &pygmyFile, "picon128", READ );
    //drawImage( 0,0, &pygmyFile, 0 );
    /*ucBuffer = sysAllocate( 5 );
    copyString( "Test", ucBuffer );
    ucBuffer = sysReallocate( ucBuffer, 20 );
    appendString( "ThisToo", ucBuffer );
    print( COM3, "\rTest String: %s", ucBuffer );*/
    //pinConfig( SHIELD_CENTER, PULLUP );
    //pinInterrupt( gemClickMenu, SHIELD_CENTER,  TRIGGER_RISING|TRIGGER_FALLING );
    //drawRect( 0, 20, 127, 40, VISIBLE|BORDER|ROUNDED, 8); 
    //gemDrawMenu();
    //drawWidget( &btnOK );
    //gemInit();
     
    
    //sysFree( ucBuffer );
    //fileOpen( &pygmyFile, "jpeg", READ );
    //drawJPEG( &pygmyFile, 0, 0 );
    //drawPNG( &pygmyFile, 0, 0 ); 
    /*mstatsTemp mstats();
    print( COM3, "\rbytes_total: %d", bytes_total );
    print( COM3, "\rchunks_used: %d", chunks_used );
    print( COM3, "\rbytes_used: %d", bytes_used );
    print( COM3, "\rchunks_free: %d", chunks_free );
    print( COM3, "\rbytes_free: %d", bytes_free );*/
    while(1){;}
}

void volumeSet( void )
{
    PYGMYI2CPORT pygmyI2C;
    
    i2cConfig( &pygmyI2C, TX1, RX1, 0x49, I2CSPEEDFAST );
    i2cStart( &pygmyI2C );
    i2cWriteByte( &pygmyI2C, pygmyI2C.Address );
    i2cWriteByte( &pygmyI2C, 44 );
    i2cStop( &pygmyI2C );
    print( COM3, "\rSet Volume" );
}

void drawReset( void )
{
    PYGMY_RESET;
}

void drawLogoCanvas( void )
{
    PYGMYFILE imageFile;
    //PYGMYCOLOR pygmyColor = { 255, 255, 255 };

    //guiClearArea( &pygmyColor, 0, 0, 127, 127 );
    //if( fileOpen( &imageFile, "picon.pbm", READ ) ){
    fileOpenResource( &imageFile, (u8*)PYGMY_picon );
    drawImage( 0, 0, &imageFile, 0 ); 
    //} //else{
    //    print( COM3, "\rFile picon.pbm missing" );
    //} // else
}       

void drawFormJerboa( void )   
{
    PYGMYFILE imageFile;
    PYGMYWIDGET widgetButton; 

    formFreeAll();
    formNew( 0, 0, 128, 128 );
    widgetButton.Type = CANVAS;
    widgetButton.X = 0;
    widgetButton.Y = 0;
    widgetButton.Width = 128;
    widgetButton.Height = 128;
    widgetButton.String = ""; 
    formAddWidget( &widgetButton );
    widgetAddEventHandler( widgetGetCurrent(), drawMainMenu, SELECTED );
    
    if( fileOpen( &imageFile, "jerboa.pbm", READ ) ){
        drawImage( 0, 0, &imageFile, 0 ); 
    } else{
        print( COM3, "\rFile jerboa.pbm missing" );
    } // else
    
}


void drawFormPeng( void )   
{
    PYGMYFILE imageFile;
    PYGMYWIDGET widgetButton; 

    
    formFreeAll();
    formNew( 0, 0, 128, 128 );
    widgetButton.Type = CANVAS;
    widgetButton.X = 0;
    widgetButton.Y = 0;
    widgetButton.Width = 128;
    widgetButton.Height = 128;
    widgetButton.String = ""; 
    formAddWidget( &widgetButton );
    widgetAddEventHandler( widgetGetCurrent(), drawFormJerboa, SELECTED );
    
    if( fileOpen( &imageFile, "peng.pbm", READ ) ){
        drawImage( 0, 0, &imageFile, 0 ); 
    } else{
        print( COM3, "\rFile peng.pbm missing" );
    } // else
    
}


void drawToucanForm( void )   
{
    PYGMYFILE imageFile;
    PYGMYWIDGET widgetButton; 

    formFreeAll();
    formNew( 0, 0, 128, 128 );
    widgetButton.Type = CANVAS;
    widgetButton.X = 0;
    widgetButton.Y = 0;
    widgetButton.Width = 128;
    widgetButton.Height = 128;
    widgetButton.String = ""; 
    formAddWidget( &widgetButton );
    widgetAddEventHandler( widgetGetCurrent(), drawFormPeng, SELECTED );
    
    if( fileOpen( &imageFile, "toucan.pbm", READ ) ){
        drawImage( 0, 0, &imageFile, 0 ); 
    } else{
        print( COM3, "\rFile toucan.pbm missing" );
    } // else
    
}

void drawLogoForm( void )   
{
    PYGMYFILE imageFile;
    PYGMYWIDGET widgetButton; 

    formFreeAll();
    formNew( 0, 0, 128, 128 );
    widgetButton.Type = CANVAS;
    widgetButton.X = 0;
    widgetButton.Y = 0;
    widgetButton.Width = 128;
    widgetButton.Height = 128;
    widgetButton.String = ""; 
    formAddWidget( &widgetButton );
    widgetAddEventHandler( widgetGetCurrent(), drawToucanForm, SELECTED );
    
    fileOpenResource( &imageFile, (u8*)PYGMY_picon );
    drawImage( 0, 0, &imageFile, 0 ); 
}

void drawMainMenu( void )
{
    PYGMYCOLOR pygmyColor;
    PYGMYFILE pygmyFile;
    PYGMYWIDGET widgetButton;

    colorSetRGB( colorGetRootColor(), 0x65, 0xBE, 0xFF ); //0x48, 0x8E, 0xDF );
    colorSetRGB( colorGetRootBackColor(), 0x65, 0x65, 0x65 );
    colorSetRGB( colorGetRootFocusColor(), 0x65, 0x65, 0x65 );//0x38, 0x6E, 0xBF  );
    colorSetRGB( colorGetRootFocusBackColor(), 0x65, 0x8E, 0xDF );
    colorSetRGB( colorGetRootClearColor(), 0xFF, 0x0E, 0xE0 );//0xFF, 0xE0, 0xE0 );
     
    guiSetRadius( 8 );
    guiSetStyle( VISIBLE|BUTTON|ROUNDED|BORDER|CENTERED );
    
    formFreeAll();
    formNew( 0, 0, 128, 128 );
    //formCurrentSetColor;
    //formCurrentSetBackColor;
    //formCurrentSetClearColor;
    widgetButton.Style = CAPTION|FILLED|VISIBLE|ROUNDED|BORDER|CENTERED;
    //widgetButton.Style = FILLED|VISIBLE|ROUNDED|BORDER|CENTERED;
    widgetButton.Type = BUTTON;
    widgetButton.X = 0;
    widgetButton.Y = 6;
    widgetButton.Width = 127;
    widgetButton.Height = 36;
    widgetButton.String = "Start Game";
    formAddWidget( &widgetButton );
    widgetAddEventHandler( widgetGetCurrent(), gemInit, SELECTED );
    
    widgetButton.Y = 46;
    widgetButton.String = "Images";
    formAddWidget( &widgetButton );
    widgetAddEventHandler( widgetGetCurrent(), drawLogoForm, SELECTED );
    
    /*widgetButton.Y = 86;
    widgetButton.String = "Reset";
    formAddWidget( &widgetButton );
    //widgetAddEventHandler( widgetGetCurrent(), eventGotFocus, GOTFOCUS );
    //widgetAddEventHandler( widgetGetCurrent(), eventLostFocus, LOSTFOCUS );
    widgetAddEventHandler( widgetGetCurrent(), drawReset, SELECTED );
    */
    drawForms( );
}
/*
void drawRFTest( void )
{
    //PYGMYCOLOR pygmyColor;
    //PYGMYFILE pygmyFile;
    PYGMYWIDGET widgetButton;

    colorSetRGB( colorGetRootColor(), 0x65, 0xBE, 0xFF ); //0x48, 0x8E, 0xDF );
    colorSetRGB( colorGetRootBackColor(), 0x65, 0x65, 0x65 );
    colorSetRGB( colorGetRootFocusColor(), 0x65, 0x65, 0x65 );//0x38, 0x6E, 0xBF  );
    colorSetRGB( colorGetRootFocusBackColor(), 0x65, 0x8E, 0xDF );
    colorSetRGB( colorGetRootClearColor(), 0xFF, 0x0E, 0xE0 );
     
    guiSetRadius( 8 );
    guiSetStyle( VISIBLE|BUTTON|ROUNDED|BORDER|CENTERED );
    
    formFreeAll();
    formNew( 0, 0, 128, 128 );
    
    widgetButton.Style = CAPTION|CENTERED|VISIBLE|ROUNDED|BORDER;
    widgetButton.Type = LABEL;
    widgetButton.X = 0;
    widgetButton.Y = 6;
    widgetButton.Width = 127;
    widgetButton.Height = 36;
    widgetButton.String = "Humidity";
    formAddWidget( &widgetButton );
    
    widgetButton.Style = CAPTION|VISIBLE|ROUNDED;
    widgetButton.Type = LABEL;
    widgetButton.X = 0;
    widgetButton.Y = 46;
    widgetButton.Width = 127;
    widgetButton.Height = 36;
    copyString( "Waiting...", globalHumidity );
    widgetButton.String = globalHumidity;
    formAddWidget( &widgetButton );
    //formSetFocus( formGetWidgetCount() - 1 );
    
    widgetButton.Style = CAPTION|FILLED|VISIBLE|ROUNDED|BORDER|CENTERED;
    widgetButton.Type = BUTTON;
    widgetButton.Y = 86;
    widgetButton.String = "Back";
    formAddWidget( &widgetButton );
    formSetFocus( formGetWidgetCount() - 1 );
    
    widgetAddEventHandler( widgetGetCurrent(), eventRFBack, SELECTED );
    
    
    drawForms();
    //taskNewSimple( "humidity", 1000, (void*)threadRFHumidity );
}
*/
/*
void threadRFHumidity( void )
{
    PYGMYWIDGET *pygmyWidget;
    PYGMYFILE pygmyFile;
    u16 uiLen;
    static uiLock = 0;
    
    
    if( !socketGet( 0x13480238, 0xFFFFFFFF ) ){
        //uiLock = ;
        //print( COM3, "\rNo Socket Found" );
        if( fileOpen( &pygmyFile, "humidity.txt", READ ) ){
            print( COM3, "\rFound humidity.txt" );
            uiLen = fileGetBuffer( &pygmyFile, 20, globalHumidity );
            if( uiLen != 0 && uiLen < 8 ){
                globalHumidity[ uiLen ] = '\0';
            } // if
            print( COM3, "\rLoaded value: %s", globalHumidity );
            fileDelete( "humidity.txt" );
            print( COM3, "\rDeleted humidity.txt" );
        
        //drawRFTest( );
        drawForms();
        } // if
        print( COM3, "\rRequesting humidity.txt" );
        socketRequestFile( 0x13480238, "humidity.txt" );
        print( COM3, "..." );
    } else if( uiLock == 0 ){
        uiLock = 1;
        socketRequestFile( 0x13480238, "humidity.txt" );
    } else{
        print( COM3, "Testing for File..." );
    } // else
}*/
/*
void threadRFHumiditySave( void )
{
    PYGMYMESSAGE pygmyMsg;
    PYGMYSOCKET *pygmySocket;
    u8 ucBuffer[ 20 ];
    
    if( msgGet( "humidity", PYGMY_UNMARKEDID, &pygmyMsg ) ){
        print( COM3, "\rMessage Received" );
        
        if( isStringSame( pygmyMsg.Message, "TX" ) ){
            convertIntToString( humidityRead(), "%d", ucBuffer );
            pygmySocket = socketGet( pygmyMsg.Value, pygmyMsg.Value );
            if( pygmySocket ){
                print( COM3, "\rSocket Found" );
                socketSendDataFromString( pygmySocket, ucBuffer ); 
            } else{
                print( COM3, "\rNo Socket Found" );
            } // else
        } else if( isStringSame( pygmyMsg.Message, "RX" ) ){
            print( COM3, "\rHumidity: %d", pygmyMsg.Value );
            convertIntToString( pygmyMsg.Value, "%d", globalHumidity );
            drawForms();
        } // else if
    }
}*/
/*
void eventRFBack( void )
{
    //formRemove();
    //form
    //drawForms();
    taskDelete( "humidity", 0 );
    drawMainMenu();
}*/
