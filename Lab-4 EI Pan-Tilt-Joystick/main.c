/* Here will go the code for the fourth lab of the Control Engineering course */
/* CENTRO DE ENSEÑANZA TÉCNICA Y SUPERIOR
 * Engineering in Cybernetics Electronics
 * Subject: Control Engineering
 * 
 * Lab Practice 4: Pan-Tilt-Joystick
 * Date: -
 * 
 * Team members: Ana Valeria González
 *               Arturo López
 *               César Martínez
 *               Fernando Raúl Cortez
 *               Andrés Marín
 *

/* DriverLib Includes */
#include <driverlib.h>
#include <msp432.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Application Defines */
#define TIMER_A_PERIOD 32000

/* Statics */
static volatile uint16_t x_ADC_Res;
static volatile uint16_t y_ADC_Res;
static volatile uint_fast16_t normalized_x_Res;
static volatile uint_fast16_t normalized_y_Res;

Timer_A_PWMConfig pwmConfig_x =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_64,
        TIMER_A_PERIOD,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0
};

Timer_A_PWMConfig pwmConfig_y =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_64,
        TIMER_A_PERIOD,
        TIMER_A_CAPTURECOMPARE_REGISTER_2,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0
};

int main(void)
{
    /* Stop WDT  */
    MAP_WDT_A_holdTimer();

    //P2.4 and P2.5 output
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setting Flash wait state */
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Setting DCO to 48MHz  */
    MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

    /* Enabling the FPU for floating point operation */
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();

    //![Single Sample Mode Configure]
    /* Initializing ADC (MCLK/1/4) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4,0);

    /* Configuring GPIOs (5.5 A0) (5.4 A1) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,
    GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4,
    GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
    ADC_INPUT_A0, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS,
    ADC_INPUT_A1, false);

    /* Configuring Sample Timer */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Configuring Initial PWM */
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig_x);
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig_y);

    /* Enabling/Toggling Conversion */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

    /* Enabling interrupts and starting the watchdog timer */
    MAP_ADC14_enableInterrupt(ADC_INT1);
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();

    /* Sleeping when not in use 
    while (1)
    {
        MAP_PCM_gotoLPM0();
    }*/
}

void ADC14_IRQHandler(void)
{

    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT1 & status)
    {
        x_ADC_Res= MAP_ADC14_getResult(ADC_MEM0);
        y_ADC_Res= MAP_ADC14_getResult(ADC_MEM1);
        if ((x_ADC_Res <= (x_ADC_Res * 1.1)) && (x_ADC_Res >= (x_ADC_Res * 0.9))){

        }
        else{
            normalized_x_Res = 2*(x_ADC_Res);
        }
        if ((y_ADC_Res <= (y_ADC_Res * 1.1)) && (y_ADC_Res >= (y_ADC_Res * 0.9))){

        }
        else{
            normalized_y_Res = 2*(y_ADC_Res);
        }
        pwmConfig_x.dutyCycle = normalized_x_Res;
        pwmConfig_y.dutyCycle = normalized_y_Res;
        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig_x);
        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig_y);
        MAP_ADC14_toggleConversionTrigger();
    }
    __delay_cycles(10000);
}

