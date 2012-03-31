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


PYGMYFILE fileFont;
PYGMYFONT fontOrbitron18;
void drawMainMenu( void );
void drawRFTest( void );
void eventRFBack( void );
void threadRFHumidity( void );
void threadRFHumiditySave( void );

u8 globalHumidity[ 20 ];

void main( void )
{
    u8 i, ucChar, ucBuffer[ 20 ];

    sysInit();
    socketInit();
    rfInit();
    print( COM3, "\rV2 RFID: 0x%X", socketGetID());
    //pressureInit( TX1, RX1, NONE, NONE );
    //print( COM3, "\rPressure: %d", (u16)pressureReadkPa() );
    //
    //fileDelete( "humidity.txt" );

    if( pygmyGlobalData.MCUID == DESC_STM32F103XLD ){
        print( COM3, "\rpygmyGlobalData.MCUID == DESC_STM32F103XLD" );
    } // if
    if( pygmyGlobalData.MainClock == 72000000 ){
        print( COM3, "\rpygmyGlobalData.MainClock == 72000000 " );
    } // if
    if( pygmyGlobalData.DelayTimer == PYGMY_TIMER9 ){
        print( COM3, "\rpygmyGlobalData.DelayTimer == PYGMY_TIMER9" );
    } // if
     if( pygmyGlobalData.PWMTimer == PYGMY_TIMER10 ){
        print( COM3, "\rpygmyGlobalData.PWMTimer == PYGMY_TIMER10" );
     } // if
    //stopwatchStart();
    //delay( 1000000 );
    //print( COM3, "\rStopwatch %d", stopwatchGet() );
    //stopwatchStart();
    //delay( 200000 );
    //print( COM3, "\rStopwatch %d", stopwatchGet() );
    
    eepromOpen( 0x51, TX1, RX1, NONE );
    //eepromPutString( 0, "test" );
    eepromPutString( 0, "<shield varitronix>" );
    //eepromPutChar( 1, 'U' );
    //eepromPutChar( 2, 'B' );
    print( COM3, "\rEEPROM Direct Read: " );
    /*for( i = 0; i < 3; i++ ){
        ucChar = eepromGetChar( i );
        if( isAlphaOrNumeric( ucChar ) ) {
            print( COM3, "%c", ucChar );
        } else{
            print( COM3, "(%d)", ucChar  );//ucBuffer, len( "humidity" ) );
        } // else
    } // for
    */
    //ucBuffer[ len( "humidity" ) ] = '\0';
    //print( COM3, "\rEEPROM: %s", ucBuffer );
    //
    if( isStringSame( ucBuffer, "<shield varitronix>" ) ){
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
        humidityInit( A2, A3 );
    } // else
    //convertFloatToString( 1489.01234567, "%5.10", ucBuffer );
    //print( COM3, ucBuffer );
    //print( COM3, "\r%1.10f", -1489.01234567 );
    //print( COM3, "\r%1.10f", convertStringToFloat( "1489.01234567" ) );
    //print( COM3, "\r%1.10f", convertStringToFloat( "+1489.01234567" ) );
    //print( COM3, "\r%1.10f", convertStringToFloat( "-1489.01234567" ) );
    
    
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

void drawMainMenu( void )
{
    PYGMYCOLOR pygmyColor;
    PYGMYFILE pygmyFile;
    PYGMYWIDGET widgetButton;

    pinInterrupt( eventMouseMoveUp, A0, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveDown, D3, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveLeft, TA0, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveRight, DAC1, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickLeft, DAC2, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickCenter, TX2, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickRight, RX2, TRIGGER_RISING, 1 );

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
    //widgetAddEventHandler( widgetGetCurrent(), eventGotFocus, GOTFOCUS );
    //widgetAddEventHandler( widgetGetCurrent(), eventLostFocus, LOSTFOCUS );
    widgetAddEventHandler( widgetGetCurrent(), gemInit, SELECTED );
    
    widgetButton.Y = 46;
    widgetButton.String = "RF Demo";
    formAddWidget( &widgetButton );
    //widgetAddEventHandler( widgetGetCurrent(), eventGotFocus, GOTFOCUS );
    //widgetAddEventHandler( widgetGetCurrent(), eventLostFocus, LOSTFOCUS );
    widgetAddEventHandler( widgetGetCurrent(), drawRFTest, SELECTED );
    
    widgetButton.Y = 86;
    widgetButton.String = "Contrast";
    formAddWidget( &widgetButton );
    //widgetAddEventHandler( widgetGetCurrent(), eventGotFocus, GOTFOCUS );
    //widgetAddEventHandler( widgetGetCurrent(), eventLostFocus, LOSTFOCUS );
    //widgetAddEventHandler( widgetGetCurrent(), eventSelected, SELECTED );
    
    drawForms( );
}

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
        /*pygmyWidget = widgetGet( globalHumidity );
        if( pygmyWidget ){
            //pygmyWidget->Style = CAPTION|FILLED|VISIBLE|ROUNDED|BORDER|CENTERED;
            guiClearArea( colorGetRootClearColor(), pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
                pygmyWidget->Y+pygmyWidget->Height );
            guiSetCursor( pygmyWidget->X+2, pygmyWidget->Y + 9 );
            print( LCD, globalHumidity );
        } // if
        */
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
}

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
    
    /*PYGMYFILE pygmyFile;
    u8 ucBuffer[ 20 ];
    
    if( fileOpen( &pygmyFile, "humidity.txt", WRITE ) ){
        convertIntToString( humidityRead(), "%d", ucBuffer );
        filePutString( &pygmyFile, ucBuffer );
        fileClose( &pygmyFile );
    } // if
    */
}

void eventRFBack( void )
{
    //formRemove();
    //form
    //drawForms();
    taskDelete( "humidity", 0 );
    drawMainMenu();
}
