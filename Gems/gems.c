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

void gemDrawEndMenu( void );
void drawMainMenu( void );
void drawRFTest( void );
void eventRFBack( void );
extern const u8 PYGMY_orbitron18[];
#include "ruby.h"
#include "emerald.h"
#include "topaz.h"
#include "ytopaz.h"
#include "dollar.h"

PYGMYWIDGET widgetButton;
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
    PYGMYCOLOR pygmyColor = { 0xFF, 0xFF, 0xFF };
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
    guiSetCursor( 80, 0 );
    guiClearArea( &pygmyColor, 80, 0, 127, 18 );
    pygmyColor.R = 0xFF;
    pygmyColor.G = 0x0E;
    pygmyColor.B = 0xE0;
    fontSetColor( fontGetActive(), &pygmyColor );
    drawRect( &pygmyColor, 80, 0, 127, 18, VISIBLE|ROUNDED, 4 );
    print( LCD, "%4d", uiScore );
    return( uiScore );
}

u16 gemDetectRowsY( void )
{
    PYGMYCOLOR pygmyColor;
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

    return( uiScore );
}

void gemDrawGameOver( void )
{
    PYGMYCOLOR pygmyColor = { 0xFF, 0x00, 0x20 };

    //guiSaveScreen();
    taskDelete( "gemgrav" );
    guiInitSprites( );
    guiClearScreen( &pygmyColor);
    guiSetCursor( 8, 55 );
    fontSetColor( fontGetActive(), (PYGMYCOLOR *)colorGetRootColor() );
    print( LCD, "Game Over" );
    
    delay( 30000 );
    gemDrawEndMenu();
    PYGMY_WATCHDOG_REFRESH;
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
    //print( COM3, "\rAdded Gem %d at X %d Y %d", globalGems[ uiX ][ uiY ], uiX, uiY );

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
    PYGMYCOLOR pygmyColor = { 0xFF, 0xFF, 0xFF };
    u16 uiX, uiY;

    if( globalCursor1Coords[ 0 ] < 16 ){
        return;
    } // if
    uiX = ( globalCursor1Coords[ 0 ] - 16 ) / 16;
    uiY = ( globalCursor1Coords[ 1 ] / 16 );
    if( !gemCollision( uiX, uiY  ) ){
        guiClearArea( &pygmyColor, globalCursor1Coords[ 0 ], globalCursor1Coords[ 1 ], 
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
    PYGMYCOLOR pygmyColor = {0xFF, 0xFF, 0xFF };
    u16 uiX, uiY;

    uiX = ( globalCursor1Coords[ 0 ] + 16 ) / 16;
    uiY = ( globalCursor1Coords[ 1 ] / 16 );
    if( uiX > 7 ){
        return;
    } // if
    if( !gemCollision( uiX, uiY ) ){
        guiClearArea( &pygmyColor, globalCursor1Coords[ 0 ], globalCursor1Coords[ 1 ], 
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
    PYGMYCOLOR pygmyColor;

    pygmyColor.R = 0xFF;
    pygmyColor.G = 0xFF;
    pygmyColor.B = 0xFF;
    if( !gemCollision( ( globalCursor1Coords[ 0 ] / 16 ), ( globalCursor1Coords[ 1 ] + 16 ) / 16 ) ){
        guiClearArea( &pygmyColor, globalCursor1Coords[ 0 ], globalCursor1Coords[ 1 ], 
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
    PYGMYCOLOR pygmyColor;
    u16 i, ii;

    // Create Form to draw sprites on...
    colorSetRGB( colorGetRootColor(), 0x65, 0xBE, 0xFF ); //0x48, 0x8E, 0xDF );
    colorSetRGB( colorGetRootBackColor(), 0xFF, 0xFF, 0xFF );
    colorSetRGB( colorGetRootFocusColor(), 0xFF, 0xFF, 0xFF );//0x38, 0x6E, 0xBF  );
    colorSetRGB( colorGetRootFocusBackColor(), 0xFF, 0xFF, 0xFF );
    colorSetRGB( colorGetRootClearColor(), 0xFF, 0xFF, 0xFF );
    fontSetColor( fontGetActive(), colorGetRootColor() );
    fontSetBackColor( fontGetActive(), colorGetRootBackColor() );
    formFreeAll();
    //formNew( 0, 0, 128, 128 );
    //drawForms( );
    guiClearArea( colorGetRootClearColor(), 0, 0, 128, 128 );
    // Now create sprites
    guiInitSprites();
    globalCursor1Gem = GEM_RUBY;

    //guiSetColor( 0x00, 0x00, 0xFF );
    //guiSetBackColor( 0xFF, 0xFF, 0xFF );
    //pygmyColor.R = 0xFF;
    //pygmyColor.G = 0xFF;
    //pygmyColor.B = 0xFF;
    //guiClearScreen( &pygmyColor);
    
    //fileOpenResource( &fileFont, (u8*)PYGMY_orbitron18 );
    //guiSetFont( &fileFont, &fontOrbitron18 );
    
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
    //pinConfig( SHIELD_UP, PULLUP );
    //pinConfig( SHIELD_DOWN, PULLUP );
    //pinConfig( SHIELD_LEFT, PULLUP );
    //pinConfig( SHIELD_RIGHT, PULLUP );
    //pinConfig( SHIELD_CENTER, PULLUP );
    //pinConfig( SHIELD_BUTTON1, PULLUP );
    //pinConfig( SHIELD_BUTTON2, PULLUP );
    pinInterrupt( gemJoyUp, SHIELD_UP,  TRIGGER_RISING, 1 );
    pinInterrupt( gemJoyDown, SHIELD_DOWN,  TRIGGER_RISING, 1 );
    pinInterrupt( gemJoyLeft, SHIELD_LEFT,  TRIGGER_RISING, 1 );
    pinInterrupt( gemJoyRight, SHIELD_RIGHT,  TRIGGER_RISING, 1 );
    pinInterrupt( gemDrawEndMenu, SHIELD_BUTTON1, TRIGGER_RISING, 1 );
    pinInterrupt( gemDrawEndMenu, SHIELD_BUTTON2, TRIGGER_RISING, 1 );
    gemResetCursor( );
    taskNewSimple( "gemgrav", 200, (PYGMYFUNCPTR)gemGravity );
}

void gemClickMenu( void )
{   if( pinGet( SHIELD_CENTER ) ){
        widgetButton.Style &= ~INVERT;
    } else{
        widgetButton.Style |= INVERT;
    } // else
    
    drawWidget( &widgetButton );
}


void gemDrawEndMenu( void )
{
    PYGMYWIDGET widgetButton;

    if( taskIsRunning( "gemgrav" ) ){
        taskDelete( "gemgrav" );
    }
    guiInitSprites( );
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
    colorSetRGB( colorGetRootClearColor(), 0xFF, 0x0E, 0xE0 );
     
    guiSetRadius( 8 );
    guiSetStyle( VISIBLE|BUTTON|ROUNDED|BORDER|CENTERED );
    
    formFreeAll();
    formNew( 0, 0, 128, 128 );
    widgetButton.Style = CAPTION|FILLED|VISIBLE|ROUNDED|BORDER|CENTERED;
    widgetButton.Type = BUTTON;
    widgetButton.X = 0;
    widgetButton.Y = 6;
    widgetButton.Width = 127;
    widgetButton.Height = 36;
    widgetButton.String = "New Game";
    formAddWidget( &widgetButton );
    widgetAddEventHandler( widgetGetCurrent(), gemInit, SELECTED );
    
    widgetButton.Y = 46;
    widgetButton.String = "Main Menu";
    formAddWidget( &widgetButton );
    widgetAddEventHandler( widgetGetCurrent(), drawMainMenu, SELECTED );
    
    drawForms();
}

