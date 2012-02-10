/**************************************************************************
    PygmyOS ( Pygmy Operating System )
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
/**************************************************************************
    This example program for Pygmy OS explores the Filesystem 
    
***************************************************************************/

#include "pygmy_profile.h"

void main( void )
{
    PYGMYFILE pygmyFile;
    u32 i, ii;
    u8 ucStat, ucName[13];

   // print( COM3, "\rInit System..." );
    sysInit();

    print( COM3, "\rMCU ID: 0x%X", pygmyGlobalData.MCUID );
    fileFormat( "" );
    //filePrintDebug( COM3 );
    //if( fileOpen( &pygmyFile, "test1", WRITE ) ){
        
        print( COM3, "\rCreating Files" );
        copyString( "test", ucName );
        for( i = 0; i < 100; i++ ){
            convertIntToString( i, "%04d", ucName + 4 );
            if( !fileOpen( &pygmyFile, ucName, WRITE ) ){
                break;
            } // if
            //fileOpen( &pygmyFile, "test", WRITE );
            filePutString( &pygmyFile, "Test" );
            //fileClose( &pygmyFile );
            fileDelete( ucName );
        } // for
        
        for( i = 0; i < 2000; i++ ){
            convertIntToString( i, "%04d", ucName + 4 );
            if( !fileOpen( &pygmyFile, ucName, WRITE ) ){
                break;
            } // if
            filePutString( &pygmyFile, "Test" );
            fileClose( &pygmyFile );
        } // for
        
       /* for( i = 0; !fileIsRootFull(); i++ ){ 
            if( !filePutChar( &pygmyFile, 'T' ) ){
                print( COM3, "\rData Write Failed!" );
                break;
            } // if
            if( !( i % 65536 ) ){
                print( COM3, "." );
            } // if
        } // for
        fileClose( &pygmyFile );*/
        
        //print( COM3, "\rFile system full\r%d Bytes Written in %d milliseconds", i, stopwatchGet() );
        filePrintDebug( COM3 );
    //} else{
    //    print( COM3, "\rFile failed to open!" );
   // }
    
    while( 1 ){;}
}
