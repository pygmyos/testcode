/**************************************************************************
    PygmyOS ( Pygmy Operating System ) - BootLoader
    Copyright (C) 2011  Warren D Greenway

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
#include "pygmy_fpec.h"

#define VECT_TAB_OFFSET  0x1000
//#define FLASH_BASE       ((u32)0x08000000)

#define XMODEM_ACK          0x06
#define XMODEM_NACK         0x15
#define XMODEM_EOT          0x04
//#define XMODEM_SOH          0x01
#define XMODEM_CAN          0x18
#define XMODEM_NEXT         0x00
#define XMODEM_LAST         0x01
#define XMODEM_RECV         BIT1
#define XMODEM_MODE_SOH     BIT2
#define XMODEM_SEND         BIT3
#define XMODEM_SEND_WAIT    BIT4
#define XMODEM_SEND_EOT     BIT5
#define BOOT_CANCEL         BIT7
#define BOOT_NOOS           BIT6

#define BOOT_BAUDRATE       230400

u16 generateBootBaud( void );
u8 bootTest( void );
void bootTestAndLoad( void );
void bootBootOS( void );
u32 getIDCode( void );

void putBufferUSART3( u16 uiLen, u8 *ucBuffer );
void putcUSART3( u8 ucChar );
void putstr( u8 *ucBuffer );
void putIntUSART3( u32 ulData );
u8 xmodemWritePacket( u8 *ucBuffer );
void xmodemSendPacket( u8 ucLast );
u8 cmdHandler( u8 ucByte );

u8 cmdErase( u8 *ucParams );
u8 cmdFormat( u8 *ucParams );
u8 cmdRecv( u8 *ucParams );
u8 cmdSend( u8 *ucParams );
u8 cmdRead( u8 *ucParams );
u8 cmdRm( u8 *ucParams );
u8 cmdLs( u8 *ucParams );
u8 cmdMv( u8 *ucParams );
u8 cmdBoot( u8 *ucParams );
u8 cmdFlash( u8 *ucParams );
u8 cmdReset( u8 *ucParams );
u8 cmdTest( u8 *ucParams );
u8 cmdNull( u8 *ucParams );

const u8 STRID_L15X[] = "L15x";
const u8 STRID_F100[] = "F100";
const u8 STRID_F103[] = "F103";
const u8 BOOT_OK[] = "\rOK\r";
const u8 BOOT_ERROR[] = "\rERROR\r";

const u8 BOOT_filename[] = "boot.hex";
const char BOOT_greeting[] = "\r\rPygmyOS Loader V1\r+ cancels boot\r\r> ";

const PYGMYCMD BOOTCOMMANDS[] = {   {(u8*)"erase", cmdErase},
                                    {(u8*)"format", cmdFormat},
                                    {(u8*)"recv", cmdRecv},
                                    {(u8*)"send", cmdSend},
                                    {(u8*)"read", cmdRead},
                                    {(u8*)"rm", cmdRm},
                                    {(u8*)"ls", cmdLs},
                                    {(u8*)"mv", cmdMv},
                                    {(u8*)"boot", cmdBoot},
                                    {(u8*)"flash", cmdFlash},
                                    {(u8*)"reset", cmdReset},
                                    {(u8*)"", cmdNull} // No Commands after NULL
                                    }; 

PYGMYFILE pygmyFile;
u32 globalFileLen, globalXMTimeout, globalTransaction, globalXMCount;
u32 globalBootTimeout = 0;
u32 globalPLL, globalID, globalXTAL, globalFreq;
u8 globalStatus = 0, globalBootStatus = 0;
 


int main( void )
{
    u8 *strID;

    //DEBUGMCU->CR = 0x00000000;




    /*globalID = getIDCode( );
    globalXTAL = 8000000;
    if( globalID == 0x0416 ){
        // L152
        strID = (u8*)STRID_L15X;
        

        globalFreq = 32000000;
        globalPLL = RCC_PLL_X4|BIT16|BIT1;;
        FPEC->ACR = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY1;
    } else if( globalID == 0x0420 || globalID == 0x0428 ){
        // F100 
        strID = (u8*)STRID_F100;
        globalXTAL = 12000000;
        globalFreq = 24000000;
        globalPLL = BIT16|BIT1;
    } else{
        // F103
        strID = (u8*)STRID_F103;
        globalFreq = 72000000;
        globalPLL = RCC_PLL_X9|BIT16|BIT15|BIT14|BIT10|BIT1;
        //globalPLL = RCC_PLL_X3|BIT16|BIT1;
        FPEC->ACR = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY2;
    } // else
    */
    strID = (u8*)STRID_F100;
        globalXTAL = 12000000;
        globalFreq = 24000000;
        globalPLL = BIT16|BIT1;
    // First Init the Clocks
        PYGMY_RCC_HSE_ENABLE;
        while( !PYGMY_RCC_HSE_READY ){;}
        //GPIOA->CRH &= ~(PIN12_CLEAR|PIN11_CLEAR);
        //GPIOA->CRH |= (PIN12_OUT50_GPPUSHPULL|PIN11_OUT50_GPPUSHPULL);
        //GPIOA->BSRR = BIT11;
        
        // The following PLL Multiplier * XTAL must not exceed 24MHz for 100 Series MCU 
        //RCC->CFGR2 = 0; // Clock Prediv // Only if prescale is used with F100

        RCC->CFGR = globalPLL;
        PYGMY_RCC_PLL_ENABLE;
        while( !PYGMY_RCC_PLL_READY ){;}
        //GPIOA->BSRR = BIT12;
        // HSI Must be ON for Flash Program/Erase Operations
        PYGMY_RCC_HSI_ENABLE;
    // End Clock Init 

    // Configure Interrupts
        PYGMY_WATCHDOG_UNLOCK;
        PYGMY_WATCHDOG_PRESCALER( IWDT_PREDIV128 );
        PYGMY_WATCHDOG_TIMER( 0x0FFF );
        PYGMY_WATCHDOG_START;
        PYGMY_WATCHDOG_REFRESH;
        

        NVIC->ISER[ 1 ] = 0x00000001 <<  7 ;

        SYSTICK->LOAD = ( globalFreq * 2 ) / 1000; // Based on  ( 2X the System Clock ) / 1000
        SYSTICK->VAL = SYSTICK->LOAD;
        SYSTICK->CTRL = 0x07;   // Enable system timer
    // End Configure Interrupts

    // Basic Port Init
        RCC->APB2ENR |= (RCC_IOPBEN|RCC_IOPCEN|RCC_IOPAEN);
        PYGMY_RCC_USART3_ENABLE;


        FLASH_CS_INIT;
        SPI_SCK_INIT;
        SPI_MISO_INIT;
        SPI_MOSI_INIT;


    // End Basic Port Init
    // USART3 Init, PB10 TXD, PB11 RXD
        GPIOB->CRH &= ~( PIN10_CLEAR | PIN11_CLEAR );
        GPIOB->CRH |= ( PIN10_OUT50_ALTPUSHPULL | PIN11_IN_FLOAT );
            
        USART3->BRR = generateBootBaud( );
        USART3->CR3 |= USART_ONEBITE;
        USART3->CR1 = ( USART_OVER8 | USART_UE | USART_RXNEIE | USART_TE | USART_RE  );


    // End USART3 Init
        
    // Note: SST FLASH ICs must have write-protection removed by formatting before use
    //GPIOA->BRR = BIT11;

    fileMountRoot();
    //GPIOA->BRR = BIT12;
    //putstr( (u8*)BOOT_greeting );
    putstr( "\r\rPygmyOS Loader V1\rMCU " ); 
    putstr( strID );
    putstr( "\rFLASH " );
    putIntUSART3( SIZEREG->Pages );
    putstr( "KB\r+ cancels boot\r\r> " ); 

    
    // First allow 2 seconds for receipt of '+' char 
    // If received, cancel download sequence and wait for commands
    // Timeout is incremented every millisecond by SysTick
    






    while( 1 ){ ; }


}

