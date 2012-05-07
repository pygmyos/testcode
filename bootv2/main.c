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

#include "pygmy_profile.h"
#include "pygmy_xmodem.h"
//#include "profiles/memory/sst25vf.h"

#define BOOT_BUILDVERSION   1300 // 1.0 = 1000, 1.1 = 1100

#define VECT_TAB_OFFSET  0x1000
//#define FLASH_BASE       ((u32)0x08000000)

#define BOOT_CANCEL         BIT7
#define BOOT_NOOS           BIT6
#define BOOT_BAUDRATE       230400

u16 generateBootBaud( void );
u8 bootTest( void );
u8 bootTestAndLoad( void );
void bootBootOS( void );
u32 getIDCode( void );
u8 executeCmd( u8 *ucBuffer, PYGMYCMD *pygmyCommands );

void putBufferUSART3( u16 uiLen, u8 *ucBuffer );
void putcUSART3( u8 ucChar );
void putstr( u8 *ucBuffer );
void putIntUSART3( u32 ulData );
//u8 xmodemWritePacket( u8 *ucBuffer );
//void xmodemSendPacket( u8 ucLast );
void bootGetUSART3( void );

s8 isQuote( u8 ucChar );
s8 isNewline( u8 ucChar );

u8 cmdHandler( u8 ucByte );
u8 cmdErase( u8 *ucParams );
u8 cmdFormat( u8 *ucParams );
u8 cmdRFGet( u8 *ucParams );
u8 cmdRFPut( u8 *ucParams );
u8 cmdRecv( u8 *ucParams );
u8 cmdSend( u8 *ucParams );
u8 cmdRead( u8 *ucParams );
u8 cmdRm( u8 *ucParams );
u8 cmd_cd( u8 *ucParams );
u8 cmd_new( u8 *ucBuffer );
u8 cmd_mkdir( u8 *ucBuffer );
u8 cmd_rmdir( u8 *ucBuffer );
u8 cmd_dump( u8 *ucBuffer );
u8 cmdLs( u8 *ucParams );
u8 cmdMv( u8 *ucParams );
u8 cmdBoot( u8 *ucParams );
u8 cmdFlash( u8 *ucParams );
u8 cmdVerify( u8 *ucParams );
u8 cmdReset( u8 *ucParams );
u8 cmdTest( u8 *ucParams );
u8 cmdNull( u8 *ucParams );

const u8 STRID_L15X[] = "L15x";
const u8 STRID_F100[] = "F100";
const u8 STRID_F103[] = "F103";
const u8 BOOT_OK[] = "\rOK\r";
const u8 BOOT_ERROR[] = "\rERROR\r>";
const u8 BOOT_PROMPT[] = "\r> ";
const u8 BOOT_filename[] = "boot.hex";

const PYGMYCMD BOOTCOMMANDS[] = {   {(u8*)"erase",      cmdErase},
                                    {(u8*)"format",     cmdFormat},
                                    {(u8*)"recv",       cmdRecv},
                                    {(u8*)"send",       cmdSend},
                                    {(u8*)"rfget",      cmdRFGet},
                                    {(u8*)"rfput",      cmdRFPut},
                                    {(u8*)"read",       cmdRead},
                                    {(u8*)"new",        cmd_new},
                                    {(u8*)"mkdir",      cmd_mkdir},
                                    {(u8*)"rmdir",      cmd_rmdir},
                                    {(u8*)"rm",         cmdRm},
                                    {(u8*)"cd",         cmd_cd},
                                    {(u8*)"ls",         cmdLs},
                                    {(u8*)"mv",         cmdMv},
                                    {(u8*)"boot",       cmdBoot},
                                    {(u8*)"dump",       cmd_dump},
                                    //{(u8*)"flash", cmdFlash},
                                    //{(u8*)"verify", cmdVerify},
                                    {(u8*)"reset", cmdReset},
                                    {(u8*)"", NULL }//cmdNull} // No Commands after NULL
                                    }; 

