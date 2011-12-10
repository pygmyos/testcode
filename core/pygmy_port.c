/**************************************************************************
    PygmyOS ( Pygmy Operating System )
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

// Timer1 CH1, PA8 is the dedicated MasterClock Out
// Timer6 and 7 are used for the DACs
// Remaining Timers are used by DMA and software for general timing

PYGMYVOIDPTR globalIRQHandlers[ 16 ];

const u8 PYGMY_TIMERPINS[] = {  PA8,  PA9,  PA10, PA11,     // TIM1
                                PA0,  PA1,  PA2,  PA3,      // TIM2
                                PA6,  PA7,  PB0,  PB1,      // TIM3
                                PB6,  PB7,  PB8,  PB9  };   // TIM4

void pinConfig( u8 ucPin, u8 ucMode )
{
    u32 uiPortMode, uiPortClear;
    u16 uiPin;
    GPIO *pygmyPort;

    // Port registers are broken into 8 pins each in the STM32
    // CRL contains the config bits for 0-7
    // CRH contains the config bits for 8-15
    // each bit is configured with 4 bits
    enablePortClock( ucPin ); // Enable clock BEFORE accessing registers
    pygmyPort = getPortFromPin( ucPin );
    uiPortMode = ( PIN_CLEAR & ucMode) << ( ( ucPin % 8 ) * 4 );
    uiPortClear = ~( PIN_CLEAR << ( ( ucPin % 8 ) * 4 ) ); // Clear before setting bits
    uiPin = ucPin % 16;
    if( uiPin < 8 ){
        pygmyPort->CRL &= uiPortClear;
        pygmyPort->CRL |= uiPortMode;
    } else{
        pygmyPort->CRH &= uiPortClear;
        pygmyPort->CRH |= uiPortMode;
    } // else
    
    if( ucMode & PULLUP ){
        pinSet( ucPin, HIGH );
    } else if( ucMode & PULLDOWN ){
        pinSet( ucPin, LOW );
    } // else if
}


void pinInterrupt( PYGMYVOIDPTR pygmyFunc, u8 ucPin, u16 uiMode )
{
    // This function configures a GPIO pin to generate interrupts
    // and wake events. This function does NOT enable the interrupt
    // in the NVIC. EXTI0 is for Pin0 on all Ports, EXTI1 for Pin1, etc.
    

    u16 uiMask, uiPort, uiPin;
    
    PYGMY_RCC_AFIO_ENABLE;
    uiPort = ucPin / 16;
    uiPin = ucPin % 16;
    globalIRQHandlers[ uiPin ] = pygmyFunc;
    switch( uiPin ){
        case 0:
            enableInterrupt( EXTI0_IRQ );
            break;
        case 1:
            enableInterrupt( EXTI1_IRQ );
            break;
        case 2:
            enableInterrupt( EXTI2_IRQ );
            break;
        case 3:
            enableInterrupt( EXTI3_IRQ );
            break;
        case 4:
            enableInterrupt( EXTI4_IRQ );
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            enableInterrupt( EXTI9_5_IRQ );
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            enableInterrupt( EXTI15_10_IRQ );
            break;
    } // switch
    uiMask = ( 0x0001 << uiPin );
    AFIO->EXTICR[ uiPin / 4 ] |= ( uiPort << ( ( uiPin % 4 ) * 4 ) );   // Select Port for EXTI Line applicable to Pin
    EXTI->IMR |= uiMask;                                                // Enable Interrupt for Pin
    if( uiMode & TRIGGER_FALLING ){                                     // Enable Falling Edge Trigger
        EXTI->FTSR |= uiMask;
    } // if
    if( uiMode & TRIGGER_RISING ){                                      // Enable Rising Edge Trigger
        EXTI->RTSR |= uiMask;                                           // Both triggers may be enabled 
    } // if
    if( uiMode & TRIGGER_WAKE ){                                        // Enable Wake Events
        EXTI->EMR |= uiMask;
    } // if
    EXTI->PR = 0xFFFFFFFF;
    EXTI->SWIER = 0x00000000;
}

void enablePortClock( u8 ucPin )
{
    if( ucPin < 16 ){
        PYGMY_RCC_GPIOA_ENABLE;
    } else if( ucPin < 32 ){
        PYGMY_RCC_GPIOB_ENABLE;
    } else if( ucPin < 48 ){
        PYGMY_RCC_GPIOC_ENABLE;
    } else if( ucPin < 64 ){
        PYGMY_RCC_GPIOD_ENABLE;
    } else if( ucPin < 80 ){
        PYGMY_RCC_GPIOE_ENABLE;
    } else if( ucPin < 96 ){
        PYGMY_RCC_GPIOF_ENABLE;
    }
}

GPIO *getPortFromPin( u8 ucPin )
{
    if( ucPin < 16 ){
        return( GPIOA );
    } else if( ucPin < 32 ){
        return( GPIOB );
    } else if( ucPin < 48 ){
        return( GPIOC );
    } else if( ucPin < 64 ){
        return( GPIOD );
    } else if( ucPin < 80 ){
        return( GPIOE );
    } // else if
        
    #ifdef GPIOF
        return( GPIOF );
    #endif
    return( NULL );
}

void pinSet( u8 ucPin, u8 ucState )
{
    u16 uiPinMask, uiPinState;

    uiPinMask = BIT0 << ( ucPin % 16 );
    uiPinState = 0;
    if( ucState ){
        uiPinState = uiPinMask;
    } // if
    if( ucPin < 16 ){
        GPIOA->ODR &= ~uiPinMask;
        GPIOA->ODR |= uiPinState;
    } else if( ucPin < 32 ){
        GPIOB->ODR &= ~uiPinMask;
        GPIOB->ODR |= uiPinState; 
    } else if( ucPin < 48 ){
        GPIOC->ODR &= ~uiPinMask;
        GPIOC->ODR |= uiPinState;
    } else if( ucPin < 64 ){
        GPIOD->ODR &= ~uiPinMask;
        GPIOD->ODR |= uiPinState;
    } else if( ucPin < 80 ){
        GPIOE->ODR &= ~uiPinMask;
        GPIOE->ODR |= uiPinState;
    }
    #ifdef GPIOF
        if( ucPin < 96 ){
            GPIOF->ODR &= ~uiPinMask;
            GPIOF->ODR |= uiPinState;
        } // else if
    #endif
}

u8 pinGet( u8 ucPin )
{
    u16 uiPinMask;
    
    uiPinMask = BIT0 << ( ucPin % 16 );
    if( ucPin < 16 ){
        uiPinMask &= GPIOA->IDR;
    } else if( ucPin < 32 ){
        uiPinMask &= GPIOB->IDR;
    } else if( ucPin < 48 ){
        uiPinMask &= GPIOC->IDR;
    } else if( ucPin < 64 ){
        uiPinMask &= GPIOD->IDR;
    } else if( ucPin < 80 ){
        uiPinMask &= GPIOE->IDR;
    } // else if 
    #ifdef GPIOF
        if( ucPin < 96 ){
            uiPinMask &= GPIOF->IDR;
        } // else if
    #endif
    
    if( uiPinMask ){
        return( HIGH );
    } else{
        return( LOW );
    } // else
}

void *getTimerFromPin( u8 ucPin )
{
    u16 i, ii;
    u8 *ucTimerPins;
    
    ucTimerPins = (u8*)PYGMY_TIMERPINS;
    for( i = 0; i < 4; i++ ){
        for( ii = 0; ii < 4; ii ++ ){
            if( *(ucTimerPins + ( i * 4 ) + ii ) == ucPin ){
                if( i == 0 ){
                    return( TIM1 );
                } else if( i == 1 ){
                    return( TIM2 );
                } else if( i == 2 ){
                    return( TIM3 );
                } else if( i == 3 ){
                    return( TIM4 );
                } else{
                    return( NULL );
                }
            } // if
        } // for
    } // for
}

u8 getChannelFromPin( u8 ucPin )
{
    u8 i, ii, *ucTimerPins;
    
    ucTimerPins = (u8*)PYGMY_TIMERPINS;
    for( i = 0; i < 4; i++ ){
        for( ii = 0; ii < 4; ii ++ ){
            if( *(ucTimerPins + ( i * 4 ) + ii ) == ucPin ){
                return( 1 + ii );
            } // if
        } // for
    } // for
    
    return( 0 ); // 0 is an invalid channel
}

u8 pinCounter( u8 ucPin )
{

}

u8 pinPWM( u8 ucPin, u32 ulFreq, u8 ucDutyCycle )
{
    TIMER *pygmyTimer;
    TIMER1 *pygmyTimer1;
    u32 ulDuty;
    u8 ucChannel;

    // ToDo: Add clock frequency to pygmyGlobalData and use to set frequency here
    
    pygmyTimer = getTimerFromPin( ucPin );
    pygmyTimer1 = getTimerFromPin( ucPin );//(TIMER1 *)pygmyTimer; 
    ucChannel = getChannelFromPin( ucPin );
    
    if( !ucChannel || !pygmyTimer || ucDutyCycle > 100){
        return( 0 );
    } // if
    if( ulFreq == 0 ){
        ulFreq = 1;
    } // if
    if( ulFreq > 24000000 ){
        ulFreq = 24000000;
    } // if
    ulFreq = 24000000 / ulFreq;
    ulDuty = ( ulFreq / 100 ) * ucDutyCycle;
       
    pinConfig( ucPin, ALT );
    if( pygmyTimer1 == TIM1 ){
        PYGMY_RCC_TIMER1_ENABLE;
        pygmyTimer1->CR1 = 0;            // Disable before configuring timer
        pygmyTimer1->CR2 = 0;                        //
        pygmyTimer1->BDTR |= (TIM_MOE|TIM_AOE);
        pygmyTimer1->CNT = 0;                        // Count Register
        pygmyTimer1->ARR = ulFreq;                   // Auto Reload Register
        pygmyTimer1->PSC = 0;                        // Prescaler
        if( ucChannel == 1 ){   
            pygmyTimer1->CCR1 = ulDuty;              // Capture Compare for PWM in output, sets duty cycle
            pygmyTimer1->CCMR1 &= ~(TIM_OC1M_CLEAR|TIM_OC1PE);
            pygmyTimer1->CCMR1 |= (TIM_OC1M_PWM1|TIM_OC1PE);          // 
            pygmyTimer1->CCER |= TIM_CC1E;
        } else if( ucChannel == 2 ){
            pygmyTimer1->CCR2 = ulDuty;              // Capture Compare for PWM in output, sets duty cycle
            pygmyTimer1->CCMR1 &= ~(TIM_OC2M_CLEAR|TIM_OC2PE);
            pygmyTimer1->CCMR1 |= (TIM_OC2M_PWM1|TIM_OC2PE);          // 
            pygmyTimer1->CCER |= TIM_CC2E;
        } else if( ucChannel == 3 ){
            pygmyTimer1->CCR3 = ulDuty;              // Capture Compare for PWM in output, sets duty cycle
            pygmyTimer1->CCMR2 &= ~(TIM_OC3M_CLEAR|TIM_OC3PE);
            pygmyTimer1->CCMR2 |= (TIM_OC3M_PWM1|TIM_OC3PE);          // 
            pygmyTimer1->CCER |= TIM_CC3E;
        } else if( ucChannel == 4 ){
            pygmyTimer1->CCR4 = ulDuty;              // Capture Compare for PWM in output, sets duty cycle
            pygmyTimer1->CCMR2 &= ~(TIM_OC4M_CLEAR|TIM_OC4PE);
            pygmyTimer1->CCMR2 |= (TIM_OC4M_PWM1|TIM_OC4PE);          // 
            pygmyTimer1->CCER |= TIM_CC4E;
        } // else if
        pygmyTimer1->EGR |= TIM_UG;
        pygmyTimer1->CR1 = TIM_ARPE|TIM_CEN;
    } else{
        PYGMY_RCC_TIMER3_ENABLE;
        PYGMY_RCC_TIMER4_ENABLE;
        pygmyTimer->CR1 = 0;//&= ~( TIM_CEN );            // Disable before configuring timer
        pygmyTimer->CR2 = 0;                        //
        //pygmyTimer->SMCR = 0;                       //
        //pygmyTimer->DIER = 0;                       // DMA and interrupt enable register
        pygmyTimer->CNT = 0;                        // Count Register
        pygmyTimer->ARR = ulFreq;                   // Auto Reload Register
        pygmyTimer->PSC = 0;                        // Prescaler
        if( ucChannel == 1 ){
            pygmyTimer->CCR1 = ulDuty;              // Capture Compare for PWM in output, sets duty cycle
            pygmyTimer->CCMR1 &= ~(TIM_OC1M_CLEAR|TIM_OC1PE);
            pygmyTimer->CCMR1 |= (TIM_OC1M_PWM1|TIM_OC1PE);          // 
            pygmyTimer->CCER |= TIM_CC1E;
        } else if( ucChannel == 2 ){
            pygmyTimer->CCR2 = ulDuty;              // Capture Compare for PWM in output, sets duty cycle
            pygmyTimer->CCMR1 &= ~(TIM_OC2M_CLEAR|TIM_OC2PE);
            pygmyTimer->CCMR1 |= (TIM_OC2M_PWM1|TIM_OC2PE);          // 
            pygmyTimer->CCER |= TIM_CC2E;
        } else if( ucChannel == 3 ){
            pygmyTimer->CCR3 = ulDuty;      // Capture Compare for PWM in output, sets duty cycle
            pygmyTimer->CCMR2 &= ~(TIM_OC3M_CLEAR|TIM_OC3PE);          // 
            pygmyTimer->CCMR2 |= (TIM_OC3M_PWM1|TIM_OC3PE);
            pygmyTimer->CCER |= TIM_CC3E;
        } else if( ucChannel == 4 ){
            pygmyTimer->CCR4 = ulDuty;      // Capture Compare for PWM in output, sets duty cycle
            pygmyTimer->CCMR2 &= ~(TIM_OC4M_CLEAR|TIM_OC4PE);          // 
            pygmyTimer->CCMR2 |= (TIM_OC4M_PWM1|TIM_OC4PE);
            pygmyTimer->CCER |= TIM_CC4E;
        } // else if
        pygmyTimer->EGR |= TIM_UG;
        pygmyTimer->CR1 = TIM_ARPE|TIM_CEN;
    } // else
    
    return( 1 );
}

void EXTI0_IRQHandler( void ) 
{
    EXTI->PR |= BIT0;      // Clear pending bits to prevent recursive access
    ( globalIRQHandlers[ 0 ] )();
}

void EXTI1_IRQHandler( void ) 
{
    EXTI->PR |= BIT1;      // Clear pending bits to prevent recursive access
    ( globalIRQHandlers[ 1 ] )();
}

void EXTI2_IRQHandler( void ) 
{
    EXTI->PR |= BIT2;      // Clear pending bits to prevent recursive access
    ( globalIRQHandlers[ 2 ] )();
}

void EXTI3_IRQHandler( void ) 
{
    EXTI->PR |= BIT3;      // Clear pending bits to prevent recursive access
    ( globalIRQHandlers[ 3 ] )();
}

void EXTI4_IRQHandler( void ) 
{
    EXTI->PR |= BIT4;      // Clear pending bits to prevent recursive access
    ( globalIRQHandlers[ 4 ] )();
}

void EXTI9_5_IRQHandler( void ) 
{
    if( EXTI->PR & BIT5 ){
        EXTI->PR |= BIT5;      // Clear pending bits to prevent recursive access
        ( globalIRQHandlers[ 5 ] )();
    } // if
    if( EXTI->PR & BIT6 ){
        EXTI->PR |= BIT6;       
        ( globalIRQHandlers[ 6 ] )();
    } // if 
    if( EXTI->PR & BIT7 ){
        EXTI->PR |= BIT7;
        ( globalIRQHandlers[ 7 ] )();
    } // if
    if( EXTI->PR & BIT8 ){
        EXTI->PR |= BIT8;
        ( globalIRQHandlers[ 8 ] )();
    } // if
    if( EXTI->PR & BIT9 ){
        EXTI->PR |= BIT9;
        ( globalIRQHandlers[ 9 ] )();
    } // if
    
}

void EXTI15_10_IRQHandler( void ) 
{
    if( EXTI->PR & BIT10 ){
        EXTI->PR |= BIT10;
        ( globalIRQHandlers[ 10 ] )();
    } // if
    if( EXTI->PR & BIT11 ){
        EXTI->PR |= BIT11;
        ( globalIRQHandlers[ 11 ] )();
    } //if 
    if( EXTI->PR & BIT12 ){
        EXTI->PR |= BIT12;
        ( globalIRQHandlers[ 12 ] )();
    } // if
    if( EXTI->PR & BIT13 ){
        EXTI->PR |= BIT13;
        ( globalIRQHandlers[ 13 ] )();
    } // if
    if( EXTI->PR & BIT14 ){
        EXTI->PR |= BIT14;
        ( globalIRQHandlers[ 14 ] )();
    } // if
    if( EXTI->PR & BIT15 ){
        EXTI->PR |= BIT15;
        ( globalIRQHandlers[ 15 ] )();
    } // else if
}
