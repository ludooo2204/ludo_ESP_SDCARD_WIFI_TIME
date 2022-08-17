#include <Adafruit_MAX31865.h>
float pt100;
// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 max1 = Adafruit_MAX31865(33, 19, 23, 18);
// Adafruit_MAX31865 max1 = Adafruit_MAX31865(33);
// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF 430.0

#define RNOMINAL 100.0

void init_pt100()
{
    max1.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary
    // Check and print any faults
    uint8_t fault = max1.readFault();
    if (fault)
    {
        Serial.print("Fault 0x");
        Serial.println(fault, HEX);
        if (fault & MAX31865_FAULT_HIGHTHRESH)
        {
            Serial.println("RTD High Threshold");
        }
        if (fault & MAX31865_FAULT_LOWTHRESH)
        {
            Serial.println("RTD Low Threshold");
        }
        if (fault & MAX31865_FAULT_REFINLOW)
        {
            Serial.println("REFIN- > 0.85 x Bias");
        }
        if (fault & MAX31865_FAULT_REFINHIGH)
        {
            Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
        }
        if (fault & MAX31865_FAULT_RTDINLOW)
        {
            Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
        }
        if (fault & MAX31865_FAULT_OVUV)
        {
            Serial.println("Under/Over voltage");
        }
        max1.clearFault();
    }
}

float moyenne_pt100(int nbr_valeur)
{
    float total = 0;
    for (int i = 1; i <= nbr_valeur; i++)
    {

        total = total + max1.temperature(RNOMINAL, RREF);
        delay(10);
    }

    total = total / nbr_valeur;
    Serial.print("pt100 = ");
    Serial.println(total, 3);
    return total;
}
float getPT100()
{
    return moyenne_pt100(10);
}