PYGMYXMODEM pygmyXModem;
PYGMYFILE pygmyFile;
u32 globalFileLen, globalXMTimeout, globalTransaction, globalXMCount;
volatile u32 globalBootTimeout;
volatile u32 globalPLL, globalID, globalXTAL, globalFreq, globalBaseAddress;
volatile u8 globalStatus = 0, globalBootStatus = 0;
volatile u8 *globalStrID;
  
void bootTimeout( void )
{
    //static u8 ucFirstCall = 0;
    
    //if( ucFirstCall ){
        bootBootOS(); 
    //} // if
    //ucFirstCall = TRUE;
}
                                
int main( void )
{   
    PYGMYFILEVOLUME *pygmyVolume;
    PYGMYFILEPROPERTIES pygmyEntry;
    u32 ulClock;
    u8 i, ucLen, *ucParams[32];
    // First test the descriptor page ( last page in FLASH )
    // if configuration exists, use programmed ID, else query
    // Debug registers for ID. This is a workaround for silicon
    // issues in the F103 STM32s
    
    globalID = fpecGetID( );
    globalPLL = BIT16|BIT1;
    // First Init the Clocks
    if( globalID == 0x0416 ){
        // L152
        globalStrID = (u8*)STRID_L15X;  
		globalXTAL = 16000000;  
        globalFreq = 32000000;
        ulClock = globalFreq;
        FPEC->ACR = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY1;
    } else if( globalID == 0x0420 || globalID == 0x0428 ){
        // F100 
        globalStrID = (u8*)STRID_F100;
        globalXTAL = 12000000;
        globalFreq = 24000000;
        ulClock = globalFreq;
    } else{
        // F103
        globalStrID = (u8*)STRID_F103;
        globalXTAL = 8000000;
        globalFreq = 72000000; //72022900;
        ulClock = 36000000;
        globalPLL = RCC_PLL_X9|BIT16|BIT15|BIT14|BIT1;//BIT10|BIT1;
        FPEC->ACR = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY2;
    } // else
    //PYGMY_RCC_HSI_ENABLE;
    PYGMY_RCC_HSE_ENABLE;
    while( !PYGMY_RCC_HSE_READY );
    RCC->CFGR2 = 0;
    RCC->CFGR = globalPLL;
    PYGMY_RCC_PLL_ENABLE;
    while( !PYGMY_RCC_PLL_READY );
    // HSI Must be ON for Flash Program/Erase Operations
    // End Clock Init 
    // Configure Interrupts
    PYGMY_WATCHDOG_UNLOCK;
    PYGMY_WATCHDOG_PRESCALER( IWDT_PREDIV128 );
    PYGMY_WATCHDOG_TIMER( 0x0FFF );
    PYGMY_WATCHDOG_START;
    PYGMY_WATCHDOG_REFRESH;
    // Do not read from NVIC LOAD or VAL!!!
    // Reading from Write only registers causes unpredictable behavior!!!
    NVIC->ISER[ 1 ] = 0x00000001 <<  7 ;
    SYSTICK->VAL = globalFreq / 500;
    SYSTICK->LOAD = globalFreq / 500; // Based on  ( 2X the System Clock ) / 1000
    SYSTICK->CTRL = 0x07;   // Enable system timer
    // End Configure Interrupts

    // Basic Port Init
    RCC->APB2ENR |= (RCC_IOPBEN|RCC_IOPCEN|RCC_IOPAEN);
    PYGMY_RCC_USART3_ENABLE;
       
    // End Basic Port Init
    
	// USART3 Init, PB10 TXD, PB11 RXD
    GPIOB->CRH &= ~( PIN10_CLEAR | PIN11_CLEAR );
    GPIOB->CRH |= ( PIN10_OUT50_ALTPUSHPULL | PIN11_IN_FLOAT );
    /*ulClock = globalFreq;
    if( ulClock > 70000000 ){
        ulClock /= 2;
    } // if
    */
    USART3->BRR = ( ( (ulClock >> 3 ) / BOOT_BAUDRATE ) << 4 ) + ( ( ( ulClock / BOOT_BAUDRATE ) ) & 0x0007 );
    USART3->CR3 = USART_ONEBITE;
    USART3->CR1 = ( USART_OVER8 | USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
    // End USART3 Init
        
    // Note: SST FLASH ICs must have write-protection removed by formatting before use
    //taskInit();
    //socketInit();
    //rfInit();
    streamInit();
    //streamSetGet( COM3, cmdGetsCOM3 );
    //streamEnableEcho( COM3 );
    //streamEnableBackspace( COM3 );
    //streamEnableActionChars( COM3 );
    //streamSetRXBuffer( COM3, globalCOM3RXBuffer, __PYGMYCOM3BUFFERLEN );
    streamSetPut( COM3, putsUSART3 );
    streamSetGet( COM3, bootGetUSART3 );
    streamDisableDefaultGet( COM3 );
    fileMount( (PYGMYMEMIO *)&SST25VF, 0, "nebula", FLASH_CS, FLASH_SCK, FLASH_MISO, FLASH_MOSI );
    //fileFormat( fileGetCurrentMountPoint( ), "nebula" );
    //fileOpen( "/nebula/test", FOLDER|WRITE|READ, 0 );
    //fileOpen( "/nebula/images", FOLDER|WRITE|READ, 0 );
    //fileOpen( "/nebula/test/test.txt", WRITE|READ, 0 );
    
    //putstr( "\rSplit: " ); putstr( splitString( "/nebula/test.txt", '/', -1 ) );
    //if( fileSeekPath( "/nebula", &pygmyEntry ) ){
    //    filePrintProperties( &pygmyEntry );
    //} // if
    /*ucLen = getAllSubStrings( "/root/test/test/test.txt", ucParams, 32, FILESEPARATORS );
    putstr( "\rParams Found: " ); putIntUSART3( ucLen );
    for( i = 0; i < ucLen; i++ ){
        putstr( "\rParam" ); putIntUSART3( i ); putstr( ": " ); putstr( ucParams[ i ] );
    } // for
    */
    //putstr( "\rPygmy Boot V2.0\rMCU " );
    print( COM3, "\rPygmy Boot V2.0\rMCU " );
    //putstr( (u8*)globalStrID );
    print( COM3, "%s\rFLASH %d\r", globalStrID, SIZEREG->Pages );
    //putstr( "\rFLASH " );
    //putIntUSART3( SIZEREG->Pages );
    //putcUSART3( '\r' );
    //fileMountRoot();
    // First allow 2 seconds for receipt of '+' char 
    // If received, cancel download sequence and wait for commands
    // Timeout is incremented every millisecond by SysTick
    
    //taskNewSimple( "xmodem", 1000, (void *)xmodemProcessTimer );
    //taskNewSimple( "boot", 5000, (void *)bootTimeout );
    
    /*for( globalBootTimeout = 0; globalBootTimeout < 2000; ){
        ;
    }
    if( !(globalBootStatus & BOOT_CANCEL) ){
        bootBootOS();   
    }*/
    while( 1 ){
        // Wait for commands
    }
}

u8 bootTest( void )
{
    // This function pre-tests every row in firmware file for corruption before erase and program
    u32 i;
    u8 ucBuffer[ 64 ], *ucSubString;//ucCalculatedSum, ;

    //if( !fileOpen( &pygmyFile, (u8*)BOOT_filename, READ ) ){
    //    return( FALSE );
    //}
 
    for( ; !( pygmyFile.Properties.Attributes & EOF ); ){
        // Get an IHEX packet
        for( i = 0; i < 64; i++ ){
            ucBuffer[ i ] = fileGetChar( &pygmyFile );
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
        //for( i = 0, ucCalculatedSum = 0; i < ucSubString[ 0 ]+4; i++ ){
        //    ucCalculatedSum += ucSubString[ i ];
        //} // for
        //ucCalculatedSum = 1 + ( 0xFF ^ (u8)ucCalculatedSum ); 
        //if( (u8)ucCalculatedSum != ucSubString[ i ] ){ // Last short is checksum
        if( sysCRC8( ucSubString, ucSubString[ 0 ]+4 ) != ucSubString[ i ] ){ // Last short is checksum
            return( FALSE ); // Corrupt HEX Row
        } // if
        i = ( (u16)ucSubString[ 1 ] << 8 ) + ucSubString[ 2 ];
        if( ucSubString[ 3 ] == IHEX_DATA && i < 0x2000 ){
            return( 0 );
        } // if 
    } // for

    //putstr( (u8*)BOOT_OK );
    print( COM3, (u8*)BOOT_OK );
    return( 1 );
}

u8 bootTestAndLoad( void )
{
    u32 i;
    //u16 *uiAddress;
    u8 ucBuffer[ 64 ], ucStatus, *ucSubString;

    //putstr( "\rFlashing" );
    print( COM3, "\rFlashing" );
    //if( !fileOpen( &pygmyFile, (u8*)BOOT_filename, READ ) ){ // If file boot.hex exists then load
    //    return( 0 );
    //} // if
    
    fpecEraseProgramMemory();
    //putstr( "..." );
    print( COM3, "..." );
    for( ucStatus = 0; !( pygmyFile.Properties.Attributes & EOF ) && ucStatus != 0xFF; ){
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

    // Write Device Descriptor to last page of FLASH
    //fpecWriteLong( uiAddress, globalID );
    //fpecWriteLong( uiAddress + 2, globalXTAL );
    //fpecWriteLong( uiAddress + 4, globalFreq );
    //fpecWriteLong( uiAddress + 6, BOOT_BUILDVERSION );
    //PYGMY_WATCHDOG_REFRESH;
    //fpecWriteDescriptor( 0, globalID );
    //fpecWriteDescriptor( 1, globalXTAL );
    //fpecWriteDescriptor( 2, globalFreq );
    //fpecWriteDescriptor( 3, BOOT_BUILDVERSION );

    return( 1 );
}

void bootBootOS( void )
{
    PYGMYVOIDPTR pygmyMain;
    u32 *ulOS;
    
    ulOS = (u32*)0x08004004; // Address is start vector table + 4 bytes
    if ( *ulOS != 0xFFFFFFFF ){
        //putstr( "\rBooting..." );
        print( COM3, "\rBooting..." );
        pygmyMain = (PYGMYVOIDPTR)*ulOS;
        RCC->CIR = 0x009F0000;
        SCB->VTOR = ((u32)0x08004000 & (u32)0x1FFFFF80);
        pygmyMain(); // pass control to Pygmy OS
    } // if
    globalBootStatus = BOOT_CANCEL;
    taskDelete( "boot" );
    //putstr( "\rNo OS\r> " );
    print( COM3, "\rNo OS> " );
} 

void putBufferUSART3( u16 uiLen, u8 *ucBuffer )
{
    u16 i;

    for( i = 0; i < uiLen ; i++ ){
        //putcUSART3( ucBuffer[ i ] ) ;
        print( COM3, "%c", ucBuffer[ i ] ) ;
    } // for   
}
/*
void putIntUSART3( u32 ulData )
{
    s32 i, iMagnitude, iValue;
    
    if( ulData & BIT31 ){
        putstr( "ERROR" );
        return;
    }  // if  
    for( i = 0, iMagnitude = 1; ( iMagnitude * 10 ) <= ulData; i++ )
        iMagnitude *= 10;
    
    for( ; i >= 0; i-- ){
        iValue = ulData / iMagnitude;
        putcUSART3( 48 + iValue ); // 48 = '0' in ASCII
        ulData -= ( iValue * iMagnitude );
        iMagnitude /= 10;
    }
    
}

void putcUSART3( u8 ucChar )
{
    while( !( USART3->SR & USART_TXE ) );
    USART3->DR = ucChar; 
}

void putstr( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        putcUSART3( *(ucBuffer++) );
    }
}*/

void bootGetUSART3( void )
//void USART3_IRQHandler( void )
{
    static u8 ucBuffer[ 256 ], ucIndex = 0;
    u8 ucByte;

    if( USART3->SR & USART_RXNE ) { 
        USART3->SR &= ~USART_RXNE;
        ucByte = USART3->DR ;
        
        if( xmodemProcess( &pygmyXModem, ucByte ) ){
            return;
        } // if
        if( ucByte == '+' ){
            taskDelete( "boot" );
            globalBootStatus = BOOT_CANCEL;
            //putstr( (u8*)BOOT_PROMPT );
            print( COM3, BOOT_PROMPT );
            return;
        } // if
        if( globalBootStatus & BOOT_CANCEL ){
            if( ucByte == '\r' ){
                ucBuffer[ ucIndex ] = '\0'; // Add NULL to terminate string
                ucIndex = 0;
                if( executeCmd( ucBuffer, (PYGMYCMD *)BOOTCOMMANDS ) ){
                    //putstr( (u8*)BOOT_PROMPT );
                    print( COM3, BOOT_PROMPT  ); 
                } else{
                    //putstr( (u8*)BOOT_ERROR );
                    print( COM3, BOOT_ERROR ); 
                } // else
            } else{
                //putcUSART3( ucByte );
                //print( COM3, "%c", ucByte );
                USART3->DR = ucByte;
                if( ucByte == '\b'  ){
                    if( ucIndex ){
                        --ucIndex;
                    } // if
                } else {
                    ucBuffer[ ucIndex++ ] = ucByte;
                } // else
            } // else
        } // if
    } // if
   
}

u8 cmd_dump( u8 *ucBuffer )
{
    PYGMYFILEVOLUME *pygmyVolume;
    u32 i, ulAddress, ulReadLen;
    
    u8 ucData, ucLen, *ucParams[3];

    pygmyVolume = fileGetCurrentMountPoint( );
    ucLen = getAllSubStrings( ucBuffer, ucParams, 3, WHITESPACE|NEWLINE );
    if( !pygmyVolume || ucLen < 2 ){
        return( FALSE );
    } // if
    
    if( isStringSameIgnoreCase( ucParams[ 0 ], "sector" ) ){
        ulAddress = convertStringToInt( ucParams[ 1 ] );
        ulReadLen = 4096;
    } else{
        ulAddress = convertStringToInt( ucParams[ 0 ] );
        ulReadLen = convertStringToInt( ucParams[ 1 ] );
    } // else
    print( COM3, "\r" );
    for( i = 0; i < ulReadLen; i++ ){
        ucData = pygmyVolume->IO->GetChar( pygmyVolume->Port, ulAddress + i );
        if( !(i % 16 ) ){
            print( COM3, "\r" );
        } // if
        if( isPrintable( ucData ) ){
            print( COM3, "\"%c\"", ucData );
        } else{
            print( COM3, "(%02X)", ucData );
        } // else
    } // for

    return( TRUE );
}

u8 cmdErase( u8 *ucParams )
{
    fpecEraseProgramMemory( );// 8, SIZEREG->Pages - 2 );
    globalStatus |= BIT0; // Mark main memory as erased

    return( 1 );
}

u8 cmdFormat( u8 *ucParams )
{
    fileFormat( globalMountPoints, "nebula" );
    //fileWriteNewTableEntry( globalMountPoints, "test", WRITE|READ, 0 );
    //fileMountRoot();

    return( 1 );
}

u8 cmdRFGet( u8 *ucParams )
{
   u8 *ucParam1, *ucParam2;
    
    ucParam1 = getNextSubString( ucParams, WHITESPACE|NEWLINE );
    ucParam2 = getNextSubString( NULL, WHITESPACE|NEWLINE );
    if( ucParam1 && ucParam2 ){
        socketRequestFile( convertStringToInt( ucParam1 ), ucParam2 );
        
        return( TRUE );
    } // if

    return( FALSE );
}

u8 cmdRFPut( u8 *ucParams )
{
    u8 *ucParam1, *ucParam2;
    
    ucParam1 = getNextSubString( ucParams, WHITESPACE|NEWLINE );
    ucParam2 = getNextSubString( NULL, WHITESPACE|NEWLINE );
    if( ucParam1 && ucParam2 ){
        socketSendFile( convertStringToInt( ucParam1 ), ucParam2 );
        
        return( TRUE );
    } // if

    return( FALSE );
}

u8 cmdRecv( u8 *ucParams )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucParams, WHITESPACE|NEWLINE );
    
    /*if( fileOpen( &pygmyFile, ucSubString, WRITE ) ){
        globalStatus |= BIT1;       // BIT1 used to mark In XModem Status
        globalXMTimeout = 1000; // 10 seconds
        globalTransaction = 60;
        globalXMCount = 1;
        return( 1 );
    } // if
        
    return( 0 );*/
    return( xmodemRecv( &pygmyXModem, ucSubString ) );
}

u8 cmdSend( u8 *ucParams )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucParams, WHITESPACE|NEWLINE );
    /*if( fileOpen( &pygmyFile, ucSubString, READ ) ){
        globalXMCount = 1;
        globalXMTimeout = 1000;
        globalTransaction = 10;
        globalStatus = ( XMODEM_SEND | XMODEM_SEND_WAIT ); 
        
        return( 1 );
     } // if
 
    return( 0 );*/
    return( xmodemSend( &pygmyXModem, ucSubString ) );
}

