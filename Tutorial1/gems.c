/**************************************************************************
    Pygmy Gems ( Example for Pygmy Operating System )
    Copyright (C) 2011-2012  Warren D Greenway

    This file is part of an example app for PygmyOS.

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

#include "pygmy_profile.h"
#include "gems.h"
#include "fonts/orbitron18.h"
#include "ruby.h"
#include "emerald.h"
#include "topaz.h"
#include "ytopaz.h"
#include "dollar.h"

PYGMYFILE fileFont, imgBusy, imgRuby, imgEmerald, imgTopaz, imgYTopaz, imgDollar;
PYGMYFONT fontOrbitron18;

u16 globalRubyCoords[ 64 ], globalEmeraldCoords[ 64 ], globalTopazCoords[ 64 ], globalYTopazCoords[ 64 ];
u16 globalDollarCoords[ 64 ], globalCursor1Coords[ 2 ];
u16 globalRubyCount, globalEmeraldCount, globalTopazCount, globalYTopazCount;
u16 globalPoints =  0, globalBonus = 1, globalSpeed = 500;
u8 globalGems[ 8 ][ 8 ], globalCursor1Gem;

void gemDetectRows( void )
{
    
}

u16 gemDetectRowsX( void )
{
    u16 uiX, uiY, uiColCount, uiColStart, uiScore;
  
    uiScore = 0;
   
    for( uiY = 0; uiY < 8; uiY++ ){
        for( uiX = 0, uiColCount = 0; uiX < 7; uiX++ ){
            if( globalGems[ uiX ][ uiY ] && ( globalGems[ uiX ][ uiY ] == globalGems[ uiX+1 ][ uiY ] ) ){
                if( !uiColCount ){
                    uiColStart = uiX;
                    uiColCount = 1;
                } // if
                ++uiColCount;
            } else{
                if( uiColCount > 2 ){
                    break;
                } // if
                uiColCount = 0;
            } // else
        } // for
        if( uiColCount > 2 ){
            uiScore += uiColCount;
            for( uiX = uiColStart; uiX < uiColStart + uiColCount; uiX++ ){
                guiRemoveSpriteCoords( globalGems[ uiX ][ uiY ], uiX * 16, uiY * 16 );
                globalGems[ uiX ][ uiY ] = 0;
                gemAdd( uiX, uiY, GEM_DOLLAR ); 
            } // for
        } // if
    } // for
    uiScore += gemDetectRowsY( );
    print( COM3, "\rGems deleted: %d", uiScore );
    return( uiScore );
}

u16 gemDetectRowsY( void )
{
    u16 uiX, uiY, uiRowCount, uiRowStart, uiScore;
  
    uiScore = 0;
   
    
    for( uiX = 0; uiX < 8; uiX++ ){
        for( uiY = 0, uiRowCount = 0; uiY < 7; uiY++ ){
            if( globalGems[ uiX ][ uiY ] && ( globalGems[ uiX ][ uiY ] == globalGems[ uiX ][ uiY+1 ] ) ){
                if( !uiRowCount ){
                    uiRowStart = uiY;
                    uiRowCount = 1;
                } // if
                ++uiRowCount;
            } else{
                if( uiRowCount > 2 ){
                    break;
                } // if
                uiRowCount = 0;
            } // else
        } // for
        if( uiRowCount > 2 ){
            uiScore += uiRowCount;
            for( uiY = uiRowStart; uiY < uiRowStart + uiRowCount; uiY++ ){
                guiRemoveSpriteCoords( globalGems[ uiX ][ uiY ], uiX * 16, uiY * 16 );
                globalGems[ uiX ][ uiY ] = 0;
                gemAdd( uiX, uiY, GEM_DOLLAR ); 
            } // for
        } // if
    } // for

    //print( COM3, "\rGems deleted: %d", uiScore );
    return( uiScore );
}

void gemDrawGameOver( void )
{
    
    taskDelete( "gemgrav", 0 );
    guiInitSprites( );
    guiClearScreen();
    guiSetCursor( 8, 55 );
    print( LCD, "Game Over" );
    drawRect( 0, 0, 127, 127, 8, 0 );
}

void gemResetCursor( void )
{
    PYGMYSPRITE *spriteCursor, *spriteGem;
    
    globalCursor1Gem = sysRandom( GEM_RUBY, GEM_YTOPAZ );
    
    spriteCursor = guiGetSprite( GEM_CURSOR1 );
    spriteGem = guiGetSprite( globalCursor1Gem );
    fileCopyHandle( &spriteGem->File, &spriteCursor->File );
    spriteCursor->Coords[ 0 ] = 48;
    spriteCursor->Coords[ 1 ] = 0;
    spriteCursor->Index = 0;
    spriteCursor->Len = 2;
}

void gemProcess( void )
{
   /* PYGMYSPRITE *pygmySprite[ GEM_MAXSPRITES ];
    u16 i, uiX, uiY;
    u8 ucGem;

    for( i = GEM_RUBY; i < GEM_MAXSPRITES + 1; i++ ){
        pygmySprite[ i ] = guiGetSprite( i );
        pygmySprite[ i ]->Len = 0;
    } // for
    for( i = 0; i < 64; i++ ){
        if( globalGems[ i ] ){ 
            ucGem = globalGems[ i ];
            uiX = i % 8;
            uiY = i / 8;
            pygmySprite[ globalGems[ i ] ]->Coords[ pygmySprite[ globalGems[ i ] ]->Len++ ] = uiX;
            pygmySprite[ globalGems[ i ] ]->Coords[ pygmySprite[ globalGems[ i ] ]->Len++ ] = uiY;
        } // if
    } // for
    //pygmySprite = guiGetSprite( );
    */
}

