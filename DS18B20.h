
#include <OneWire.h>
#include <DallasTemperature.h>

// Data DS18B20 temperature
#define ONE_WIRE_BUS 14
String dataPiscine;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

float tempC;

void init_ds18b20()
{
    delay(500);
    sensors.begin();

    if (!sensors.getAddress(insideThermometer, 0))
        Serial.println("Unable to find address for Device 0");
    sensors.setResolution(insideThermometer, 12);

    delay(500);
}

// function to print the temperature for a device
float getDS18B20(DeviceAddress deviceAddress)
{
    sensors.requestTemperatures(); // Send the command to get temperatures
    float tempC = sensors.getTempC(deviceAddress);
    delay(1000);
    if (tempC > -10 && tempC < 100)
    {
        //  Serial.println(tempC,1);
        tempC = tempC * tempC * 0.001831 + 0.882817 * tempC + 2.307283;
        Serial.print("Temp corrigé = ");
        Serial.println(tempC, 1);
    }
    else
    {

        Serial.println("bug");
    }
    return tempC;
}