u8 cmdRead( u8 *ucParams )
{
    u16 i;
    u8 *ucSubString, ucChar;
    
    ucSubString = getNextSubString( ucParams, WHITESPACE|NEWLINE );
    if( ucSubString ){//&& fileOpen( &pygmyFile, ucSubString, READ ) ){
        for( i = 0; !(pygmyFile.Properties.Attributes & EOF ); i++ ){
            ucChar = fileGetChar( &pygmyFile );
            if( !(pygmyFile.Properties.Attributes & EOF ) ){
                if( ucChar == '\r' ){
                    i = 0;
                } // if
                if( !( i % 80 ) ){
                    //putcUSART3( '\r' );
                    print( COM3, "\r" );
                } // if
                if( ucChar != '\r' && ( ucChar < 32 || ucChar > 126 ) ){
                    //putcUSART3( '(' );
                    //putIntUSART3( ucChar );
                    //putcUSART3( ')' );
                    print( COM3, "(%d)", ucChar );
                } else{
                    //putcUSART3( ucChar );
                    print( COM3, "%c", ucChar );
                } // else
            } // if
        } // 
        return( 1 );
    } // if

    return( 0 );
}

u8 cmdRm( u8 *ucParams )
{   
    return( fileDelete( getNextSubString( ucParams, WHITESPACE|NEWLINE ) ) );

    //return( 0 );
}