u32 getIDCode( void )
{
    // Returns the device ID containing type/family
    // Low Power devices = 0x0416
    // Low density devices = 0x412
    // Mediumdensity devices = 0x410
    // High density devices = 0x414
    // XL density devices = 0x430
    // Connectivity devices = 0x418
    // Low and medium density value line devices = 0x420
    // High density value line devices = 0x428
    u32 *ulID;
    
    ulID = (u32*)0xE0042000; // Address of 32bit ID and revision code in DBG regs
    
    return( *ulID & 0x00000FFF );
}

u16 generateBootBaud( void )
{ 
    // USARTDIV = DIV_Mantissa + (DIV_Fraction / 8 × (2 – OVER8))
    u32 uiBRR, ulClock;;
    
    // APB bus is limited to 36MHz, if clock is above that then div by 4


    ulClock = globalFreq;
    if( ulClock == 72000000 ){
        ulClock /= 4;
    } // if
    uiBRR = ( ( ( ulClock >> 3 ) / BOOT_BAUDRATE ) << 4 ) + ( ( ( ulClock / BOOT_BAUDRATE ) ) & 0x0007 );

    return( (u16)uiBRR ); 
}

u8 bootTest( void )
{
    // This function pre-tests every row in firmware file for corruption before erase and program
    u32 i;
    u8 ucBuffer[ 64 ], ucCalculatedSum, *ucSubString;

    if( !fileOpen( &pygmyFile, (u8*)BOOT_filename, READ ) ){
        //putstr( "\rNo boot.hex" );
        return( 0 );
    }
 
    for( ; !( pygmyFile.Attributes & EOF ); ){
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
            putstr( (u8*)BOOT_ERROR );
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
            putstr( (u8*)BOOT_ERROR );
            return( 0 ); // Corrupt HEX Row
        } // if
        i = ( (u16)ucSubString[ 1 ] << 8 ) + ucSubString[ 2 ];
        if( ucSubString[ 3 ] == IHEX_DATA && i < 0x2000 ){
            putstr( (u8*)BOOT_ERROR );
            return( 0 );
        } // if 
    } // for

    return( 1 );
}