u8 gemCollision( u16 uiX, u16 uiY )
{
    // Function tests location in 8 x 8 grid for existing sprite
   
    if( globalGems[ uiX ][ uiY ] || uiX > 7 || uiY > 7 ){
        return( TRUE ); // Collision detected, space occupied
    } // if
    
    return( FALSE );
}

u8 gemAdd( u16 uiX, u16 uiY, u8 ucGem )
{
    PYGMYSPRITE *pygmySprite;
   
    if( !uiY ){
        // Game over if gem placed in row 0
        return( FALSE );
    } // if
    pygmySprite = guiGetSprite( ucGem );
    globalGems[ uiX ][ uiY ] = ucGem;
    pygmySprite->Coords[ pygmySprite->Len++ ] = uiX * 16;
    pygmySprite->Coords[ pygmySprite->Len++ ] = uiY * 16;
    print( COM3, "\rAdded Gem %d at X %d Y %d", globalGems[ uiX ][ uiY ], uiX, uiY );

    return( TRUE );
}

void gemJoyUp( void )
{
    //globalCursor1Coords[ 0 ] 
}

void gemJoyDown( void )
{

}

void gemJoyLeft( void )
{
    u16 uiX, uiY;

    if( globalCursor1Coords[ 0 ] < 16 ){
        return;
    } // if
    uiX = ( globalCursor1Coords[ 0 ] - 16 ) / 16;
    uiY = ( globalCursor1Coords[ 1 ] / 16 );
    if( !gemCollision( uiX, uiY  ) ){
        guiClearArea( globalCursor1Coords[ 0 ], globalCursor1Coords[ 1 ], 
            globalCursor1Coords[ 0 ] + 16, globalCursor1Coords[ 1 ] + 16 );
        globalCursor1Coords[ 0 ] -= 16;
    } //if else{
        //gemAdd( globalCursor1Coords[ 0 ]/16, globalCursor1Coords[ 1 ]/16, globalCursor1Gem );
        //gemProcess( );
        //gemResetCursor();
    //} // else
}

void gemJoyRight( void )
{
    u16 uiX, uiY;

    uiX = ( globalCursor1Coords[ 0 ] + 16 ) / 16;
    uiY = ( globalCursor1Coords[ 1 ] / 16 );
    if( uiX > 7 ){
        return;
    } // if
    if( !gemCollision( uiX, uiY ) ){
        guiClearArea( globalCursor1Coords[ 0 ], globalCursor1Coords[ 1 ], 
            globalCursor1Coords[ 0 ] + 16, globalCursor1Coords[ 1 ] + 16 );
        globalCursor1Coords[ 0 ] += 16;
    } // else{ 
        //gemAdd( uiX, uiY, globalCursor1Gem );
        //gemProcess();
        //gemResetCursor();
    //} // else
}

void gemJoyClick( void )
{

}

void gemButton1( void )
{
    
}

void gemButton2( void )
{
    gemInit();
}

