/*

"Maximal" sketch to demonstrate emonLibCM

This demonstrates the use of every API function.
This sketch provides an example of every Application Interface function. 
Many in fact set the default value for the emonTx V3.4, and are therefore
not needed in most cases. If you do need to change a value, the 
Application Interface section of the User Documentation gives full details.

Configuracion de 4 canales del ecoPower y 1 de voltage
eliminadas las referencias a temperatura y pulsos

Para que funcionen las lecturas sin tener referencia de tensión
En la libreria EmonTxCM esta comentada la linea:
acPresent = acSense > acDetectedThreshold
09-07-2020 Ajustado el valor de "Voltage Calibration Trans 8V F&F  258.00"

Ejemplo de configuración

https://github.com/openenergymonitor/EmonLibCM/blob/master/Examples/EmonTxV34CM_max/EmonTxV34CM_max.ino

*/

#include <Arduino.h>
#include "emonLibCM.h"

#include <SoftwareSerial.h>
#define RXPIN 2    //11
#define TXPIN 3    //12
SoftwareSerial wifiSerialInit (RXPIN,TXPIN);

#define BPS 115200 //Velocidad de las comunicaciones
#define RTR 97.6 //resistencia para el transformador de intensidad

bool recalibrate = false;                                  //  Do not demonstrate the recalibration functions
   

void setup() 
{  
  Serial.begin(BPS);
  wifiSerialInit.begin(BPS);
  
  Serial.println("\nEmonTx v3.4 EmonLibCM Continuous Monitoring Maximal Demo"); 

  EmonLibCM_SetADC_VChannel(6, 258.00);                    // ADC Input channel, voltage calibration - for Ideal UK Adapter = 268.97   Trans 8V F&F  258.00
  EmonLibCM_SetADC_IChannel(0, RTR, 1.0);                // ADC Input channel, current calibration, phase calibration
  EmonLibCM_SetADC_IChannel(1, RTR, 1.0);                //  The current channels will be read in this order
  EmonLibCM_SetADC_IChannel(2, RTR, 1.0);                //  90.91 for 100 A : 50 mA c.t. with 22R burden - v.t. leads c.t by ~4.2 degrees
  EmonLibCM_SetADC_IChannel(3, RTR, 1.0);                //  16.67 for 100 A : 50 mA c.t. with 120R burden - v.t. leads c.t by ~1 degree
  EmonLibCM_SetADC_IChannel(4, RTR, 1.0); 
  EmonLibCM_SetADC_IChannel(5, RTR, 1.0); 

  EmonLibCM_setADC(10, 104);                               // ADC Bits (10 for emonTx & Arduino except Due=12 bits, ADC Duration 104 us for 16 MHz operation)
  EmonLibCM_ADCCal(5.0);                                   // ADC Reference voltage, (3.3 V for emonTx,  5.0 V for Arduino)
  
  EmonLibCM_cycles_per_second(50);                         // mains frequency 50Hz, 60Hz
  EmonLibCM_datalog_period(10);                            // period of readings in seconds - normal value for emoncms.org
  
  EmonLibCM_min_startup_cycles(10);                        // number of cycles to let ADC run before starting first actual measurement

  EmonLibCM_setPulseEnable(false);                          // Enable pulse counting
  EmonLibCM_TemperatureEnable(false); 


  EmonLibCM_Init();                                        // Start continuous monitoring.

}

void loop()             
{
  if (EmonLibCM_Ready())   
  {
    String messageToTx = "";
    
    Serial.println(EmonLibCM_acPresent()?"AC present ":"AC missing ");
   
    delay(50);

    Serial.print(" V=");Serial.print(EmonLibCM_getVrms());
    Serial.print(" f=");Serial.println(EmonLibCM_getLineFrequency(),2);   

   
    for (byte ch=0; ch<6; ch++){

        Serial.print("Ch ");Serial.print(ch+1);
        Serial.print(" I=");Serial.print(EmonLibCM_getIrms(ch),3);
        Serial.print(" W=");Serial.print(EmonLibCM_getRealPower(ch));
        Serial.print(" VA=");Serial.print(EmonLibCM_getApparentPower(ch));
        Serial.print(" Wh=");Serial.print(EmonLibCM_getWattHour(ch));
        Serial.print(" pf=");Serial.print(EmonLibCM_getPF(ch),4);   
        Serial.println();
        delay(10);
        
        // messageToTx = "";
        // messageToTx = "W"+ String(ch+1) + ":"+ EmonLibCM_getRealPower(ch);
        // wifiSerialInit.println (messageToTx);

        // Serial.println(messageToTx);
        delay(10);
    } 
    delay(10);       
  }
}