void bootTestAndLoad( void )
{
    u32 i;
    u8 ucBuffer[ 64 ], ucStatus, *ucSubString;

    //putstr( "\rFlashing" );
    
    if( fileOpen( &pygmyFile, (u8*)BOOT_filename, READ ) ){ // If file boot.hex exists then load
        fpecUnlock();
        if ( !fpecEraseProgramMemory( 8 ) ){ // Always erase all pages before attempting a write
            putstr( (u8*)BOOT_ERROR );
            return;
        } // if

        for( ucStatus = 0; !( pygmyFile.Attributes & EOF ) && ucStatus != 0xFF; ){
            for( i = 0; i < 64; i++ ){
                ucBuffer[ i ] = fileGetChar( &pygmyFile );
                if( ucBuffer[ i ] == '\r' ){
                    ucBuffer[ i ] = '\0';
                    ucSubString = getNextSubString( (u8*)ucBuffer, WHITESPACE|NEWLINE );
                    putstr( "\r" );
                    putstr( ucSubString );
                    // Add 1 to pointer before passing to skip the ':' packet start char
                    ucStatus = fpecProcessIHEX( (u8*)( ucSubString + 1 ) );
                    if( ucStatus == 0xFF ){
                        putstr( "\rEOF" );
                    } // if
                    break; // Time to fetch next IHEX entry
                } // if
            } // for
        } // for
    } // if
    
}

void bootBootOS( void )
{
    //PYGMYVOIDPTR pygmyMain;
    u32 *ulOS;
    
    putstr( "\rBoot..." );
    ulOS = (u32*)0x08002004; // Address is start vector table + 4 bytes
    //pygmyMain = (PYGMYVOIDPTR)*ulOS;
    if ( *ulOS != 0xFFFFFFFF ){
        //putstr( "OS Found" );
        RCC->CIR = 0x009F0000;
        SCB->VTOR = ((u32)0x08002000 & (u32)0x1FFFFF80);//FLASH_BASE | VECT_TAB_OFFSET;
        goto *ulOS;//pygmyMain; // pass control to Pygmy OS
    } // if
    //putstr( "\rNo OS\r> " );
} 

void putBufferUSART3( u16 uiLen, u8 *ucBuffer )
{
    u16 i;

    for( i = 0; i < uiLen ; i++ ){
        putcUSART3( ucBuffer[ i ] ) ;
    }    
}

void putIntUSART3( u32 ulData )
{
    s32 i, iMagnitude, iValue;

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
    
    USART3->DR = ucChar; 
    while( !( USART3->SR & USART_TXE ) );

}

void putstr( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        putcUSART3( *(ucBuffer++) );
    } // for
    while( !( USART3->SR & USART_TXE ) );
    //putcUSART3( '.' );

}