void gemGravity( void )
{
    if( !gemCollision( ( globalCursor1Coords[ 0 ] / 16 ), ( globalCursor1Coords[ 1 ] + 16 ) / 16 ) ){
        guiClearArea( globalCursor1Coords[ 0 ], globalCursor1Coords[ 1 ], 
            globalCursor1Coords[ 0 ] + 16, globalCursor1Coords[ 1 ] + 16 );
        globalCursor1Coords[ 1 ] += 16;
    } else{
        if( !gemAdd( globalCursor1Coords[ 0 ]/16, globalCursor1Coords[ 1 ]/16, globalCursor1Gem ) ){
            gemDrawGameOver();
            return;
        } // if
        gemDetectRowsX( );
        gemResetCursor();
    } // else
}

void gemInit( void )
{
    u16 i, ii;
    
    guiClearScreen();
    guiInitSprites();
    globalCursor1Gem = GEM_RUBY;

    guiSetColor( 0x00, 0x00, 0xFF );
    guiSetBackColor( 0xFF, 0xFF, 0xFF );
    fileOpenResource( &fileFont, (u8*)PYGMY_orbitron18 );
    guiSetFont( &fileFont, &fontOrbitron18 );
    
    fileOpenResource( &imgRuby, (u8*)PYGMY_ruby );
    fileOpenResource( &imgEmerald, (u8*)PYGMY_emerald );
    fileOpenResource( &imgTopaz, (u8*)PYGMY_topaz );
    fileOpenResource( &imgYTopaz, (u8*)PYGMY_ytopaz );
    fileOpenResource( &imgDollar, (u8*)PYGMY_dollar );
    guiCreateSprite( &imgRuby, GEM_CURSOR1, globalCursor1Coords, 2, 100, 0 );
    guiCreateSprite( &imgRuby, GEM_RUBY, globalRubyCoords, 0, 100, 0 );
    guiCreateSprite( &imgEmerald, GEM_EMERALD, globalEmeraldCoords, 0, 100, 0 );
    guiCreateSprite( &imgTopaz, GEM_TOPAZ, globalTopazCoords, 0, 100, 0 );
    guiCreateSprite( &imgYTopaz, GEM_YTOPAZ, globalYTopazCoords, 0, 100, 0 );
    guiCreateSprite( &imgDollar, GEM_DOLLAR, globalDollarCoords, 0, 100, 0 );

    for( i = 0; i < 8; i++ ){
        for( ii = 0; ii < 8; ii++ ){
            globalGems[ i ][ ii ] = 0;
        } // for
    } // for
    pinConfig( SHIELD_UP, PULLUP );
    pinConfig( SHIELD_DOWN, PULLUP );
    pinConfig( SHIELD_LEFT, PULLUP );
    pinConfig( SHIELD_RIGHT, PULLUP );
    pinConfig( SHIELD_CENTER, PULLUP );
    pinConfig( SHIELD_BUTTON1, PULLUP );
    pinConfig( SHIELD_BUTTON2, PULLUP );
    pinInterrupt( gemJoyUp, SHIELD_UP,  TRIGGER_RISING );
    pinInterrupt( gemJoyDown, SHIELD_DOWN,  TRIGGER_RISING );
    pinInterrupt( gemJoyLeft, SHIELD_LEFT,  TRIGGER_RISING );
    pinInterrupt( gemJoyRight, SHIELD_RIGHT,  TRIGGER_RISING );
    //pinInterrupt( gemInit, SHIELD_BUTTON2, TRIGGER_RISING );
    gemResetCursor( );
    taskNewSimple( "gemgrav", 200, (PYGMYFUNCPTR)gemGravity );
}

void gemDrawMenu( void )
{
    PYGMYWIDGET widgetButton;
    
    guiSetFontColor( &fontOrbitron18, 0x00, 0x0, 0xFF );
    guiSetFontBackColor( &fontOrbitron18, 0xFF, 0xFF, 0xFF );
    widgetButton.Font = &fontOrbitron18;
    widgetButton.Color.R = 0x00;
    widgetButton.Color.G = 0x00;
    widgetButton.Color.B = 0xFF;
    widgetButton.BackColor.R = 0xFF;
    widgetButton.BackColor.G = 0xFF;
    widgetButton.BackColor.B = 0xFF;
    //widgetButton.Cursor.X = 8;
    //widgetButton.Cursor.Y = ( 128 / 2 ) - 10;
    widgetButton.Style = VISIBLE|ROUNDED|BUTTON|BORDER|CENTERED;
    widgetButton.X= 0;
    widgetButton.Y = ( 128 / 2 ) - 20;
    widgetButton.Width = 128;
    widgetButton.Height = 40;
    widgetButton.String = "Start Game";
    
    drawWidget( &widgetButton );
}


