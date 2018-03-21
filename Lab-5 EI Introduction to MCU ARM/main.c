/* Here will go the code for the fourth lab of the Control Engineering course */
/* CENTRO DE ENSEÑANZA TÉCNICA Y SUPERIOR
 * Engineering in Cybernetics Electronics
 * Subject: Control Engineering
 * 
 * Lab Practice 5: Introduction to MCU ARM
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

/* Statics */
static volatile uint16_t input_ADC;
static volatile uint16_t ref_ADC;
static volatile bool mode = true;

int main(void)
{
    /* Stop WDT  */
    MAP_WDT_A_holdTimer();

    /* Configuring outputs */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN7);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN1);
 
    /* LED for showing if mode bool is ON/OFF */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
 
    /* Configuring P1.1 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    MAP_Interrupt_enableInterrupt(INT_PORT1);

    /* Initializing Variables */
    input_ADC = 0;
    ref_ADC = 0;

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

void bar_Display (int,int);
void dot_Display (int,int);

/* Code for the Bar Display */
void bar_Display(int input_ADC, int ref_ADC){

}

/* Code for the Dot Display */
void dot_Display(int input_ADC, int ref_ADC){
    
}

void ADC14_IRQHandler(void)
{
    /* Check convertion status or the interrupt */
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);
    /* Get values from the ADC conversion*/
    input_ADC= MAP_ADC14_getResult(ADC_MEM0);
    ref_ADC= MAP_ADC14_getResult(ADC_MEM0);

    if (ADC_INT1 & status)
    {
        /* Choose what mode for the display we will be using */
        if (mode == true){
            bar_Display(input_ADC,ref_ADC);
        }
        else{
            dot_Display(input_ADC,ref_ADC);
        }
        MAP_ADC14_toggleConversionTrigger();
     
        /* Delay method */
        __delay_cycles(10000);
    }
}

void PORT1_IRQHandler(void)
{
    /* Check status of the button push interrup */
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN1)
    {
        /* Toggle the display mode */
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        mode= !mode;
    }

}