void USART3_IRQHandler( void )
{
    static u8 ucBuffer[ 256 ], ucIndex = 0;
    //u16 i, uiID;
    u8 ucByte;//, *ucSubString;//, *ucParam1;

    if( USART3->SR & USART_RXNE ) { 
        USART3->SR &= ~USART_RXNE;
        ucByte = USART3->DR ;
    
        if( globalStatus & XMODEM_RECV ){ // RECV      
            if( globalStatus & XMODEM_MODE_SOH ){ // Recieving Payload, after <SOH>
                ucBuffer[ ucIndex++ ] = ucByte;
                if( ucIndex == 131 ){
                    globalStatus &= ~XMODEM_MODE_SOH; // Reset Packet Marker
                    if( globalXMCount == ucBuffer[ 0 ] && 
                        xmodemWritePacket( (u8*)(ucBuffer+2) ) ) { // Returns 1 if check sum correct, 0 if not
                        if( globalXMCount + 1 == 256 ){
                            globalXMCount = 0;
                        } else{
                            ++globalXMCount;
                        } // else

                        putcUSART3( 0x06 );// Return ACK
                    } else{
                        putcUSART3( 0x15 );// Return NACK
                    } // else
                } // if


            } else{
                if( ucByte == 0x01 ){ // This is XModem <SOH>
                    globalStatus |= XMODEM_MODE_SOH; // Set Packet Marker
                    globalXMTimeout = 1000;
                    globalTransaction = 60;
                } else if( ucByte == 0x04 ){ // 0x04 Marks End Of Transmission              
                    globalStatus &= ~( XMODEM_RECV );
                    filePutBuffer( &pygmyFile, 128, ucBuffer );
                    fileClose( &pygmyFile );
                    putcUSART3( 0x06 ); // Send Ack to close connection
                    putstr( "Done\r> " );
                } // else if

                ucIndex = 0;
            } // else

        } else if( globalStatus & XMODEM_SEND ){ // SEND
            if( ucByte == XMODEM_ACK ){
                if( globalStatus & XMODEM_SEND_EOT ){
                    globalStatus &= ~( XMODEM_SEND_EOT|XMODEM_SEND);
                    putstr( (u8*)BOOT_OK );
                    return;
                } // if
                globalXMTimeout = 1000;
                globalTransaction = 10;
                if( pygmyFile.Attributes & EOF ){
                    globalStatus |= XMODEM_SEND_EOT;
                    putcUSART3( XMODEM_EOT );
                } else{
                    ++globalXMCount;
                    xmodemSendPacket( XMODEM_NEXT ); // 0 = next pack
                } // else
            } else if( ucByte == XMODEM_NACK ){
                globalXMTimeout = 1000;
                globalTransaction = 10;
                if( globalStatus & XMODEM_SEND_WAIT ){
                    xmodemSendPacket( XMODEM_NEXT ); // 1 = next, in this case this is first
                    globalStatus &= ~XMODEM_SEND_WAIT;
                } else{
                    xmodemSendPacket( XMODEM_LAST ); // 0 = next pack
                } // else
            } // else if
        
        } else if( ucByte == '\r' || ( ucByte == '+' && !( globalBootStatus & BOOT_CANCEL ) ) ){

            ucBuffer[ ucIndex ] = '\0'; // Add NULL to terminate string
            ucIndex = 0;
            if( ucByte == '+' ){
                putstr( "\rBoot Canceled\r> " );
                globalBootStatus |= BOOT_CANCEL;
                return;
            } // if


            if( executeCmd( ucBuffer, BOOTCOMMANDS ) ){
                putstr( "\r> " );
            } else{
                putstr( "\rERROR\r> " );
            } // else
        } else{
            putcUSART3( ucByte );
            if( ucByte == '\b'  ){
                if( ucIndex ){
                    --ucIndex;
                } // if
            } else { 
                // Recieved backspace, rollback index
                ucBuffer[ ucIndex++ ] = ucByte;
            } // else
        } // else

    } // if
   
}

u8 xmodemWritePacket( u8 *ucBuffer )
{
    u8 i, ucSum;

    // count ( 0-1 before calling this function ) must be validated by calling function 
    // 0-127 are data bytes, 128 is checksum 
    ucSum = ucBuffer[ 0 ];
    for( i = 1; i < 128; i++ ){
        ucSum += ucBuffer[ i ]; 
    } // for
    if( ucSum == ucBuffer[ 128 ] ){
        filePutBuffer( &pygmyFile, 128, ucBuffer );
 
        return( 1 );
    } // if


    return( 0 );
}