u8 cmd_new( u8 *ucBuffer )
{
    if( fileOpen( ucBuffer, WRITE|READ, 0 ) ){
        return( TRUE );
    } 
    
    return( FALSE );
}

u8 cmd_mkdir( u8 *ucBuffer )
{
    if( fileOpen( ucBuffer, FOLDER|WRITE|READ, 0 ) ){
        return( TRUE );
    } 
    
    return( FALSE );
}

u8 cmd_rmdir( u8 *ucBuffer )
{

}

u8 cmd_cd( u8 *ucParams )
{
    u8 *ucParam;
    
    ucParam = getNextSubString( ucParams, NEWLINE );
    return( fileChangeCurrentPath( ucParam ) );
}

u8 cmdLs( u8 *ucParams )
{
    PYGMYFILEVOLUME *pygmyVolume;
    PYGMYFILEPROPERTIES pygmyEntry;
    u16 i, ii, uiID;
    u8 ucData, ucMountPoints;

    pygmyVolume = fileGetCurrentMountPoint( );
    
    /*putstr( "\rListing for " ); 
    if( !pygmyVolume ){
        putstr( "/\r" );
        ucMountPoints = fileGetMountPointCount( );
        for( i = 0; i < ucMountPoints; i++ ){
            pygmyVolume = fileGetMountPoint( i );
            putstr( "\r<MNT> " ); putstr( pygmyVolume->Properties.Name ); putstr( ", " ); putstr( pygmyVolume->Properties.Path );
        } // for
    } else{
        putstr( "\rVolPath: (" ); putstr( pygmyVolume->Properties.Path ); putstr( ")\r" );
        //filePrintProperties( &pygmyVolume->Properties );
        for( i = 0; i < 255; i++ ){
            if( i == 0 ){
                if( !fileList( pygmyVolume->Properties.Path, &pygmyEntry ) ){
                    break;
                } // if
            } else{
                if( !fileList( NULL, &pygmyEntry ) ){
                    break;
                } // if
            } // else
            if( pygmyEntry.Attributes & FOLDER ){
                putstr( "\r<DIR> " );
            } else{
                putstr( "\r      " );
            } // else
            putstr( pygmyEntry.Name );
        
        } // for
    } // else
    */
    //putcUSART3( '\r' );
    print( COM3, "\r" );
    for( i = 0; i < 6000; i++ ){
        ucData = globalMountPoints->IO->GetChar( globalMountPoints->Port, i );
        if( !( i % 16 ) ){
            print( COM3, "\r%d ", i / 16 );
            //putcUSART3( '\r' );
            //putIntUSART3( i / 16 );
            //putcUSART3( ' ' );
        } // if
        if( isPrintable( ucData ) ){
            //putcUSART3( ucData );
            print( COM3, "%c", ucData );
        } else if( ucData != 0xFF ){
            print( COM3, "(%d)", ucData );
            //putcUSART3( '(' );
            //putIntUSART3( ucData );
            //putcUSART3( ')' );
        } // else if
    } // for
    //putstr( "\r\rFree:\t\t" );
    //putIntUSART3( fileGetFreeSpace( globalMountPoints ) );
    //putcUSART3( '\r' );
    
    return( TRUE );
}

