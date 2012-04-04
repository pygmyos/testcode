/****************************************************************************
*  Copyright (c) 2011 by Michael Fischer. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
****************************************************************************
*  History:
*
*  09.04.2011  mifi  First Version
*  29.04.2011  mifi  Call SystemInit, and set the Vector Table Offset
*                    before copy of data and bss segment.
****************************************************************************/
#define __CRT_C__

#include <stdint.h>

/*=========================================================================*/
/*  DEFINE: All extern Data                                                */
/*=========================================================================*/
/*
 * The next data are defined by the linker script.
 */
extern unsigned long _stext;
extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _estack;

extern int main (void);

void SystemInit (void) __attribute__((weak));

void SystemInit (void)
{
    // ToDO: Add any system init code here
}

// MCU calls on reset, place RAM init code, etc. in this function before call to main()

void ResetHandler (void)
{
   uint32_t *pSrc;
   uint32_t *pDest;
   
   SystemInit();    
   
   *((uint32_t*)0xE000ED08) = (uint32_t)&_stext; // Set Vector Offset Register
   
    // Copy the initialized data from Flash to RAM
    pSrc  = &_etext;
    pDest = &_sdata;
    while(pDest < &_edata){
        *pDest++ = *pSrc++;
    }
   
    //Clear the ".bss" segment.
    pDest = &_sbss;
    while(pDest < &_ebss){
        *pDest++ = 0;
    }
     
    main();    
   
    while(1) {};    

}