void xmodemSendPacket( u8 ucLast )
{
    static u8 ucBuffer[ 132 ];
    u8 i, ucSum;
    
    if( !ucLast ){ // If not resend then make a new packet
        ucBuffer[ 0 ] = 0x01; // <SOH>
        ucBuffer[ 1 ] = globalXMCount; // Packet Count
        ucBuffer[ 2 ] = 0xFF - globalXMCount; // Ones Complement of Count
        for( ucSum = 0, i = 3; i < 131; i++ ){ // 128 bytes payload
            ucBuffer[ i ] = fileGetChar( &pygmyFile );
            ucSum += ucBuffer[ i ];
        } // for
        ucBuffer[ i ] = ucSum; // add the check sum as the final bytes    
    } // if
    
    putBufferUSART3( 132, ucBuffer );
}

void SysTick_Handler( void )
{   
    PYGMY_WATCHDOG_REFRESH;
    
    if( ( globalBootTimeout > 2000 ) && !( globalBootStatus & BOOT_CANCEL ) ){
        globalBootStatus |= BOOT_CANCEL;
        bootBootOS(); 
    } else{
        ++globalBootTimeout;
    } // else
   

    if( globalStatus & XMODEM_RECV ){
        if( globalXMTimeout )
            --globalXMTimeout;
        else{
            putcUSART3( XMODEM_NACK );
            globalXMTimeout = 1000;
            if( globalTransaction )
                --globalTransaction;
            else{
                globalStatus &= ~(XMODEM_RECV|XMODEM_MODE_SOH);
                putstr( "XM Timeout\r> " );
            } // lse
        } // else
    } else if( globalStatus & XMODEM_SEND ){
        if( globalXMTimeout )
            --globalXMTimeout;
        else{
            xmodemSendPacket( XMODEM_LAST );
            globalXMTimeout = 1000;
            if( globalTransaction )
                --globalTransaction;
            else{
                globalStatus &= ~(XMODEM_SEND|XMODEM_SEND_EOT);
                putstr( (u8*)BOOT_ERROR );
            } // else
        } // else
    } // else if

}

u8 cmdErase( u8 *ucParams )
{
    fpecUnlock();
    fpecEraseProgramMemory( 8 );
    globalStatus |= BIT0; // Mark main memory as erased

    return( 1 );
}

u8 cmdFormat( u8 *ucParams )
{
    fileFormat( "" );
    fileMountRoot();

    return( 1 );
}

u8 cmdRecv( u8 *ucParams )
{
    // typing recv cancels global timeout and prevents boot
    // starting a data transfer with a pending reboot could lead
    // to filesystem corruption

    u8 *ucSubString;
    
    globalBootStatus |= BOOT_CANCEL;
    ucSubString = getNextSubString( ucParams, WHITESPACE|NEWLINE );
    if( fileOpen( &pygmyFile, ucSubString, WRITE ) ){
        globalStatus |= BIT1;       // BIT1 used to mark In XModem Status
        globalXMTimeout = 1000; // 10 seconds
        globalTransaction = 60;
        globalXMCount = 1;
        return( 1 );
    } 

        
    return( 0 );
}

u8 cmdSend( u8 *ucParams )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucParams, WHITESPACE|NEWLINE );
    if( fileOpen( &pygmyFile, ucSubString, READ ) ){
        globalXMCount = 1;
        globalXMTimeout = 1000;
        globalTransaction = 10;
        globalStatus |= ( XMODEM_SEND | XMODEM_SEND_WAIT ); 
        
        return( 1 );
     } // if
 
    return( 0 );
}

