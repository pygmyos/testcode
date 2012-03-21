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

#include "pygmy_profile.h"
#include "fonts/orbitron18.h"
#include "gems.h"

/*void btnOKClick( void )
{
    PYGMYWIDGET widgetButton;
    
    guiSetColor( 0x00, 0x00, 0xFF );
    guiSetBackColor( 0xFF, 0xFF, 0xFF );
    guiClearScreen();
    fileOpenResource( &fileFont, (u8*)PYGMY_orbitron18 );
    guiSetFont( &fileFont, &fontOrbitron18 );
    widgetButton.Font = &fontOrbitron18;
    if( pinGet( SHIELD_CENTER ) ){
        guiSetFontColor( &fontOrbitron18, 0x00, 0x0, 0xFF );
        guiSetFontBackColor( &fontOrbitron18, 0xFF, 0xFF, 0xFF );
        
        widgetButton.Color.R = 0x00;
        widgetButton.Color.G = 0x00;
        widgetButton.Color.B = 0xFF;
        widgetButton.BackColor.R = 0xFF;
        widgetButton.BackColor.G = 0xFF;
        widgetButton.BackColor.B = 0xFF;
    } else{
        guiSetFontColor( &fontOrbitron18, 0xFF, 0xFF, 0xFF );
        guiSetFontBackColor( &fontOrbitron18, 0x00, 0x00, 0xFF );
        //widgetButton.Font = &fontOrbitron18;
        widgetButton.Color.R = 0xFF;
        widgetButton.Color.G = 0xFF;
        widgetButton.Color.B = 0xFF;
        widgetButton.BackColor.R = 0x00;
        widgetButton.BackColor.G = 0x00;
        widgetButton.BackColor.B = 0xFF;
    } // else
    //widgetButton.Cursor.X = 8;
    //widgetButton.Cursor.Y = ( 128 / 2 ) - 10;
    widgetButton.Radius = 8;
    widgetButton.Style = VISIBLE|BUTTON|ROUNDED|BORDER|CENTERED;
    widgetButton.X= 0;
    widgetButton.Y = ( 128 / 2 ) - 20;
    widgetButton.Width = 127;
    widgetButton.Height = 39;
    widgetButton.String = "Start Game";
    
    drawWidget( &widgetButton );
}*/
PYGMYFILE fileFont;
PYGMYFONT fontOrbitron18;
void drawMainMenu( void );

void main( void )
{
    //mstats mstatsTest;
    
    u8 *ucBuffer;

    sysInit();
    //rfInitSockets();
    //rfInit();
    //print( COM3, "\rRFID: 0x%X", rfGetID());
    //pressureInit( TX1, RX1, NONE, NONE );
    //print( COM3, "\rPressure: %d", (u16)pressureReadkPa() );
    //humidityInit( A2, A3 );
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
    pinInterrupt( eventMouseMoveUp, A0, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveDown, D3, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveLeft, TA0, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseMoveRight, DAC1, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickLeft, DAC2, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickCenter, TX2, TRIGGER_RISING, 1 );
    pinInterrupt( eventMouseClickRight, RX2, TRIGGER_RISING, 1 );
    drawMainMenu();
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

    colorSetRGB( colorGetRootColor(), 0x65, 0xBE, 0xFF ); //0x48, 0x8E, 0xDF );
    colorSetRGB( colorGetRootBackColor(), 0x65, 0x65, 0x65 );
    colorSetRGB( colorGetRootFocusColor(), 0x65, 0x65, 0x65 );//0x38, 0x6E, 0xBF  );
    colorSetRGB( colorGetRootFocusBackColor(), 0x65, 0x8E, 0xDF );
    colorSetRGB( colorGetRootClearColor(), 0xE0, 0xE0, 0xFF );
    
    guiSetRadius( 8 );
    guiSetStyle( VISIBLE|BUTTON|ROUNDED|BORDER|CENTERED );
    
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
    //widgetAddEventHandler( widgetGetCurrent(), eventSelected, SELECTED );
    
    widgetButton.Y = 86;
    widgetButton.String = "Contrast";
    formAddWidget( &widgetButton );
    //widgetAddEventHandler( widgetGetCurrent(), eventGotFocus, GOTFOCUS );
    //widgetAddEventHandler( widgetGetCurrent(), eventLostFocus, LOSTFOCUS );
    //widgetAddEventHandler( widgetGetCurrent(), eventSelected, SELECTED );
    /*widgetButton.Type = CHECKBOX;
    widgetButton.Value = TRUE;
    widgetButton.Style = CAPTION|VISIBLE|ROUNDED;
    widgetButton.X = 4;
    widgetButton.Y = 4;
    widgetButton.Width = 64;
    widgetButton.Height = 20;
    widgetButton.String = "Easy";
    formAddWidget( &widgetButton );
    
    widgetButton.Type = HSCROLLBAR;
    widgetButton.Value = 50;
    widgetButton.Style = VISIBLE|ROUNDED|BORDER;
    widgetButton.X = 0;
    widgetButton.Y = 107;
    widgetButton.Width = 108;
    widgetButton.Height = 20;
    widgetButton.String = NULL;
    formAddWidget( &widgetButton );
    
    widgetButton.Type = VSCROLLBAR;
    widgetButton.Value = 50;
    widgetButton.Style = VISIBLE|ROUNDED|BORDER;
    widgetButton.X = 107;
    widgetButton.Y = 0;
    widgetButton.Width = 20;
    widgetButton.Height = 128;
    widgetButton.String = NULL;
    formAddWidget( &widgetButton );*/
    
    drawForms( );
    
}
