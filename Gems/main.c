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
//#include "gems.h"

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

void main( void )
{
    //mstats mstatsTest;
    PYGMYFILE pygmyFile;
    PYGMYWIDGET widgetButton;
    u8 *ucBuffer;

    sysInit();
    fileOpen( &pygmyFile, "picon128", READ );
    drawImage( 0,0, &pygmyFile, 0 );
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
    
    /*guiSetColor( 0xFF, 0xFF, 0xFF );
    guiSetBackColor( 0xFF, 0xFF, 0xFF );
    guiClearScreen();
    guiSetColor( 0x48, 0x8E, 0xDF );
    guiSetBackColor( 0x65, 0x65, 0x65 );
    fileOpenResource( &fileFont, (u8*)PYGMY_orbitron18 );
    guiSetFont( &fileFont, &fontOrbitron18 );
    guiSetRadius( 8 );
    guiSetStyle( FILLED|VISIBLE|BUTTON|ROUNDED|BORDER|CENTERED );
    guiSetColor( 0x00, 0x00, 0x00 );
    drawThickLine( 8, 8, 120, 8, 2, VISIBLE );
    guiSetColor( 0xff, 0x00, 0x00 );
    drawThickLine( 8, 20, 120, 8, 2, VISIBLE );
    guiSetColor( 0x00, 0xff, 0x00 );
    drawThickLine( 8, 50, 120, 8, 3, VISIBLE );
    guiSetColor( 0x00, 0x00, 0xff );
    drawThickLine( 8, 100, 120, 8, 4, VISIBLE );
    
    
    widgetButton.Style = CAPTION|FILLED|VISIBLE|ROUNDED|BORDER|CENTERED;
    
    widgetButton.Type = BUTTON;
    widgetButton.X= 0;
    widgetButton.Y = ( 128 / 2 ) - 20;
    widgetButton.Width = 108;
    widgetButton.Height = 39;
    widgetButton.String = "Start Game";
    formNew( 0, 0, 128, 128 );
    formAddWidget( &widgetButton );
    widgetButton.Type = CHECKBOX;
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
    formAddWidget( &widgetButton );
    */
    //drawForms( );
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
