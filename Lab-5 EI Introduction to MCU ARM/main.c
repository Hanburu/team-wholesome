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

int main(void)
{
    /* Stop WDT  */
    MAP_WDT_A_holdTimer();

    /* Configuring outputs */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN5);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);
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

    /* Sleeping when not in use */
    while (1)
    {
        MAP_PCM_gotoLPM0();
    }
}

void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT1 & status)
    {
        if (input_ADC >= (ref_ADC * 0.125)){
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);
        }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0);
        }
        if (input_ADC >= (ref_ADC * 0.250)){
            MAP_ADC14_toggleConversionTrigger();
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN1);
        }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN1);
        }
        if (input_ADC >= (ref_ADC * 0.375)){
            MAP_ADC14_toggleConversionTrigger();
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN2);
        }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2);
        }
        if (input_ADC >= (ref_ADC * 0.500)){
            MAP_ADC14_toggleConversionTrigger();
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN3);
        }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN3);
        }
        if (input_ADC >= (ref_ADC * 0.625)){
            MAP_ADC14_toggleConversionTrigger();
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
        }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN4);
        }
        if (input_ADC >= (ref_ADC * 0.750)){
            MAP_ADC14_toggleConversionTrigger();
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
        }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
        }
        if (input_ADC >= (ref_ADC * 0.875)){
            MAP_ADC14_toggleConversionTrigger();
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
        }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
        }
        if (input_ADC >= (ref_ADC * 1.000)){
            MAP_ADC14_toggleConversionTrigger();
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);
        }
        else{
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
        }
        MAP_ADC14_toggleConversionTrigger();
    }
}