u8 cmdRead( u8 *ucParams )
{
    u16 i;
    u8 *ucSubString, ucChar;
    
    ucSubString = getNextSubString( ucParams, WHITESPACE|NEWLINE );
    if( ucSubString && fileOpen( &pygmyFile, ucSubString, READ ) ){
        for( i = 0; !(pygmyFile.Attributes & EOF ); i++ ){
            ucChar = fileGetChar( &pygmyFile );
            if( !(pygmyFile.Attributes & EOF ) ){
                if( ucChar == '\r' ){
                    i = 0;
                } // if
                if( !( i % 80 ) ){
                    putcUSART3( '\r' );
                } // if
                if( ucChar != '\r' && ( ucChar < 32 || ucChar > 126 ) ){
                    putcUSART3( '(' );
                    putIntUSART3( ucChar );
                    putcUSART3( ')' );
                } else{
                    putcUSART3( ucChar );
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

    return( 0 );
}

u8 cmdLs( u8 *ucParams )
{
    u16 i, uiID;
  
    for( i = 0; i < pygmyRootVolume.MaxFiles; i++ ){
        uiID = fileGetName( i, pygmyFile.Name );
        if( uiID ){
            putcUSART3( '\r' );
            putstr( pygmyFile.Name );
            putstr( "\t\t" );

            putIntUSART3( fileGetLength( uiID ) );
        } // if
    } // for
    putstr( "\rFree:\t\t" );
    putIntUSART3( fileGetFreeSpace() );
    //putcUSART3( '\r' );

    return( 1 );
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
    if( bootTest() ){
        bootTestAndLoad();
    } // if
    PYGMY_RESET;
    
    // No return after reset
}

u8 cmdFlash( u8 *ucParams )
{
    if( bootTest() ){
        bootTestAndLoad();
    } // if

    return( 1 );
}
/*
u8 cmdTest( u8 *ucParams )
{
    // The first file entry is the storage descriptor which is formatted as follows:
    //    [ NAME 13B ][ ATTRIB B ][ SECTORS 2B ][ SECTORSIZE 2B ][ MAXFILES 4B ][ MEDIALEN 4B ]
    // The second file entry is the table pointers, which is formatted as follows
    //    [ FATPERSECTOR 2B ][ FILESSECTORS 2B ][ FATSECTORS 2B ][ FIRSTFILESECTOR 4B ][ FILES_A 4B ][ FILES_B 4B ][ FAT_A 4B ][ FAT_B 4B ] 
   // putstr( "\rAttr: " );
    //putIntUSART3( flashReadByte( PYGMY_FILE_VOLUME_FIELD_ATTRIB ) );
    //putstr( "\rSec: " );
    putIntUSART3( flashReadWord( PYGMY_FILE_VOLUME_FIELD_SECTORS ) );
    //putstr( "\rFiles: " );
    putIntUSART3( flashReadLong( PYGMY_FILE_VOLUME_FIELD_MAXFILES ) );
    //putstr( "\rCap: " );
    putIntUSART3( flashReadLong( PYGMY_FILE_VOLUME_FIELD_MEDIASIZE ) );
    //putstr( "\rFAT: " );
    putIntUSART3( flashReadWord( PYGMY_FILE_VOLUME_FIELD_ENTRIESPERFAT ) );
    //putstr( "\rFSec: " );
    putIntUSART3( flashReadWord( PYGMY_FILE_VOLUME_FIELD_FILESSECTORS ) );
    //putstr( "\rFATSec: " );
    putIntUSART3( flashReadWord( PYGMY_FILE_VOLUME_FIELD_FATSECTORS ) );
    //putstr( "\rFirstFSec: " );
    putIntUSART3( flashReadLong( PYGMY_FILE_VOLUME_FIELD_FIRSTFILESECTOR ) );
    //putstr( "\rFilesA: " );
    putIntUSART3( flashReadLong( PYGMY_FILE_VOLUME_FIELD_FILES_A ) );
    //putstr( "\rFilesB: " );
    putIntUSART3( flashReadLong( PYGMY_FILE_VOLUME_FIELD_FILES_B ) );
    //putstr( "\rFATA: " );
    putIntUSART3( flashReadLong( PYGMY_FILE_VOLUME_FIELD_FAT_A ) );
    //putstr( "\rFATB: " );
    putIntUSART3( flashReadLong( PYGMY_FILE_VOLUME_FIELD_FAT_B ) );
    //putstr( "\r" );
    
    return( 1 );
}*/

u8 cmdNull( u8 *ucParams )
{
    return( 0 );
}

u8 executeCmd( u8 *ucBuffer, PYGMYCMD *pygmyCommands )
{
    u16 i;
    u8 *ucCommand;
    
    ucCommand = getNextSubString( ucBuffer, WHITESPACE | PUNCT );
    if( !ucCommand ){
        return( 0 );
    } // if
        
    for( i = 0; 1; i++ ){ 
        if( isStringSame( "", pygmyCommands[ i ].Name ) ){
            return( 0 );
        } // if
        if( isStringSame( ucCommand, pygmyCommands[ i ].Name ) ){
            if( pygmyCommands[ i ].Call( "" ) ){
                return( 1 );
            }else{

                return( 0 );
            } // else
        }// if
    } // for

    return( 0 );
}