u8 cmdMv( u8 *ucParams )
{
    return( fileRename( getNextSubString( ucParams, WHITESPACE|NEWLINE ), getNextSubString( "", WHITESPACE|NEWLINE ) ) );
}

u8 cmdReset( u8 *ucParams )
{
    PYGMY_RESET;

    // No return after reset
}

u8 cmdBoot( u8 *ucParams )
{
    bootTestAndLoad();
    PYGMY_RESET;
    
    // No return after reset
}
/*
u8 cmdFlash( u8 *ucParams )
{
    return( bootTestAndLoad() );

    //return( 1 );
}

u8 cmdNull( u8 *ucParams )
{
    return( 0 );
}*/

u8 executeCmd( u8 *ucBuffer, PYGMYCMD *pygmyCommands )
{
    u16 i;
    u8 *ucCommand;
    
    ucCommand = getNextSubString( ucBuffer, WHITESPACE | PUNCT );
        
    for( i = 0; 1; i++ ){ 
        if( isStringSame( "", pygmyCommands[ i ].Name ) ){
            return( 0 );
        } // if
        if( isStringSame( ucCommand, pygmyCommands[ i ].Name ) ){
            if( pygmyCommands[ i ].Call( "" ) ){
                return( 1 );
            } else{
                return( 0 );
            } // else
        }// if
    } // for

    return( 0 );
}
