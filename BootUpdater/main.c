/**************************************************************************
    PygmyOS ( Pygmy Operating System ) - BootLoader
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
/**************************************************************************
This PygmyOS application example is used to update the Pygmy Bootloader
The bootloader hex is converted to a resource file ( header ) with pygtoh.exe
This resource is then included and loaded into a file handle and used to 
replace the bootloader in the first 8KB of internal flash
***************************************************************************/


#include "pygmy_profile.h"
#include "boothex.h"

u8 bootTest( PYGMYFILE *pygmyFile );
void boot( void );

void main( void )
{
    PYGMYFILE pygmyFile;
    u8 i, ucStatus, ucBuffer[ 65 ], *ucSubString;

    sysInit();
    print( COM3, "\rUpdating bootloader, cancel boot after completion" );
    print( COM3, "\rOpening Resource..." );
    if( !fileOpenResource( &pygmyFile, (u8*)PYGMY_boothex ) ){
        print( COM3, "\rFile Failed to Open" );
    } else{
        print( COM3, "\rErasing..." );
        for( i = 0; i < 8; i++ ){
            fpecErasePage( i );
        } // for;
        print( COM3, "\rFlashing..." );
        for( ucStatus = 0; !( fileIsEOF( &pygmyFile ) ) && ucStatus != 0xFF; ){
            for( i = 0; i < 64; i++ ){
                ucBuffer[ i ] = fileGetChar( &pygmyFile );
                if( ucBuffer[ i ] == '\r' ){
                    ucBuffer[ i ] = '\0';
                    ucSubString = getNextSubString( (u8*)ucBuffer, WHITESPACE|NEWLINE );
                    // Add 1 to pointer before passing to skip the ':' packet start char
                    ucStatus = fpecProcessIHEX( (u8*)( ucSubString + 1 ) );
                    break; // Time to fetch next IHEX entry
                } // if
            } // for
        } // for
    } // else
    print( COM3, "\rBootloader Updated" );
    print( COM3, "\rReseting..." );
    delay( 100 );
    PYGMY_RESET;
    //boot();
}

u8 bootTest( PYGMYFILE *pygmyFile )
{
    // This function pre-tests every row in firmware file for corruption before erase and program
    u32 i;
    u8 ucBuffer[ 64 ], ucCalculatedSum, *ucSubString;

    for( ; !( fileIsEOF( pygmyFile ) ); ){
        // Get an IHEX packet
        for( i = 0; i < 64; i++ ){
            ucBuffer[ i ] = fileGetChar( pygmyFile );
            if( ucBuffer[ i ] == '\r' ){
                ucBuffer[ i ] = '\0';
                break;
            } // if
        } // for
        ucSubString = getNextSubString( ucBuffer, WHITESPACE|NEWLINE );
        
        if( *(ucSubString++) != ':' ){
            return( 0 );
        } // if
        // Following works because the output cannot be longer than the input being converted
        convertHexEncodedStringToBuffer( ucSubString, ucSubString );
        if( ucSubString[ 3 ] == IHEX_EOF ){
            break; // We have reached EOF without error
        }
        for( i = 0, ucCalculatedSum = 0; i < ucSubString[ 0 ]+4; i++ ){
            ucCalculatedSum += ucSubString[ i ];
        } // for
        ucCalculatedSum = 1 + ( 0xFF ^ (u8)ucCalculatedSum ); 
        if( (u8)ucCalculatedSum != ucSubString[ i ] ){ // Last short is checksum
            return( 0 ); // Corrupt HEX Row
        } // if
        i = ( (u16)ucSubString[ 1 ] << 8 ) + ucSubString[ 2 ];
        //if( ucSubString[ 3 ] == IHEX_DATA && i > 0x2000 ){
        //    return( 0 );
        //} // if 
    } // for

    return( 1 );
} 

void boot( void )
{
    PYGMYVOIDPTR pygmyBoot;
    u32 *ulOS;
    
    ulOS = (u32*)0x08000004; // Address is start vector table + 4 bytes
    if ( *ulOS != 0xFFFFFFFF ){
        pygmyBoot = (PYGMYVOIDPTR)*ulOS;
        RCC->CIR = 0x009F0000;
        SCB->VTOR = ((u32)0x08000000 & (u32)0x1FFFFF80);
        pygmyBoot(); // pass control to Pygmy Bootloader
    } // if
    print( COM3, "\rBootloader Fail " );
} 
