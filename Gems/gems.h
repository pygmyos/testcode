/**************************************************************************
    Pygmy Gems ( Pygmy Operating System )
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
//#include "icons/busy.h"


#define GEM_EMPTY   0
#define GEM_CURSOR1 1
#define GEM_CURSOR2 2
#define GEM_CURSOR3 3
#define GEM_RUBY    4
#define GEM_EMERALD 5
#define GEM_TOPAZ   6
#define GEM_YTOPAZ  7
#define GEM_DOLLAR  8
#define GEM_MAXSPRITES 9

void gemDetectRows( void );
u16 gemDetectRowsX( void );
u16 gemDetectRowsY( void );
void gemDrawGameOver( void );
void gemResetCursor( void );
void gemProcess( void );
u8 gemCollision( u16 uiX, u16 uiY );
u8 gemAdd( u16 uiX, u16 uiY, u8 ucGem );

void gemJoyUp( void );
void gemJoyDown( void );
void gemJoyLeft( void );
void gemJoyRight( void );
void gemJoyClick( void );
void gemButton1( void );
void gemButton2( void );
void gemGravity( void );
void gemInit( void );

void gemClickMenu( void );
void gemDrawMenu( void );

