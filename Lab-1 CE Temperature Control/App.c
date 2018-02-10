/* Here will be the code for the first lab of the course of Control Engineering */
/* CENTRO DE ENSEÑANZA TÉCNICA Y SUPERIOR
 * Engineering in Cybernetics Electronics
 * Subject: Control Engineering
 * 
 * Lab Practice 1: Sensor Temperature Control
 * Date: February 9th, 2018
 * 
 * Team members: Ana Valeria González
 *               Arturo López
 *               César Martínez
 *               Fernando Raúl Cortez
 *               Andrés Marín
 */

//Include LCD library
#include <LiquidCrystal.h>

//Declare LCD Object
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Declare pin numbers
const int sensorPin = A0;
const int fanPin = 9;

// Declare a constant that will hold the voltage/degree ration of the temperature sensor [V/°C]
const double VOLTAGE_TEMP_RATIO = 0.01;

// Declare lowest boundary of the temperature intervals
const int TEMP_INTERVAL_1 = 20;
const int TEMP_INTERVAL_2 = 22;
const int TEMP_INTERVAL_3 = 25;
const int TEMP_INTERVAL_4 = 28;

// Declare the PWM values for the fan speed depending on each temperature interval
const int PWM_INTERVAL_1 = 70;
const int PWM_INTERVAL_2 = 130;
const int PWM_INTERVAL_3 = 190;
const int PWM_INTERVAL_4 = 255; 

/*
 * Method that converts the digital value read by the analog pin and converts it into the propiate voltage
 */
double convertDigitalToVoltage(int read_byte) {
  const double REFERENCE_VOLTAGE = 5.0;
  const double MAX_DIGITAL_VALUE = 1023.0;
  
  return ((double)read_byte) * REFERENCE_VOLTAGE / MAX_DIGITAL_VALUE; 
}

/*
 * Method that converts the read voltage of the analog pin into the temperature read by the sensor
 */
double convertVoltageToCelsius(double read_voltage, double voltage_per_degree) {
    return read_voltage / voltage_per_degree;
}

/*
 * Method that determines the interval in which the read temperature is and gets the PWM to control the speed of a fan
 */
int getFanPWMFromTemperature(double temperature) {
  if (temperature >= TEMP_INTERVAL_4) {
    return PWM_INTERVAL_4;
  }
  else if (temperature >= TEMP_INTERVAL_3) {
    return PWM_INTERVAL_3;
  }
  else if (temperature >= TEMP_INTERVAL_2) {
    return PWM_INTERVAL_2;
  }
  else if (temperature >= TEMP_INTERVAL_1) {
    return PWM_INTERVAL_1;
  }
  else {
    return 0;
  }
}

/*
 * Arduino's setup method
 */
void setup() {
  // Set the mode for the used pins
  pinMode(sensorPin, INPUT);
  pinMode(fanPin, OUTPUT);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Control B6");
  // Set the baud rate for the Serial Monitor
  Serial.begin(9600);
}

/*
 * Arduino's main method
 */
void loop() {
  // Read the digital value given by the sensorPin's ADC
  int sensorValue = analogRead(sensorPin);

  // Convert the read digital value into a voltage (analog) value
  double voltageValue = convertDigitalToVoltage(sensorValue);

  // Convert the read voltage into a temperature in degrees Celsius
  double temperatureValue = convertVoltageToCelsius(voltageValue, VOLTAGE_TEMP_RATIO);

  // Print to the Serial Monitor the current temperature [°C] for testing purposes
  Serial.print("Temperature: ");
  Serial.println(temperatureValue);

  // Set the PWM of the fanPin to control its speed according to the temperature.
  analogWrite(fanPin, getFanPWMFromTemperature(temperatureValue));
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  //Print a message to second line of LCD
  lcd.print(temperatureValue);
  
  // Repeat the cycle once per second
  delay(1000);
}
